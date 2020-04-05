//
// Created by newap on 3/27/2020.
//

#ifndef HW_ARCHIVER_LIB_ARCHIVER_H_
#define HW_ARCHIVER_LIB_ARCHIVER_H_

#include "types.h"
#include "bitbuf.hpp"
#include "utils.h"
#include <map>
#include <queue>
#include <sstream>
#include <cmath>

/**
 * Abstract class for archivers
 */
class archiver {
 public:
  /**
   * Compares files and returns if they had matched
   * @param oneContents first file's contents
   * @param twoContents second file's contents
   * @return true if they match and false otherwise
   */
  bool compareFiles(const vector<uint8_t> &oneContents, const vector<uint8_t> &twoContents) {
    if (oneContents.size() != twoContents.size())
      return false;

    for (int i = 0; i < oneContents.size(); i++)
      if (oneContents[i] != twoContents[i])
        return false;

    return true;
  }

  /**
   * Gets and returns contents of the file
   * @param filename file name
   * @return contents of the file
   */
  vector<uint8_t> getContents(const string &filename) {
    ifstream fin(filename, ios::in | ios::binary);
    vector<uint8_t> contents = getContents(fin);
    fin.close();

    return contents;
  }

  /**
   * Gets and returns contents from the stream
   * @param in stream
   * @return contents from the stream
   */
  vector<uint8_t> getContents(istream &in) {
    auto begin = istreambuf_iterator<char>(in);
    auto end = istreambuf_iterator<char>();

    vector<uint8_t> contents(begin, end);
    return contents;
  }

  /**
   * Counts and returns the frequencies list from contents
   * @param contents contents
   * @return the frequencies list from contents
   */
  vector<int> getFrequencies(const vector<uint8_t> &contents) {
    vector<int> freqs(MAX_CHAR, 0);

    for (const uint8_t &ch: contents)
      freqs[ch]++;

    return freqs;
  }

  /**
   * Counts and returns the file size
   * @param filename filename
   * @return the file size
   */
  size_t getSize(const string &filename) {
    vector<uint8_t> contents = getContents(filename);

    size_t size = contents.size();
    return size;
  }

  /**
   * Counts and gets the file size, frequencies list and entropy from file
   * @param filename filename
   * @param size size
   * @param freqs frequencies list
   * @param entropy entropy
   */
  void getDetails(const string &filename, size_t &size, vector<int> &freqs, double &entropy) {
    vector<uint8_t> contents = getContents(filename);

    size = contents.size();

    freqs = getFrequencies(contents);

    entropy = 0;

    for (int i = 0; i < MAX_CHAR; i++) {
      if (freqs[i] != 0) {
          double p_x = (double) freqs[i] / size;
          entropy -= p_x * log(p_x) / log(256);
      }
    }
  }

  /**
   * Compress file
   * @param inFileName file to compress
   * @param outFileName compressed file
   */
  virtual void compress(const string &inFileName, const string &outFileName) {
    ifstream infile(inFileName, ios::in | ios::binary);
    ofstream outfile(outFileName, ios::out | ios::binary);

    compress(infile, outfile);

    infile.close();
    outfile.close();
  }

  /**
   * Decompress file
   * @param inFileName file to decompress
   * @param outFileName decompressed files
   */
  virtual void decompress(const string &inFileName, const string &outFileName) {
    ifstream infile(inFileName, ios::in | ios::binary);
    ofstream outfile(outFileName, ios::out | ios::binary);

    decompress(infile, outfile);

    infile.close();
    outfile.close();
  }

  /**
   * Compress stream
   * @param in stream to compress
   * @param out compressed stream
   */
  virtual void compress(istream &in, ostream &out) = 0;

  /**
   * Decompress stream
   * @param in stream to decompress
   * @param out decompressed stream
   */
  virtual void decompress(istream &in, ostream &out) = 0;
};

#endif //HW_ARCHIVER_LIB_ARCHIVER_H_
