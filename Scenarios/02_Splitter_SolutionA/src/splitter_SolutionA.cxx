#include <cstdlib>

#include "hls_stream.h"

namespace {
  void split(
    hls::stream<int>& input, 
    hls::stream<int>& outputA, 
    hls::stream<int>& outputB
  ) {
    while (true) {
      const int number = input.read();

      outputA.write(number);
      outputB.write(number);

      if (number < 0) {
        break;
      }
    }
  }

  void multiplyByTwo(hls::stream<int>& input, hls::stream<int>& output) {
    while (true) {
      const int number = input.read();

      if (number < 0) {
        output.write(number);
        break;
      }

      output.write(2 * number);
    }
  }

  void multiplyByThree(hls::stream<int>& input, hls::stream<int>& output) {
    while (true) {
      const int number = input.read();

      if (number < 0) {
        output.write(number);
        break;
      }

      output.write(3 * number);
    }
  }

  void sum(
    hls::stream<int>& inputA,
    hls::stream<int>& inputB,
    hls::stream<int>& output
  ) {
    while (true) {
      const int numberA = inputA.read();
      const int numberB = inputB.read();

      if (numberA < 0) {
        output.write(numberA);
        break;
      }

      else if (numberB < 0) {
        output.write(numberB);
        break;
      }

      output.write(numberA + numberB);
    }
  }
}

void splitter_SolutionA(hls::stream<int>& input, hls::stream<int>& output) {
  #pragma HLS INTERFACE ap_ctrl_none port=return
  #pragma HLS DATAFLOW

  hls::stream<int> inputA{};
  hls::stream<int> inputB{};

  hls::stream<int> outputA{};
  hls::stream<int> outputB{};
  
  split(input, inputA, inputB);
  multiplyByTwo(inputA, outputA);
  multiplyByThree(inputB, outputB);
  sum(outputA, outputB, output);
}

#ifndef __SYNTHESIS__

#include "UnitTesting/unitTesting.h"

int main() {
  splitter_SolutionA(
    UnitTesting::UnitTestableInputStream<int>({
      0, 1, 2, 3, 4, 5, 6, -1
    }), 
    UnitTesting::UnitTestableOutputStream<int>({
      0, 5, 10, 15, 20, 25, 30, -1
    })
  );
}

#endif

