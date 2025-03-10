#include <cstdlib>

#include "hls_stream.h"

namespace {
  static constexpr std::size_t numberOfSorters = 128;

  void sort(hls::stream<int>& input, hls::stream<int>& output) {
    #pragma HLS INLINE off

    int buffer = -1;

    while (true) {
      const int number = input.read();

      if (number >= 0 && buffer < 0) {
        buffer = number;
      }
      else if (number < 0 && buffer < 0) {
        buffer = number;

        break;
      }
      else if (number < 0) {
        output.write(buffer);
        buffer = number;

        break;
      }
      else if (number >= buffer){
        output.write(number); 
      }
      else {
        output.write(buffer); 
        buffer = number; 
      }
    }

    output.write(buffer);
  }
};

void parameterisingFirmware_SolutionA(hls::stream<int>& input, hls::stream<int>& output) {
  #pragma HLS INTERFACE ap_ctrl_none port=return
  #pragma DATAFLOW

  static_assert(numberOfSorters > 0, "Maximum input size must be at least 4.");
  
  hls::stream<int, 1> internal[numberOfSorters]{};

  sort(input, internal[0]);
  
  for (std::size_t index = 0; index < numberOfSorters - 2; index++) {
    #pragma HLS UNROLL

    sort(internal[index], internal[index + 1]);
  }

  sort(internal[numberOfSorters - 2], output);
}

#ifndef __SYNTHESIS__

#include "UnitTesting/unitTesting.h"

int main() {
  parameterisingFirmware_SolutionA(
    UnitTesting::UnitTestableInputStream<int>({
      16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 17, -1
    }), 
    UnitTesting::UnitTestableOutputStream<int>({
      17, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1
    })
  );
}

#endif

