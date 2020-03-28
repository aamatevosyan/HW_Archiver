//
// Created by newap on 3/27/2020.
//

#ifndef HW_ARCHIVER_LIB_BITBUF_H_
#define HW_ARCHIVER_LIB_BITBUF_H_

#include <bitset>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

static const int BYTE_SIZE = 8;

template<typename T>
T fromBinaryString(const string &str) {
  const int BITS_COUNT = sizeof(T) * BYTE_SIZE;
  bitset<BITS_COUNT> bits(str);
  return bits.to_ulong();
}

template<typename T>
string toBinaryString(const T &value, const int &cl) {
  const int BITS_COUNT = sizeof(value) * BYTE_SIZE;
  bitset<BITS_COUNT> bits(value);
  return bits.to_string().substr(BITS_COUNT - cl);
}

class bitbuf {
 protected:
  vector<bitset<BYTE_SIZE>> bits;
  int block{0};
  int cell{0};
  string code;
};

struct ibitbuf : bitbuf {
 public:
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

  explicit ibitbuf(const vector<uint8_t> &contents) {
    loadData(contents);
  }

  explicit ibitbuf(istream &stream) {
    auto begin = istreambuf_iterator<char>(stream);
    auto end = istreambuf_iterator<char>();

    vector<uint8_t> contents((istreambuf_iterator<char>(stream)), istreambuf_iterator<char>());

    loadData(contents);
  }

  void loadData(const vector<uint8_t> &contents) {
    for (const uint8_t &val: contents)
      bits.emplace_back(val);
  }

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

class obitbuf : bitbuf {
 public:
  obitbuf() {
    block = -1;
  }

  void writeBit(const bool &bit) {
    if (cell == 0) {
      block++;
      bits.emplace_back();
    }

    bits[block][cell] = bit;
    cell = (cell == 7) ? 0 : cell + 1;
  }

  template<typename T>
  void writeBits(const T &value, const int &size) {
    string str = toBinaryString(value, size);
    addBinaryString(str, true);
  }

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

#endif //HW_ARCHIVER_LIB_BITBUF_H_
