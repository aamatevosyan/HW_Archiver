//
// Created by newap on 3/27/2020.
//

#ifndef HW_ARCHIVER_LIB_UTILS_H_
#define HW_ARCHIVER_LIB_UTILS_H_

#include <string>
#include <stdexcept>
using namespace std;

/**
 * Throws exception with message
 * @param msg message
 */
static void error(const string &msg) {
  throw runtime_error(msg);
}

/**
 * Counts and returns the bits in number
 * @param n number
 * @return count of bits
 */
static constexpr unsigned int countBits(unsigned int n)
{
  unsigned int count = 0;
  while (n) {
    count++;
    n >>= 1;
  }
  return count;
}

/**
 * Combines three numbers and returns encoded number from them
 * @param j first number
 * @param k second number
 * @param c third number
 * @param k_size second number bit's count
 * @param c_size third numbers bit's count
 * @return encoded number
 */
static uint64_t combineNumber(uint64_t j, uint64_t k, uint8_t c, const unsigned int& k_size,
                              const unsigned int& c_size) {
  uint64_t res = (j << (k_size + c_size)) | (k << c_size) | c;
  return res;
}

#endif //HW_ARCHIVER_LIB_UTILS_H_
