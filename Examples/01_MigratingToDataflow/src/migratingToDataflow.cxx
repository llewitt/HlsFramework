#include <cstdlib>

#include "hls_stream.h"

namespace {
  void loader(const int* input, hls::stream<int, 1>& output, std::size_t vsize) {
    #pragma HLS INLINE off

    for (std::size_t index = 0; index < vsize; index++) {
      output.write(input[index]);
    }
  }

  void square(hls::stream<int, 1>& input, hls::stream<int, 1>& output) {
    #pragma HLS INLINE off

    while (true) {
      const int number = input.read();

      if (number < 0) {
        output.write(number);
        
        break;
      }
      else {
        output.write(number * number);
      }
    }
  }

  void unloader(hls::stream<int, 1>& input, int* output) {
    #pragma HLS INLINE off

    std::size_t index = 0;

    while (true) {
      const int number = input.read();

      output[index] = number;
      index++;
      
      if (number < 0) {
        break;
      }
    }
  }
}

void migratingToDataflow(
  const int* input, 
  int* output, 
  const std::size_t vsize
) {
  #pragma HLS DATAFLOW
  #pragma HLS INTERFACE mode=m_axi port=input depth=8192
  #pragma HLS INTERFACE mode=m_axi port=output depth=8192

  hls::stream<int, 1> loaderOutput("loaderOutput");
  hls::stream<int, 1> unloaderInput("unloaderInput");

  loader(input, loaderOutput, vsize);
  square(loaderOutput, unloaderInput);
  unloader(unloaderInput, output);
}

#ifndef __SYNTHESIS__

#include "UnitTesting/unitTesting.h"

int main() {
  migratingToDataflow(
    UnitTesting::UnitTestableInputArray<int, 8192>({0, 1, 2, 3, 4, 5, 6, -1}), 
    UnitTesting::UnitTestableOutputArray<int, 8192>({0, 1, 4, 9, 16, 25, 36, -1}),
    8
  );
}

#endif

