#include <cstdlib>

#include "hls_stream.h"
#include "hls_vector.h"

void wideFreeRunningKernel(
  hls::stream<hls::vector<float, 16>>& input, 
  hls::stream<hls::vector<float, 16>>& output
) {
  #pragma HLS INTERFACE ap_ctrl_none port=return
  #pragma HLS PIPELINE

  const hls::vector<float, 16> numbers = input.read();
  const hls::vector<float, 16> squaredNumbers{};

  for (const std::size_t index = 0; index < 16; index++) {
    #pragma HLS UNROLL 

    const float number = numbers[index];

    if (numbers[index] < 0) {
      squareNumbers[index] = number;
    }
    else {
      squareNumbers[index] = number * number;
    }
  }
}

#ifndef __SYNTHESIS__

#define BOOST_TEST_MODULE wideFreeRunningKernelTestSuite
#include <vector>

#include <boost/test/unit_test.hpp>

#include "UnitTesting/unitTesting.h"

BOOST_AUTO_TEST_CASE(wideFreeRunningKernelTestCase)
{
  BOOST_CHECK(UnitTesting::streamToStream(
    wideFreeRunningKernel,
    std::vector<hls::vector<float, 16>>{
      0, 1, 2, 3, 4, 5, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1
    },
    std::vector<hls::vector<float, 16>>{
      0, 1, 4, 9, 16, 25, 36, 49, -1, -1, -1, -1, -1, -1, -1, -1
    }
  )); 
}

#endif

