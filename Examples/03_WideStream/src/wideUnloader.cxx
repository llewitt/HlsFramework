#include <cstdlib>
#include <numeric>

#include "hls_stream.h"
#include "hls_vector.h"

void wideUnloader(
  hls::stream<hls::vector<float, 16>>& input, 
  hls::vector<float, 16>* output
) {
  std::size_t index = 0;
  while (true) {
    const hls::vector<float, 16> numbers = input.read();

    output[index] = numbers;

    const bool foundSentinel = std::accumulate(
      numbers.begin(), 
      numbers.end(), 
      false, 
      [](const auto a, const auto b){ 
        return a || (*reinterpret_cast<const int*>(&b) & (1 << 31)); 
      }
    );

    if (foundSentinel) {
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
    wideUnloader,
    std::vector<hls::vector<float, 16>>{
      {0, 1, 2, 3, 4, 5, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1}
    },
    std::vector<hls::vector<float, 16>>{
      {0, 1, 2, 3, 4, 5, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1}
    }
  )); 
}

#endif

