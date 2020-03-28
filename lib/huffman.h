//
// Created by newap on 3/28/2020.
//

#ifndef HW_ARCHIVER_LIB_HUFFMAN_H_
#define HW_ARCHIVER_LIB_HUFFMAN_H_

#include "archiver.h"

class huffman : public archiver {
 public:
  void compress(const string& inFileName, const string& outFileName) override {
    archiver::compress(inFileName, outFileName);
  }

  void decompress(const string& inFileName, const string& outFileName) override {
    archiver::decompress(inFileName, outFileName);
  }

  void compress(istream &in, ostream &out) override {
    vector<uint8_t> contents = getContents(in);
    map<ext_char, int> freq = getFrequencyTable(contents);

    writeHeader(out, freq);
    Node *tree = buildEncodingTree(freq);

    encode(contents, tree, out);
    freeNodeTree(tree);
  }

  void decompress(istream &in, ostream &out) override {
    map<ext_char, int> freq = readHeader(in);

    Node *tree = buildEncodingTree(freq);

    decode(in, tree, out);
    freeNodeTree(tree);
  }

 private:
  map<ext_char, int> getFrequencyTable(const vector<uint8_t> &contents) {
    map<ext_char, int> result;

    for (const auto &ch: contents)
      result[ch]++;

    result[PSEUDO_EOF] = 1;

    return result;
  }

  Node *buildEncodingTree(const map<ext_char, int> &frequencies) {
    priority_queue<Node *, vector<Node *>, compare> pq;

    for (const auto &freq : frequencies)
      pq.push(new Node(freq.first, freq.second));

    makeNodeTree(pq);

    Node *lastNode = pq.top();
    pq.pop();

    return lastNode;
  }

  void makeNodeTree(priority_queue<Node *, vector<Node *>, compare> &pq) {
    while (pq.size() > 1) {

      Node *right = pq.top();
      pq.pop();

      Node *left = pq.top();
      pq.pop();

      Node *top = new Node(NOT_A_CHAR, right->freq + left->freq);
      top->one = right;
      top->zero = left;

      pq.push(top);
    }
  }

  void freeNodeTree(Node *root) {
    if (root->one)
      freeNodeTree(root->one);

    if (root->zero)
      freeNodeTree(root->zero);

    delete root;
  }

  void writeHeader(ostream &out, map<ext_char, int> &frequencies) {
    if (frequencies.count(PSEUDO_EOF) <= 0) {
      error("No PSEUDO_EOF defined.");
    }

    out << frequencies.size() - 1;

    out << ' ';

    for (const auto &val: frequencies) {
      ext_char ch = val.first;

      if (ch == PSEUDO_EOF) continue;

      uint8_t res = ch;

      out << res << frequencies[ch] << ' ';
    }
  }

  map<ext_char, int> readHeader(istream &in) {
    map<ext_char, int> result;

    int numValues;
    in >> numValues;

    in.get();

    for (int i = 0; i < numValues; i++) {
      char c = in.get();
      ext_char ch = (uint8_t) c;

      int frequency;
      in >> frequency;

      in.get();

      result[ch] = frequency;
    }

    result[PSEUDO_EOF] = 1;
    return result;
  }

  void encode(vector<uint8_t> &contents, Node *tree, ostream &out) {
    map<ext_char, string> encodingMap;
    string code;
    makeEncodingMap(encodingMap, tree, code);

    obitbuf bout;

    for (const uint8_t &ch: contents)
      bout.addBinaryString(encodingMap[ch]);

    bout.addBinaryString(encodingMap[PSEUDO_EOF]);
    bout.writeToStream(out);
  }

  void decode(istream &in, Node *tree, ostream &out) {
    map<string, ext_char> decodingMap;
    string code;
    makeDecodingMap(decodingMap, tree, code);

    ibitbuf bin(in);

    string curr;
    int bit;

    while (true) {
      bit = bin.readBit();
      curr += (bit == 1) ? '1' : '0';

      if (decodingMap.count(curr) > 0) {
        ext_char ch = decodingMap[curr];

        if (ch == PSEUDO_EOF) break;

        out << (char) ch;
        curr = "";
      }
    }
  }

  void makeDecodingMap(map<string, ext_char> &decodingMap, Node *node, const string& code) {
    if (node->one)
      makeDecodingMap(decodingMap, node->one, code + "1");

    if (node->zero)
      makeDecodingMap(decodingMap, node->zero, code + "0");

    if (node->character != NOT_A_CHAR)
      decodingMap[code] = node->character;
  }

  void makeEncodingMap(map<ext_char, string> &encodingMap, Node *node, const string &code) {
    if (node->one)
      makeEncodingMap(encodingMap, node->one, code + "1");

    if (node->zero)
      makeEncodingMap(encodingMap, node->zero, code + "0");

    if (node->character != NOT_A_CHAR)
      encodingMap[node->character] = code;
  }
};

#endif //HW_ARCHIVER_LIB_HUFFMAN_H_
