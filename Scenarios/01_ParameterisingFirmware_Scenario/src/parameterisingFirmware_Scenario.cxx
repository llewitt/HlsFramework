#include <cstdlib>

#include "hls_stream.h"

namespace {
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

void parameterisingFirmware_Scenario(hls::stream<int>& input, hls::stream<int>& output) {
  #pragma HLS INTERFACE ap_ctrl_none port=return
  #pragma DATAFLOW
  
  hls::stream<int, 1> internal[15]{};

  sort(input, internal[0]);
  sort(internal[0], internal[1]);
  sort(internal[1], internal[2]);
  sort(internal[2], internal[3]);
  sort(internal[3], internal[4]);
  sort(internal[4], internal[5]);
  sort(internal[5], internal[6]);
  sort(internal[6], internal[7]);
  sort(internal[7], internal[8]);
  sort(internal[8], internal[9]);
  sort(internal[9], internal[10]);
  sort(internal[10], internal[11]);
  sort(internal[11], internal[12]);
  sort(internal[12], internal[13]);
  sort(internal[13], internal[14]);
  sort(internal[14], output);
}

#ifndef __SYNTHESIS__

#include "UnitTesting/unitTesting.h"

int main() {
  parameterisingFirmware_Scenario(
    UnitTesting::UnitTestableInputStream<int>({
      16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 17, -1
    }), 
    UnitTesting::UnitTestableOutputStream<int>({
      17, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1
    })
  );
}

#endif

