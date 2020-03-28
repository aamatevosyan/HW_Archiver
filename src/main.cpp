#include <iostream>
#include "../lib/archiver.h"
#include "../lib/huffman.h"
#include "../lib/lz77.h"
#include "../lib/lzw.h"
using namespace std;

int main() {
  archiver* c;
//  c = new huffman();
  c = new lz77<16 * 1024, 4 * 1024>();
  //c = new lzw(15);
////  huffman c;
////  lz77<4096, 1024> c;
//  lzw c(15);

  cout << sizeof(unsigned long) << endl;

//  uint64_t res = combineNumber(2303, 2, 231, 13, 8);
//  cout << res << endl;
  //cout << toBinaryString(res, 35) << endl;

  c->compress("7.bmp", "7.bmp.lzw");
  c->decompress("7.bmp.lzw", "7M.bmp");
  bool res = c->compareFiles(c->getContents("7.bmp"), c->getContents("7M.bmp"));
  cout << res << endl;

  delete c;

  return 0;
}
