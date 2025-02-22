#include <cstdlib>
#include <iostream>

#include "hls_stream.h"

void unloader(hls::stream<float>& input, float* output) {
  std::size_t index = 0;
  while (true) {
    const float number = input.read();

    output[index] = number;

    if (*reinterpret_cast<const int*>(&number) & (1 << 31)) {
      break;
    }

    index++;
  }
}

#ifndef __SYNTHESIS__

#define BOOST_TEST_MODULE unloaderTestSuite
#include <vector>

#include <boost/test/unit_test.hpp>

#include "UnitTesting/unitTesting.h"

BOOST_AUTO_TEST_CASE(unloaderTestCase)
{
  BOOST_CHECK(UnitTesting::streamToMemoryMap(
    unloader,
    std::vector<float>{0, 1, 2, 3, 4, 5, 6, 7, -1},
    std::vector<float>{0, 1, 2, 3, 4, 5, 6, 7, -1}
  )); 
}

#endif

