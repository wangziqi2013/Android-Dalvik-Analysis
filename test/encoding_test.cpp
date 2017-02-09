
/*
 * encoding_test.cpp - Tests encoding schemes
 */

#include "common.h"
#include "test_suite.h"

using namespace wangziqi2013;
using namespace android_dalvik_analysis;

/*
 * TestULEB128() - Tests whether ULEB128 works. We use the slower version as
 *                 a reference solution
 */
void TestULEB128() {
  _PrintTestName();
  
  uint8_t test1[] = {0x80, 0x7F};
  uint8_t *p1 = test1;
  uint8_t *p2 = test1;
  
  uint32_t value1 = EncodingUtility::ReadULEB128(&p1);
  uint32_t value2 = EncodingUtility::ReadLEB128(&p2, false);
  dbg_printf("v1 = %u; v2 = %u\n", value1, value2);
  assert(value1 == value2);
  assert(p1 == p2);
  
  uint8_t test2[] = {0x88, 0x97, 0x7F};
  
  p1 = p2 = test2;
  value1 = EncodingUtility::ReadULEB128(&p1);
  value2 = EncodingUtility::ReadLEB128(&p2, false);
  dbg_printf("v1 = %u; v2 = %u\n", value1, value2);
  assert(value1 == value2);
  assert(p1 == p2);
  
  uint8_t test3[] = {0x88, 0x97, 0xAF, 0x03};
  
  p1 = p2 = test3;
  value1 = EncodingUtility::ReadULEB128(&p1);
  value2 = EncodingUtility::ReadLEB128(&p2, false);
  dbg_printf("v1 = %u; v2 = %u\n", value1, value2);
  assert(value1 == value2);
  assert(p1 == p2);
  
  uint8_t test4[] = {0x88, 0x97, 0xAF, 0xF3, 0x01};
  
  p1 = p2 = test4;
  value1 = EncodingUtility::ReadULEB128(&p1);
  value2 = EncodingUtility::ReadLEB128(&p2, false);
  dbg_printf("v1 = %u; v2 = %u\n", value1, value2);
  assert(value1 == value2);
  assert(p1 == p2);
  
  return;
}

/*
 * TestSLEB128() - Tests whether SLEB128 works. We use the slower version as
 *                 a reference solution
 */
void TestSLEB128() {
  _PrintTestName();
  
  uint8_t test0[] = {0x7F};
  uint8_t *p1 = test0;
  uint8_t *p2 = test0;
  
  int32_t value1 = EncodingUtility::ReadSLEB128(&p1);
  int32_t value2 = EncodingUtility::ReadLEB128(&p2, true);
  dbg_printf("v1 = %d; v2 = %d\n", value1, value2);
  assert(value1 == value2);
  assert(p1 == p2);
  
  uint8_t test1[] = {0x80, 0x7F};
  p1 = p2 = test1;
  
  value1 = EncodingUtility::ReadSLEB128(&p1);
  value2 = EncodingUtility::ReadLEB128(&p2, true);
  dbg_printf("v1 = %d; v2 = %d\n", value1, value2);
  assert(value1 == value2);
  assert(p1 == p2);
  
  uint8_t test2[] = {0x88, 0x97, 0x7F};
  
  p1 = p2 = test2;
  value1 = EncodingUtility::ReadSLEB128(&p1);
  value2 = EncodingUtility::ReadLEB128(&p2, true);
  dbg_printf("v1 = %d; v2 = %d\n", value1, value2);
  assert(value1 == value2);
  assert(p1 == p2);
  
  uint8_t test3[] = {0x88, 0x97, 0xAF, 0x43};
  
  p1 = p2 = test3;
  value1 = EncodingUtility::ReadSLEB128(&p1);
  value2 = EncodingUtility::ReadLEB128(&p2, true);
  dbg_printf("v1 = %d; v2 = %d\n", value1, value2);
  assert(value1 == value2);
  assert(p1 == p2);
  
  uint8_t test4[] = {0x88, 0x97, 0xAF, 0xF3, 0x41};
  
  p1 = p2 = test4;
  value1 = EncodingUtility::ReadSLEB128(&p1);
  value2 = EncodingUtility::ReadLEB128(&p2, true);
  dbg_printf("v1 = %d; v2 = %d\n", value1, value2);
  dbg_printf("*** For 5 byte sequences SLEB128 is not clearly defined\n");
  //assert(value1 == value2);
  //assert(p1 == p2);
  
  return;
}


int main() {
  TestULEB128();
  TestSLEB128();
  return 0; 
}

