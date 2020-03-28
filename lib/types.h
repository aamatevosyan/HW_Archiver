//
// Created by newap on 3/27/2020.
//

#ifndef HW_ARCHIVER_LIB_TYPES_H_
#define HW_ARCHIVER_LIB_TYPES_H_

#include <cstdint>
/**
 * Custom defined type for storing symbols
 */
typedef int ext_char;

/**
 * PSEUDO EOF symbol which represents the last non existing symbol
 */
const ext_char PSEUDO_EOF = 256;

/**
 * Symbol for combined internal node
 */
const ext_char NOT_A_CHAR = 257;

/**
 * Max char count
 */
const ext_char MAX_CHAR = 256;


/**
 * Node structure
 */
struct Node {

  /**
   * Character
   */
  ext_char character;

  /**
   * Node for '0' code
   */
  Node *zero;

  /**
   * Node for '1' code
   */
  Node *one;

  /**
   * Weight of character
   */
  int freq;

  /**
   * Main constructor
   * @param character character
   * @param weight freq
   */
  Node(ext_char character, int freq) {
    this->character = character;
    this->freq = freq;

    this->zero = this->one = nullptr;
  }
};

/**
 * Structure that defines comparator for Node
 */
struct compare {
  bool operator()(const Node *left, const Node *right) {
    return left->freq > right->freq;
  }
};

struct Triplet {
  uint64_t j;
  uint64_t k;
  uint8_t c;

  Triplet(uint64_t j, uint64_t k, uint8_t c) {
    this->j = j;
    this->k = k;
    this->c = c;
  }
};

#endif //HW_ARCHIVER_LIB_TYPES_H_
