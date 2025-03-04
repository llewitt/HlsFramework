#include <cstdlib>

#include "hls_stream.h"

void freeRunningKernel(hls::stream<int>& input, hls::stream<int>& output) {
  #pragma HLS INTERFACE ap_ctrl_none port=return
  #pragma HLS PIPELINE style=flp

  const int number = input.read();

  if (number < 0) {
    output.write(number);
  }
  else {
    output.write(number * number); 
  }
}

#ifndef __SYNTHESIS__

#include "UnitTesting/unitTesting.h"

int main() {
  freeRunningKernel(
    UnitTesting::UnitTestableInputStream<int>({0}), 
    UnitTesting::UnitTestableOutputStream<int>({0})
  );

  freeRunningKernel(
    UnitTesting::UnitTestableInputStream<int>({1}), 
    UnitTesting::UnitTestableOutputStream<int>({1})
  );

  freeRunningKernel(
    UnitTesting::UnitTestableInputStream<int>({2}), 
    UnitTesting::UnitTestableOutputStream<int>({4})
  );

  freeRunningKernel(
    UnitTesting::UnitTestableInputStream<int>({3}), 
    UnitTesting::UnitTestableOutputStream<int>({9})
  );

  freeRunningKernel(
    UnitTesting::UnitTestableInputStream<int>({4}), 
    UnitTesting::UnitTestableOutputStream<int>({16})
  );

  freeRunningKernel(
    UnitTesting::UnitTestableInputStream<int>({5}), 
    UnitTesting::UnitTestableOutputStream<int>({25})
  );

  freeRunningKernel(
    UnitTesting::UnitTestableInputStream<int>({6}), 
    UnitTesting::UnitTestableOutputStream<int>({36})
  );

  freeRunningKernel(
    UnitTesting::UnitTestableInputStream<int>({-1}), 
    UnitTesting::UnitTestableOutputStream<int>({-1})
  );
}

#endif

