#include <cstdlib>

#include "hls_stream.h"

void splitter_Scenario(hls::stream<int>& input, hls::stream<int>& output) {
  #pragma HLS INTERFACE ap_ctrl_none port=return
  
  while (true) {
    const int number = input.read();

    if (number < 0) {
      output.write(number);
      break;
    }

    const int a = 2 * number;
    const int b = 3 * number;
    const int c = a + b;

    output.write(c);
  }
}

#ifndef __SYNTHESIS__

#include "UnitTesting/unitTesting.h"

int main() {
  splitter_Scenario(
    UnitTesting::UnitTestableInputStream<int>({
      0, 1, 2, 3, 4, 5, 6, -1
    }), 
    UnitTesting::UnitTestableOutputStream<int>({
      0, 5, 10, 15, 20, 25, 30, -1
    })
  );
}

#endif

