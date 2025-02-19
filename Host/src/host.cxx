#include <iostream>
#include <fstream>

#include "nlohmann/json.hpp"

#include "xrt/xrt_bo.h"
#include "xrt/xrt_device.h"
#include "xrt/xrt_kernel.h"

namespace {
  static constexpr std::size_t bufferSize = 8192;

  struct Kernel {
    const std::string& name;
    xrt::kernel& kernel;
    xrt::run run;

    Kernel(
      const std::string& name, 
      xrt::kernel&& kernel
    ) : 
      name(name), 
      kernel(kernel), 
      run(kernel) {
    }
  };

  struct InputInterface {
    const std::string& fileName;
    xrt::bo bufferObject;
  };

  struct OutputInterface {
    const std::string& fileName;
    xrt::bo bufferObject;
  };
}

int main(int argc, const char* argv[]) {
  if (argc != 3) {
    std::cout << "Useage: " << argv[0] << " <firmware.xclbin> <interfaces.json> <sequence.json>" << std::endl;

    return -1;
  }

  const nlohmann::json interfaces{nlohmann::json::parse(std::ifstream{argv[1]})};
  const nlohmann::json stages{nlohmann::json::parse(std::ifstream{argv[2]})};

  xrt::device device{0};

  std::vector<Kernel> kernels{};
  for (const nlohmann::json interface : interfaces) {
    const std::string& kernelName = interface.at("kernelName").get<const std::string>();

    kernels.push_back(Kernel{
      kernelName,
      std::move(xrt::kernel(device, device.get_xclbin_uuid(), kernelName))
    });
  }

  std::vector<InputInterface> inputInterfaces{};
  std::vector<OutputInterface> outputInterfaces{};
  for (const nlohmann::json interface : interfaces) {
    const std::string& kernelName = interface.at("kernelName").get<const std::string>();
    const std::string& mode = interface.at("mode").get<const std::string>();
    const std::string& fileName = interface.at("fileName").get<const std::string>();
    const int argumentIndex = std::stoi(interface.at("argumentIndex").get<const std::string>());
      
    if (mode == "INPUT") {
      for (Kernel& kernel : kernels) {
        if (kernelName != kernel.name) {
          continue; 
        }

        inputInterfaces.push_back(InputInterface{
          .fileName = fileName,
          .bufferObject = std::move(xrt::bo(
                device, 
                sizeof(float) * bufferSize, 
                xrt::bo::flags::normal, 
                kernel.kernel.group_id(argumentIndex)
          ))
        });

        kernel.run.set_arg(argumentIndex, inputInterfaces.back().bufferObject);

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
          .bufferObject = std::move(xrt::bo(
                device, 
                sizeof(float) * bufferSize, 
                xrt::bo::flags::normal, 
                kernel.kernel.group_id(argumentIndex)
          ))
        });

        kernel.run.set_arg(argumentIndex, outputInterfaces.back().bufferObject);

        break;
      }
    }
  }

  for (const nlohmann::json interface : interfaces) {
    const std::string& kernelName = interface.at("kernelName").get<const std::string>();
    const std::string& mode = interface.at("mode").get<const std::string>();
    const std::string& fileName = interface.at("fileName").get<const std::string>();
    const int argumentIndex = std::stoi(interface.at("argumentIndex").get<const std::string>());
      
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

  for (const nlohmann::json interface : interfaces) {
    const std::string& kernelName = interface.at("kernelName").get<const std::string>();
    const std::string& mode = interface.at("mode").get<const std::string>();
    const std::string& fileName = interface.at("fileName").get<const std::string>();
    const int argumentIndex = std::stoi(interface.at("argumentIndex").get<const std::string>());
      
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

  for (const nlohmann::json stage : stages) {
    for (const std::string& kernelName :  stage) {
      for (Kernel& kernel : kernels) {
        if (kernel.name != kernelName) {
          continue;
        }

        kernel.run.start();

        break;
      }

      for (Kernel& kernel : kernels) {
        if (kernel.name != kernelName) {
          continue;
        }

        kernel.run.wait();

        break;
      }
    }
  }
}

