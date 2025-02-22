#include <cstdlib>

#include "hls_stream.h"

void loader(const float* input, hls::stream<float>& output) {
  std::size_t index = 0;
  while (true) {
    const float number = input[index];

    output.write(number);

    if (*reinterpret_cast<const int*>(&number) & (1 << 31)) {
      break;
    }

    index++;
  }
}

#ifndef __SYNTHESIS__

#define BOOST_TEST_MODULE loaderTestSuite
#include <vector>

#include <boost/test/unit_test.hpp>

#include "UnitTesting/unitTesting.h"

BOOST_AUTO_TEST_CASE(loaderTestCase)
{
  BOOST_CHECK(UnitTesting::memoryMapToStream(
    loader,
    std::vector<float>{0, 1, 2, 3, 4, 5, 6, 7, -1},
    std::vector<float>{0, 1, 2, 3, 4, 5, 6, 7, -1}
  )); 
}

#endif



