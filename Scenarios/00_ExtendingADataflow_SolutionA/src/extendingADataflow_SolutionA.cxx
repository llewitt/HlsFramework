#include <cstdlib>

#include "hls_stream.h"

namespace {
  void multiplyByThree(hls::stream<int>& input, hls::stream<int>& output) {
    #pragma HLS INLINE off

    while (true) {
      const int number = input.read();

      if (number < 0) {
        output.write(number);

        break;
      }
      else {
        output.write(3 * number); 
      }
    }
  }

  void keepEvenNumbers(hls::stream<int>& input, hls::stream<int>& output) {
    #pragma HLS INLINE off

    while (true) {
      const int number = input.read();

      if (number < 0) {
        output.write(number);

        break;
      }
      else if (number % 2 == 0) {
        output.write(number);
      }
    }
  }
};

void extendingADataflow_SolutionA(hls::stream<int>& input, hls::stream<int>& output) {
  #pragma HLS INTERFACE ap_ctrl_none port=return
  #pragma DATAFLOW
  
  hls::stream<int, 1> multiplyByThreeOutput{};

  multiplyByThree(input, multiplyByThreeOutput);
  keepEvenNumbers(multiplyByThreeOutput, output);
}

#ifndef __SYNTHESIS__

#include "UnitTesting/unitTesting.h"

int main() {
  extendingADataflow_SolutionA(
    UnitTesting::UnitTestableInputStream<int>({0, 1, 2, 3, 4, 5, 6, -1}), 
    UnitTesting::UnitTestableOutputStream<int>({0, 6, 12, 18, -1})
  );
}

#endif

