#include <cstdlib>

#include "hls_stream.h"

void allInOne(const float* input, float* output) {
  std::size_t index = 0;
  while (true) {
    const float number = input[index]; 


    if (number < 0) {
      output[index] = number;

      break;
    }
    else {
      output[index] = number * number;
    }

    index++;
  }
}

#ifndef __SYNTHESIS__
#include <array>

int main() {
  const std::array<float, 9> input{0, 1, 2, 3, 4, 5, 6, 7, -1};
  std::array<float, 9> output{};
  const std::array<float, 9> outputReference{0, 1, 4, 9, 16, 25, 36, 49, -1};

  allInOne(input.data(), output.data());
  for (std::size_t index = 0; index < output.size(); index++) {
    if (output[index] != outputReference[index]) {
       return -1;
    }
  }

  return 0;
}

#endif

