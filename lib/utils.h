//
// Created by newap on 3/27/2020.
//

#ifndef HW_ARCHIVER_LIB_UTILS_H_
#define HW_ARCHIVER_LIB_UTILS_H_

#include <string>
#include <stdexcept>
using namespace std;

static void error(const string &msg) {
  throw runtime_error(msg);
}

static constexpr unsigned int countBits(unsigned int n)
{
  unsigned int count = 0;
  while (n)
  {
    count++;
    n >>= 1;
  }
  return count;
}

static uint64_t combineNumber(uint64_t j, uint64_t k, uint8_t c, const unsigned int& k_size,
                            const unsigned int& c_size) {
  uint64_t res = (j << (k_size + c_size)) | (k << c_size) | c;
  return res;
}

#endif //HW_ARCHIVER_LIB_UTILS_H_
