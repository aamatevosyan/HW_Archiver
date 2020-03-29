//
// Created by newap on 3/27/2020.
//

#ifndef HW_ARCHIVER_LIB_BITBUF_HPP_
#define HW_ARCHIVER_LIB_BITBUF_HPP_

#include <bitset>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Byte's size
 */
static const int BYTE_SIZE = 8;

/**
 * Converts and returns value from binary string
 * @tparam T value type
 * @param str binary string
 * @return value
 */
template<typename T>
T fromBinaryString(const string &str) {
  const int BITS_COUNT = sizeof(T) * BYTE_SIZE;
  bitset<BITS_COUNT> bits(str);
  return bits.to_ulong();
}

/**
 * Converts and returns binary string from value
 * @tparam T value type
 * @param value value
 * @param cl binary string's length
 * @return binary string from value
 */
template<typename T>
string toBinaryString(const T &value, const int &cl) {
  const int BITS_COUNT = sizeof(value) * BYTE_SIZE;
  bitset<BITS_COUNT> bits(value);
  return bits.to_string().substr(BITS_COUNT - cl);
}

/**
 * Abstract calss for bitbif
 */
class bitbuf {
 protected:
  vector<bitset<BYTE_SIZE>> bits;
  int block{0};
  int cell{0};
  string code;
};

/**
 * Class for bit input manipulation
 */
struct ibitbuf : bitbuf {
 public:

  /**
   * Computes and gets the value from the first n bits
   * @tparam T value type
   * @param result value
   * @param size bits count
   * @return the value from the first n bits
   */
  template<typename T>
  bool getData(T &result, int size) {
    T mask = 1;
    result = 0;

    int bit;

    for (size_t i = 0; i < size; i++) {
      bit = readBit();

      if (bit == -1)
        return false;

      if (bit)
        result |= mask;
      mask <<= 1;
    }
    return true;
  }

  /**
 * Computes and gets the value from the first n bits in reverse order
 * @tparam T value type
 * @param result value
 * @param size bits count
 * @return the value from the first n bits in reverse order
 */
  template<typename T>
  bool getDataReverse(T &result, int size) {
    T mask = 1 << (size - 1);
    result = 0;

    int bit;

    for (size_t i = 0; i < size; i++) {
      bit = readBit();

      if (bit == -1)
        return false;

      if (bit)
        result |= mask;
      mask >>= 1;
    }
    return true;
  }

  /**
   * Default constructor
   * @param contents
   */
  explicit ibitbuf(const vector<uint8_t> &contents) {
    loadData(contents);
  }

  /**
   * Constructor
   * @param stream input stream
   */
  explicit ibitbuf(istream &stream) {
    auto begin = istreambuf_iterator<char>(stream);
    auto end = istreambuf_iterator<char>();

    vector<uint8_t> contents((istreambuf_iterator<char>(stream)), istreambuf_iterator<char>());

    loadData(contents);
  }

  /**
   * Loads contents
   * @param contents contents
   */
  void loadData(const vector<uint8_t> &contents) {
    for (const uint8_t &val: contents)
      bits.emplace_back(val);
  }

  /**
   * Reads current bit, if there is no bit available, returns -1
   * @return current bit
   */
  int readBit() {
    if (block == bits.size())
      return -1;

    int value = bits[block][cell];

    if (cell == 7) {
      cell = 0;
      block++;
    } else {
      cell++;
    }

    return value;
  }

};

/**
 * Class for bit output manipulation
 */
class obitbuf : bitbuf {
 public:
  /**
   * Default constructor
   */
  obitbuf() {
    block = -1;
  }

  /**
   * Writes bit to contents
   * @param bit bit
   */
  void writeBit(const bool &bit) {
    if (cell == 0) {
      block++;
      bits.emplace_back();
    }

    bits[block][cell] = bit;
    cell = (cell == 7) ? 0 : cell + 1;
  }

  /**
   * Write bits of value to contents
   * @tparam T value type
   * @param value value
   * @param size count of bits
   */
  template<typename T>
  void writeBits(const T &value, const int &size) {
    string str = toBinaryString(value, size);
    addBinaryString(str, true);
  }

  /**
   * Adds binary string to contents
   * @param str binary string
   * @param isReversed is binary string in reverse order
   */
  void addBinaryString(string &str, const bool &isReversed = true) {
    code += str;
    while (code.length() > BYTE_SIZE) {
      string s = code.substr(0, BYTE_SIZE);

      if (isReversed)
        reverse(s.begin(), s.end());

      bits.emplace_back(s);
      code.erase(0, BYTE_SIZE);
      block++;
    }
    cell = code.length();
  }

  /**
   * Writes all contents to stream
   * @param outFile out stream
   * @param isReversed is binary string in reverse order
   */
  void writeToStream(ostream &outFile, const bool &isReversed = true) {
    if (isReversed)
      reverse(code.begin(), code.end());

    if (!code.empty()) {
      bits.emplace_back(code);
      block++;
    }

    code = "";
    cell = 0;

    for (const bitset<BYTE_SIZE> &val: bits)
      outFile.put((char) val.to_ulong());
  }
};

#endif //HW_ARCHIVER_LIB_BITBUF_HPP_
