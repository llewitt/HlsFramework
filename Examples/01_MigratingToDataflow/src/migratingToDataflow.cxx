#include <cstdlib>

#include "hls_stream.h"

namespace {
  void loader(const float* input, hls::stream<float>& output) {
    std::size_t index = 0;
    while (true) {
      const float number = input[index];

      output.write(number);

      if (*reinterpret_cast<const int*>(&number) & (1 << 31)) {
        break;
      }

      index++;
    }
  }

  void square(hls::stream<float>& input, hls::stream<float>& output) {
    while (true) {
      const float number = input.read();

      if (*reinterpret_cast<const int*>(&number) & (1 << 31)) {
        output.write(number);
        
        break;
      }
      else {
        output.write(number * number);
      }
    }
  }

  void unloader(hls::stream<float>& input, float* output) {
    std::size_t index = 0;
    while (true) {
      const float number = input.read();

      output[index] = number;

      if (*reinterpret_cast<const int*>(&number) & (1 << 31)) {
        break;
      }

      index++;
    }
  }
}

void migratingToDataflow(const float* input, float* output) {
  #pragma HLS DATAFLOW

  hls::stream<float> loaderOutput("loaderOutput");
  hls::stream<float> unloaderInput("unloaderInput");

  loader(input, loaderOutput);
  square(loaderOutput, unloaderInput);
  unloader(unloaderInput, output);
}

#ifndef __SYNTHESIS__
#include <array>

int main() {
  const std::array<float, 9> input{0, 1, 2, 3, 4, 5, 6, 7, -1};
  std::array<float, 9> output{};
  const std::array<float, 9> outputReference{0, 1, 4, 9, 16, 25, 36, 49, -1};

  migratingToDataflow(input.data(), output.data());
  for (std::size_t index = 0; index < output.size(); index++) {
    if (output[index] != outputReference[index]) {
       return -1;
    }
  }

  return 0;
}

#endif

