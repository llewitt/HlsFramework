#include <cstdlib>
#include <iostream>
#include <numeric>

#include "hls_stream.h"
#include "hls_vector.h"

void wideBurstUnloader(
  hls::stream<hls::vector<float, 16>>& input, 
  hls::vector<float, 16>* output
) {
  bool foundSentinel = false;
  std::size_t burstIndex = 0;
  while (true) {
    #pragma HLS PIPELINE off

    for (std::size_t index = 0; index < 16; index++) {
      #pragma HLS PIPELINE

      hls::vector<float, 16> numbers{};
      
      if (!foundSentinel) {
        numbers = input.read();
      }

      foundSentinel |= std::accumulate(
        numbers.begin(), 
        numbers.end(), 
        false, 
        [](const auto a, const auto b){ 
          return a || (*reinterpret_cast<const int*>(&b) & (1 << 31)); 
        }
      );

      //hls::vector<bool, 16> numbersLessThanZero{};
      //for (std::size_t numberIndex = 0; numberIndex < 16; numberIndex++) {
      //  #pragma HLS UNROLL

      //  if (*reinterpret_cast<const int*>(&numbers[numberIndex]) & (1 << 31)) {
      //    numbersLessThanZero[numberIndex] = true;
      //  }
      //  else {
      //    numbersLessThanZero[numberIndex] = false;
      //  }
      //}

      //foundSentinel |= numbersLessThanZero.reduce_or();

      output[(burstIndex * 16) + index] = numbers;
    }

    if (foundSentinel) {
      break;
    }

    burstIndex++;
  }
}

#ifndef __SYNTHESIS__

#define BOOST_TEST_MODULE wideBurstUnloaderTestSuite
#include <vector>

#include <boost/test/unit_test.hpp>

#include "UnitTesting/unitTesting.h"

BOOST_AUTO_TEST_CASE(wideBurstUnloaderTestCase)
{
  BOOST_CHECK(UnitTesting::streamToMemoryMap(
    wideBurstUnloader,
    std::vector<hls::vector<float, 16>>{
      {0,, 2, 3, 4, 5, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1}
    },
    std::vector<hls::vector<float, 16>>{
      {0,, 2, 3, 4, 5, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1}
    }
  )); 
}

#endif

