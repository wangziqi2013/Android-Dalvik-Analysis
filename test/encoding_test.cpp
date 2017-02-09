
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

/*
 * TestGetByte() - Tests whether we could get bytes corectly
 */
void TestGetByte() {
  _PrintTestName();
  
  uint8_t test[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0}; 
  
  uint8_t byte = EncodingUtility::GetLow4Bit(test);
  dbg_printf("0x%lX\n", static_cast<uint64_t>(byte));
  assert(byte == 0x02);
  
  byte = EncodingUtility::GetHigh4Bit(test);
  dbg_printf("0x%lX\n", static_cast<uint64_t>(byte));
  assert(byte = 0x10);
  
  byte = EncodingUtility::Get8Bit(test);
  dbg_printf("0x%lX\n", static_cast<uint64_t>(byte));
  assert(byte == 0x12);
  
  uint16_t word = EncodingUtility::Get16Bit(test);
  dbg_printf("0x%lX\n", static_cast<uint64_t>(word));
  assert(word == 0x3412);
  
  uint32_t dword = EncodingUtility::Get32Bit(test);
  dbg_printf("0x%lX\n", static_cast<uint64_t>(dword));
  assert(dword == 0x78563412);
  
  uint64_t qword = EncodingUtility::Get64Bit(test);
  dbg_printf("0x%lX\n", static_cast<uint64_t>(qword));
  assert(qword == 0xF0DEBC9A78563412UL);
  
  return;
}


int main() {
  TestULEB128();
  TestSLEB128();
  TestGetByte();
  
  return 0; 
}

