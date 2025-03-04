#include <cstdlib>

#include "hls_stream.h"
#include "ap_int.h"

void allInOne(
  const int* input, 
  int* output, 
  const std::size_t vsize
) {
  for (std::size_t index = 0; index < vsize; index++) {
    const int number = input[index];

    if (number < 0) {
      output[index] = number; 
    }
    else {
      output[index] = input[index] * input[index]; 
    }
  }
}

#ifndef __SYNTHESIS__
#include "UnitTesting/unitTesting.h"

int main() {
  allInOne(
    UnitTesting::UnitTestableInputArray<int, 8192>({0, 1, 2, 3, 4, 5, 6, -1}), 
    UnitTesting::UnitTestableOutputArray<int, 8192>({0, 1, 4, 9, 16, 25, 36, -1}),
    8
  );

  return 0;
}

#endif

