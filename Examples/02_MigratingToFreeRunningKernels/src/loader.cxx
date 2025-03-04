#include <cstdlib>

#include "hls_stream.h"

void loader(const int* input, hls::stream<int>& output, std::size_t vsize) {
  for (std::size_t index = 0; index < vsize; index++) {
    output.write(input[index]);
  }
}

#ifndef __SYNTHESIS__

#define BOOST_TEST_MODULE loaderTestSuite
#include <boost/test/unit_test.hpp>

#include "UnitTesting/unitTesting.h"

BOOST_AUTO_TEST_CASE(migratingToFreeRunningKernelsTestCase)
{
  loader(
    UnitTesting::UnitTestableInputArray<int, 8192>({0, 1, 2, 3, 4, 5, 6, -1}), 
    UnitTesting::UnitTestableOutputStream<int>({0, 1, 2, 3, 4, 5, 6, -1}),
    8
  );
}

#endif

