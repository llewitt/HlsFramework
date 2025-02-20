#include <cstdlib>

#include "hls_stream.h"

void freeRunningKernel(hls::stream<float>& input, hls::stream<float>& output) {
  #pragma HLS INTERFACE ap_ctrl_none port=return
  #pragma HLS PIPELINE

  const float number = input.read();

  if (number < 0) {
    output.write(number);
  }
  else {
    output.write(number * number);
  }
}

#ifndef __SYNTHESIS__

#define BOOST_TEST_MODULE freeRunningKernelTestSuite
#include <vector>

#include <boost/test/unit_test.hpp>

#include "UnitTesting/unitTesting.h"

BOOST_AUTO_TEST_CASE(freeRunningKernelTestCase)
{
  BOOST_CHECK(UnitTesting::streamToStream(
    freeRunningKernel,
    std::vector<float>{0},
    std::vector<float>{0}
  )); 

  BOOST_CHECK(UnitTesting::streamToStream(
    freeRunningKernel,
    std::vector<float>{1},
    std::vector<float>{1}
  )); 

  BOOST_CHECK(UnitTesting::streamToStream(
    freeRunningKernel,
    std::vector<float>{2},
    std::vector<float>{4}
  )); 

  BOOST_CHECK(UnitTesting::streamToStream(
    freeRunningKernel,
    std::vector<float>{3},
    std::vector<float>{9}
  )); 

  BOOST_CHECK(UnitTesting::streamToStream(
    freeRunningKernel,
    std::vector<float>{4},
    std::vector<float>{16}
  )); 

  BOOST_CHECK(UnitTesting::streamToStream(
    freeRunningKernel,
    std::vector<float>{5},
    std::vector<float>{25}
  )); 

  BOOST_CHECK(UnitTesting::streamToStream(
    freeRunningKernel,
    std::vector<float>{6},
    std::vector<float>{36}
  )); 

  BOOST_CHECK(UnitTesting::streamToStream(
    freeRunningKernel,
    std::vector<float>{7},
    std::vector<float>{49}
  )); 

  BOOST_CHECK(UnitTesting::streamToStream(
    freeRunningKernel,
    std::vector<float>{-1},
    std::vector<float>{-1}
  )); 
}

#endif

