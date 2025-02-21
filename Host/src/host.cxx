#include <iostream>
#include <fstream>
#include <sstream>

#include "nlohmann/json.hpp"

#include "xrt/xrt_bo.h"
#include "xrt/xrt_device.h"
#include "xrt/xrt_kernel.h"

namespace {
  static constexpr std::size_t bufferSize = 8192;

  struct Kernel {
    const std::string name;
    xrt::kernel kernel;
    xrt::run run;

    Kernel(
      const std::string name, 
      xrt::kernel kernel
    ) : 
      name(name), 
      kernel(kernel), 
      run(kernel) {
    }
  };

  struct InputInterface {
    const std::string fileName;
    xrt::bo bufferObject;
  };

  struct OutputInterface {
    const std::string fileName;
    xrt::bo bufferObject;
  };
}

int main(int argc, const char* argv[]) {
  if (argc != 4) {
    std::cout << 
      "Useage: " << 
      argv[0] << 
      " <firmware.xclbin>" << 
      " <interfaces.json>" << 
      " <sequence.json>" << std::endl;

    return -1;
  }

  const nlohmann::json interfaces{nlohmann::json::parse(std::ifstream{argv[2]})};
  const nlohmann::json stages{nlohmann::json::parse(std::ifstream{argv[3]})};

  xrt::device device{0};
  const xrt::uuid uuid = device.load_xclbin(argv[1]);

  std::vector<Kernel> kernels{};
  for (const nlohmann::json& interface : interfaces[0]) {
    const std::string& kernelName = interface.at("kernelName").get<const std::string>();

    if (![&]{
      for (const Kernel& otherKernel : kernels) {
        if (kernelName == otherKernel.name) {
          return false;
        }
      }

      return true;
    }()) {
      continue;
    }

    kernels.push_back(Kernel{
      kernelName,
      xrt::kernel(device, uuid, kernelName)
    });
  }

  std::vector<InputInterface> inputInterfaces{};
  std::vector<OutputInterface> outputInterfaces{};
  for (const nlohmann::json& interface : interfaces[0]) {
    const std::string& kernelName = interface.at("kernelName").get<const std::string>();
    const std::string& mode = interface.at("mode").get<const std::string>();
    const std::string& fileName = interface.at("fileName").get<const std::string>();
    const int argumentIndex = interface.at("argumentIndex").get<const int>();

    if (mode == "INPUT") {
      for (Kernel& kernel : kernels) {
        if (kernelName != kernel.name) {
          continue; 
        }

        inputInterfaces.push_back(InputInterface{
          .fileName = fileName,
          .bufferObject = xrt::bo(
                device, 
                sizeof(float) * bufferSize, 
                xrt::bo::flags::normal, 
                kernel.kernel.group_id(argumentIndex)
          )
        });

        kernel.run.set_arg(argumentIndex, inputInterfaces.back().bufferObject);

        std::vector<float> numbers{};
        std::ifstream file{fileName};

        std::string line{};
        while (std::getline(file, line)) {
          numbers.push_back(std::stof(line));
        }

        if (numbers.size() > bufferSize) {
          std::cout << 
            "Error: Input file (" << 
            numbers.size() << 
            ") exceeds buffer size (" <<
            bufferSize << 
            ")" << std::endl;

          return -1;
        }

        // Resize after push_back to ensure length is correct.
        numbers.resize(bufferSize);

        inputInterfaces.back().bufferObject.write(numbers.data(), numbers.size(), 0);
        inputInterfaces.back().bufferObject.sync(XCL_BO_SYNC_BO_TO_DEVICE);

        break;
      }
    }
    else if (mode == "OUTPUT") {
      for (Kernel& kernel : kernels) {
        if (kernelName != kernel.name) {
          continue; 
        }

        outputInterfaces.push_back(OutputInterface{
          .fileName = fileName,
          .bufferObject = xrt::bo(
                device, 
                sizeof(float) * bufferSize, 
                xrt::bo::flags::normal, 
                kernel.kernel.group_id(argumentIndex)
          )
        });

        kernel.run.set_arg(argumentIndex, outputInterfaces.back().bufferObject);

        break;
      }
    }
  }

  for (const nlohmann::json& interface : interfaces[0]) {
    const std::string& kernelName = interface.at("kernelName").get<const std::string>();
    const std::string& mode = interface.at("mode").get<const std::string>();
    const std::string& fileName = interface.at("fileName").get<const std::string>();
    const int argumentIndex = interface.at("argumentIndex").get<const int>();
      
    if (mode != "SHARED_INPUT") {
      continue;
    }

    for (InputInterface& inputInterface : inputInterfaces) {
      if (inputInterface.fileName != fileName) {
        continue;
      }

      for (Kernel& kernel : kernels) {
        if (kernel.name != kernelName) {
          continue; 
        }

        kernel.run.set_arg(argumentIndex, inputInterface.bufferObject);

        break;
      }

      break;
    }
  }

  for (const nlohmann::json& interface : interfaces[0]) {
    const std::string& kernelName = interface.at("kernelName").get<const std::string>();
    const std::string& mode = interface.at("mode").get<const std::string>();
    const std::string& fileName = interface.at("fileName").get<const std::string>();
    const int argumentIndex = interface.at("argumentIndex").get<const int>();
      
    if (mode != "SHARED_OUTPUT") {
      continue;
    }

    for (OutputInterface& outputInterface : outputInterfaces) {
      if (outputInterface.fileName != fileName) {
        continue;
      }

      for (Kernel& kernel : kernels) {
        if (kernel.name != kernelName) {
          continue; 
        }

        kernel.run.set_arg(argumentIndex, outputInterface.bufferObject);

        break;
      }

      break;
    }
  }

  for (const nlohmann::json stage : stages[0]) {
    for (const std::string& kernelName :  stage) {
      for (Kernel& kernel : kernels) {
        if (kernel.name != kernelName) {
          continue;
        }

        kernel.run.start();
        std::cout << kernelName << ":\tStarted" << std::endl;

        break;
      }
    }

    for (const std::string& kernelName :  stage) {
      for (Kernel& kernel : kernels) {
        if (kernel.name != kernelName) {
          continue;
        }

        kernel.run.wait();
        std::cout << kernelName << ":\tFinished" << std::endl;

        break;
      }
    }
  }

  for (OutputInterface& interface : outputInterfaces) {
    std::array<float, bufferSize> numbers{};

    interface.bufferObject.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    interface.bufferObject.read(numbers.data(), bufferSize, 0);

    std::ofstream file{interface.fileName};
    for (const float number : numbers) {
      file << number << std::endl;
    }
  }
}

