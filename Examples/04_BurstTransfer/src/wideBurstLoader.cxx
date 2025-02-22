#include <cstdlib>
#include <numeric>

#include "hls_stream.h"
#include "hls_vector.h"

void wideBurstLoader(
  const hls::vector<float, 16>* input, 
  hls::stream<hls::vector<float, 16>>& output,
  const std::size_t vsize
) {
  for (
    std::size_t index = 0; 
    index < ((vsize / 16) + ((vsize % 16 != 0) ? 1 : 0)); 
    index++
  ) {
    #pragma HLS PIPELINE

    output.write(input[index]);
  }
}

#ifndef __SYNTHESIS__

#define BOOST_TEST_MODULE wideLoaderTestSuite
#include <vector>

#include <boost/test/unit_test.hpp>

#include "UnitTesting/unitTesting.h"

BOOST_AUTO_TEST_CASE(wideLoaderTestCase)
{
  BOOST_CHECK(UnitTesting::memoryMapWithVsizeToStream(
    wideBurstLoader,
    std::vector<hls::vector<float,16>>{
      {0, 1, 2, 3, 4, 5, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1}
    },
    std::vector<hls::vector<float,16>>{
      {0, 1, 2, 3, 4, 5, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1}
    },
    16
  )); 
}

#endif

