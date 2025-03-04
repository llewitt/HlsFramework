#include <cstdlib>

#include "hls_stream.h"

void unloader(hls::stream<int>& input, int* output) {
  std::size_t index = 0;
  while (true) {
    const float number = input.read();

    output[index] = number;
    index++;

    if (number < 0) {
      break;
    }
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

