#ifndef UNIT_TESTING_H
#define UNIT_TESTING_H

#include <cstdlib>
#include <cassert>
#include <vector>
#include <array>

#include "hls_stream.h"

#define assertm(exp, msg) assert((void(msg), exp))

namespace UnitTesting {
template <typename T>
class UnitTestableInputStream {
  hls::stream<T> m_stream;

 public:
  UnitTestableInputStream(const std::vector<T>& vector) {
    for (const T& element : vector) {
      m_stream.write(element);
    } 
  }

  ~UnitTestableInputStream() {
    assertm(
      m_stream.empty(),
      "Input stream is not empty after running function."
    );
  }

  operator hls::stream<T>&() {
    return m_stream;
  }
};

template <typename T>
class UnitTestableOutputStream {
  hls::stream<T> m_stream;
  const std::vector<T>& m_vector;

 public:
  UnitTestableOutputStream(const std::vector<T>& vector) : m_vector(vector) {}

  ~UnitTestableOutputStream() {
    for (const T& element :  m_vector) {
      assertm(
        !m_stream.empty(), 
        "Output stream is empty while checking against reference."
      ); 

      assertm(
        element == m_stream.read(), 
        "Output stream does not match reference."
      ); 
   }

    assertm(
      m_stream.empty(), 
      "Output stream is not empty after checking against reference."
    ); 
  }

  operator hls::stream<T>&() {
    return m_stream;
  }
};

template <typename T, std::size_t MAX_SIZE>
class UnitTestableInputArray {
  const std::array<T, MAX_SIZE>& m_array{};

 public:
  UnitTestableInputArray(const std::array<T, MAX_SIZE>& array) : m_array(array) {}

  operator T*() {
    return m_array.data();
  }

  operator const T*() {
    return m_array.data();
  }
};

template <typename T, std::size_t MAX_SIZE>
class UnitTestableOutputArray {
  const std::vector<T>& m_reference;
  std::array<T, MAX_SIZE> m_array{};

 public:
  UnitTestableOutputArray(const std::vector<T>& reference) : m_reference(reference) {
    assertm(
      m_reference.size() < MAX_SIZE,
      "Reference size is larger than MAX_SIZE of output array."
    ); 
  }

  ~UnitTestableOutputArray() {
    for (std::size_t index = 0; index < m_reference.size(); index++) {
      assertm(
        m_reference[index] == m_array[index],
        "Output array does not match reference array"
      );
    }
  }

  operator T*() {
    return m_array.data();
  }
};
}

#endif

