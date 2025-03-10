#include <cstdlib>

#include "hls_stream.h"
#include "hls_burst_maxi.h"

void unloader(hls::stream<int>& input, hls::burst_maxi<int> output) {
  #pragma HLS INTERFACE mode=m_axi port=output depth=8192
  
  bool done = false;
  std::size_t burst = 0;
  while (!done) {
    output.write_request(burst * 16, 16);

    for (std::size_t index = 0; index < 16; index++) {
      const int number = !done ? input.read() : -1;
      done |= number < 0;
      output.write(number);
    }

    output.write_response();
    burst++;
  }
}

#ifndef __SYNTHESIS__

#include "UnitTesting/unitTesting.h"

int main() {
  unloader(
    UnitTesting::UnitTestableInputStream<int>({0, 1, 4, 9, 16, 25, 36, -1}), 
    UnitTesting::UnitTestableOutputArray<int, 8192>({0, 1, 4, 9, 16, 25, 36, -1})
  );
}

#endif

