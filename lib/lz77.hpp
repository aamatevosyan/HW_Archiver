//
// Created by newap on 3/28/2020.
//

#ifndef HW_ARCHIVER_LIB_LZ77_HPP_
#define HW_ARCHIVER_LIB_LZ77_HPP_

#include "types.h"
#include "archiver.hpp"
#include "bitbuf.hpp"
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

template<int S, int T>
struct lz77 : archiver {
 public:
  void compress(const string &inFileName, const string &outFileName) override {
    archiver::compress(inFileName, outFileName);
  }

  void decompress(const string &inFileName, const string &outFileName) override {
    archiver::decompress(inFileName, outFileName);
  }

  void compress(istream &in, ostream &out) override {
    vector<uint8_t> contents = getContents(in);
    compress(contents, out);
  }

  void decompress(istream &in, ostream &out) override {
    vector<uint8_t> contents = getContents(in);
    decompress(contents, out);
  }

 private:
  /**
   * Size for storing Triplet's j
   */
  static constexpr unsigned int J = countBits(S - 1);

  /**
 * Size for storing Triplet's k
 */
  static constexpr unsigned int K = countBits(T);

  /**
 * Size for storing Triplet's c
 */
  static constexpr unsigned int C = BYTE_SIZE;

  /**
   * Gets triplet from bitbuf and returns true if it was successful and false otherwise
   * @param triplet triplet
   * @param bin bitbuf
   * @return returns true if it was successful and false otherwise
   */
  bool getTriplet(Triplet &triplet, ibitbuf &bin) {

    uint8_t c;
    uint64_t k;
    uint64_t j;

    if (!bin.getData(c, C) || !bin.getData(k, K) || !bin.getData(j, J))
      return false;

    triplet.j = j + 1;
    triplet.k = k;
    triplet.c = c;

    return true;
  }

  /**
   * Adds triplet to bitbuf
   * @param triplet tirplet
   * @param bout bitbuf
   */
  void addTriplet(const Triplet &triplet, obitbuf &bout) {
    uint64_t result = combineNumber(triplet.j - 1, triplet.k, triplet.c, K, C);

    bitset<J + K + C> b(result);
    int bites_to_write = J + K + C;

    for (size_t i = 0; i < bites_to_write; i++)
      bout.writeBit(b[i]);
  }

  /**
   * Finds the next triplet from given contents and index
   * @param i index
   * @param contents contents
   * @return next triplet
   */
  Triplet find(int i, const vector<uint8_t> &contents) {
    int start, end;
    start = max(0, i - S);
    end = i - 1;

    int lstart, lend;
    lstart = i;
    lend = min((int) contents.size() - 1, T + i - 1);

    int maxLen = 0, j, fndIndex = 0;
    for (i = start; i <= end; i++) {
      if (contents[lstart] == contents[i]) {
        for (j = 0; j + lstart <= lend && contents[j + i] == contents[j + lstart]; j++);

        if (j > maxLen) {
          fndIndex = end - i + 1;
          maxLen = j;
        }
      }
    }

    if (maxLen == 0)
      fndIndex = 1;

    return Triplet(fndIndex, maxLen, contents[lstart + maxLen]);
  }

  /**
 * Compress contents and writes to output stream
 * @param contents contents
 * @param out output stream
 */
  void compress(const vector<uint8_t> &contents, ostream &out) {
    obitbuf bout;

    for (int i = 0; i < contents.size(); i++) {
      Triplet triplet = find(i, contents);
      addTriplet(triplet, bout);
      i += triplet.k;
    }

    bout.writeToStream(out);
  }

  /**
 * Decompress contents and writes to output stream
 * @param contents contents
 * @param out output stream
 */
  void decompress(const vector<uint8_t> &contents, ostream &out) {
    ibitbuf bin(contents);

    Triplet triplet(0, 0, 0);
    vector<uint8_t> result;

    while (getTriplet(triplet, bin)) {
      if (triplet.k > 0) {
        int start = result.size() - triplet.j;

        for (int j = 0; j < triplet.k; j++)
          result.push_back(result[start + j]);
      }
      result.push_back(triplet.c);
    }

    // checks if byte exists in the last position
    if (triplet.c == 0)
      result.pop_back();

    for (const uint8_t &val: result)
      out.put(val);
  }
};

#endif //HW_ARCHIVER_LIB_LZ77_HPP_
