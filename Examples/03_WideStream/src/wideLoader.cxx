#include <cstdlib>

#include "hls_stream.h"
#include "hls_vector.h"

void wideLoader(
  const hls::vector<float, 16>* input, 
  hls::stream<hls::vector<float, 16>>& output
) {
  std::size_t index = 0;
  while (true) {
    const hls::vector<float, 16> numbers = input[index];
    const hls::vector<bool, 16> numbersLessThanZero = numbers < 0;

    output.write(numbers);

    if (numbersLessThanZero.or_reduce()) {
      break;
    }

    index++;
  }
}

#ifndef __SYNTHESIS__

#define BOOST_TEST_MODULE wideLoaderTestSuite
#include <vector>

#include <boost/test/unit_test.hpp>

#include "UnitTesting/unitTesting.h"

BOOST_AUTO_TEST_CASE(wideLoaderTestCase)
{
  BOOST_CHECK(UnitTesting::memoryMapToStream(
    wideLoader,
    std::vector<hls::vector<float, 16>>{
      {0, 1, 2, 3, 4, 5, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1}
    },
    std::vector<hls::vector<float, 16>>{
      {0, 1, 2, 3, 4, 5, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1}
    }
  )); 
}

#endif



