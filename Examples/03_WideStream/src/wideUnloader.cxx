#include <cstdlib>
#include <iostream>

#include "hls_stream.h"
#include "hls_vector.h"

void widerUnloader(
  hls::stream<hls::vector<float, 16>>& input, 
  hls::vector<float, 16>* output
) {
  std::size_t index = 0;
  while (true) {
    const hls::vector<float, 16> numbers = input.read();
    const hls::vector<bool, 16> numbersLessThanZero = numbers < 0;

    output[index] = number;

    if (numbersLessThanZero.or_reduce()) {
      break;
    }

    index++;
  }
}

#ifndef __SYNTHESIS__

#define BOOST_TEST_MODULE widerUnloaderTestSuite
#include <vector>

#include <boost/test/unit_test.hpp>

#include "UnitTesting/unitTesting.h"

BOOST_AUTO_TEST_CASE(widerUnloaderTestCase)
{
  BOOST_CHECK(UnitTesting::streamToMemoryMap(
    widerUnloader,
    std::vector<hls::vector<float, 16>>{
      {0, 1, 2, 3, 4, 5, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1}
    },
    std::vector<hls::vector<float, 16>>{
      {0, 1, 2, 3, 4, 5, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1}
    }
  )); 
}

#endif

