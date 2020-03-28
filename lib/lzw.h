//
// Created by newap on 3/28/2020.
//

#ifndef HW_ARCHIVER_LIB_LZW_H_
#define HW_ARCHIVER_LIB_LZW_H_

#include "archiver.h"
#include "types.h"

class lzw : public archiver {
 public:
  explicit lzw(const int& wordLength) {
    _wordLength = wordLength;
  }

  void compress(const string& inFileName, const string& outFileName) override {
    archiver::compress(inFileName, outFileName);
  }

  void decompress(const string& inFileName, const string& outFileName) override {
    archiver::decompress(inFileName, outFileName);
  }

  void compress(istream& in, ostream& out) override {
    vector<uint8_t> contents = getContents(in);
    compress(contents, out);
  }

  void decompress(istream& in, ostream& out) override {
    vector<uint8_t> contents = getContents(in);
    decompress(contents, out);
  }

 private:
  int _wordLength;

  void compress(const vector<uint8_t> &contents, ostream &out) {
    int MAX_SIZE = 1 << (_wordLength - 1);

    unordered_map<string, unsigned int> dict(MAX_SIZE);
    for (int i = 0; i < MAX_CHAR; i++)
      dict[string(1, i)] = i;

    obitbuf bout;

    string curr;
    unsigned int ind = MAX_CHAR + 1;

    for (const uint8_t& c: contents) {
      curr += c;
      if (dict.find(curr) == dict.end()) {
        if (ind <= MAX_SIZE)
          dict[curr] = ind++;

        curr.erase(curr.size() - 1);
        bout.writeBits(dict[curr], _wordLength);

        curr = c;
      }
    }

    if (!curr.empty())
      bout.writeBits(dict[curr], _wordLength);

    bout.writeToStream(out);
  }

  void decompress(const vector<uint8_t> &contents, ostream &out) {
    int MAX_SIZE = 1 << (_wordLength - 1);

    unordered_map<unsigned int, string> dict(MAX_SIZE);
    for (int i = 0; i < MAX_CHAR; i++)
      dict[i] = string(1, i);

    ibitbuf bin(contents);

    unsigned int code;
    unsigned int ind = MAX_CHAR + 1;

    string curr;

    while (bin.getDataReverse(code, _wordLength)) {
      if (dict.find(code) == dict.end())
        dict[code] = curr + curr[0];

      out << dict[code];

      if (!curr.empty() && ind <= MAX_SIZE)
        dict[ind++] = curr + dict[code][0];

      curr = dict[code];
    }
  }
};

#endif //HW_ARCHIVER_LIB_LZW_H_
