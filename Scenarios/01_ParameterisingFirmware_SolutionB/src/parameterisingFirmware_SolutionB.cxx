#include <cstdlib>
#include <type_traits>

#include "hls_stream.h"
#include "hls_print.h"

namespace {
  static constexpr std::size_t numberOfSorters = 128;

  void sort(hls::stream<int>& input, hls::stream<int>& output) {
    #pragma HLS INLINE off

    int buffer = -1;

    while (true) {
      const int number = input.read();

      if (buffer < 0 && number < 0) {
        buffer = number;
        break;
      }
      if (buffer < 0) {
        buffer = number;
      }
      else if (number < 0) {
        output.write(buffer);
        buffer = number;
        break;
      }
      else if (number >= buffer){
        output.write(number); 
      }
      else {
        output.write(buffer); 
        buffer = number; 
      }
    }
      
    output.write(buffer);
  }

  template <std::size_t sortCounter>
  void sortDispatch(hls::stream<int>& input, hls::stream<int>& output);

  template <std::size_t sortCounter>
  void sortImpl(std::true_type, hls::stream<int>& input, hls::stream<int>& output) {
    #pragma HLS INLINE

    while (true) {
      const int number = input.read();

      output.write(number);
      
      if (number < 0) {
        break;
      } 
    }
  };

  template <std::size_t sortCounter>
  void sortImpl(std::false_type, hls::stream<int>& input, hls::stream<int>& output) {
    #pragma HLS INLINE

    hls::stream<int> internal{};
    sortDispatch<sortCounter - 1>(input, internal);

    sort(internal, output);
  };

  template <std::size_t sortCounter>
  void sortDispatch(hls::stream<int>& input, hls::stream<int>& output) {
    #pragma HLS INLINE

    sortImpl<sortCounter>(
      std::integral_constant<bool, sortCounter == 0>(), 
      input, 
      output
    );
  }
};

void parameterisingFirmware_SolutionB(hls::stream<int>& input, hls::stream<int>& output) {
  #pragma HLS INTERFACE ap_ctrl_none port=return
  #pragma HLS DATAFLOW

  sortDispatch<numberOfSorters>(input, output);
}

#ifndef __SYNTHESIS__

#include "UnitTesting/unitTesting.h"

int main() {
  parameterisingFirmware_SolutionB(
    UnitTesting::UnitTestableInputStream<int>({
      16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 17, -1
    }), 
    UnitTesting::UnitTestableOutputStream<int>({
      17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, -1
    })
  );
}

#endif

