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
};

void extendingADataflow_Scenario(hls::stream<int>& input, hls::stream<int>& output) {
  #pragma HLS INTERFACE ap_ctrl_none port=return
  #pragma DATAFLOW
  
  multiplyByThree(input, output);
}

#ifndef __SYNTHESIS__

#include "UnitTesting/unitTesting.h"

int main() {
  extendingADataflow_Scenario(
    UnitTesting::UnitTestableInputStream<int>({0, 1, 2, 3, 4, 5, 6, -1}), 
    UnitTesting::UnitTestableOutputStream<int>({0, 6, 12, 18, -1})
  );
}

#endif

