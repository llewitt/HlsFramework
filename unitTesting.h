namespace UnitTesting {
template <typename F, typename T>
bool streamToStream(
  const F& function,
  const std::vector<T>& inputWords, 
  const std::vector<T>& referenceWords
) {
  hls::stream<T> input{};
  hls::stream<T> output{};
  
  for (const T& words : inputWords) {
    input.write(words);
  }
  
  function(input, output);
  
  for (const T& referenceWord : referenceWords) {
    if (output.empty()) {
      return false; 
    }

    if (output.read() != referenceWord) {
      return false;
    }
  }

  if (!input.empty()) {
    return false; 
  }

  if (!output.empty()) {
    return false; 
  }

  return true;
}

template <typename F, typename T, typename U>
bool memoryMapToStream(
  const F& function,
  const std::vector<T>& inputWords, 
  const std::vector<U>& referenceWords
) {
  std::array<T, Variant::maxDataLength> input{};
  hls::stream<U> output{};

  if (inputWords.size() > input.size()) {
    return false;
  }

  std::size_t index = 0;
  for (const T& words : inputWords) {
    input[index] = words;
    index++;
  }
  
  function(input.data(), output);
  
  for (std::size_t index = 0; index < referenceWords.size(); index++) {
    if (output.empty()) {
      return false;
    }

    if (output.read() != referenceWords[index]) {
      return false;
    }
  }

  if (!output.empty()) {
    return false;
  }

  return true;
}

template <typename F, typename T, typename U>
bool streamToMemoryMap(
  const F& function,
  const std::vector<T>& inputWords, 
  const std::vector<U>& referenceWords
) {
  hls::stream<T> input{};
  std::array<U, Variant::maxDataLength> output{};

  if (referenceWords.size() > output.size()) {
    return false;
  }

  for (const T& words : inputWords) {
    input.write(words);
  }
  
  function(input, output.data());
  
  for (std::size_t index = 0; index < referenceWords.size(); index++) {
    if (output[index] != referenceWords[index]) {
        return false;
    }
  }

  if (!input.empty()) {
    return 
  }

  return true;
}
}

