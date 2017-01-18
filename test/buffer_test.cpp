
/*
 * buffer_test.cpp - Test class Buffer
 */

// To make class Buffer allocates smaller stack space for Printf()
#define BUFFER_TEST_MODE 

#include "test_suite.h"
#include "common.h" 

using namespace wangziqi2013;
using namespace android_dalvik_analysis;

static const char *test_file_name = "buffer_test.temp";

/*
 * TestBasicExtend() - Tests whether exted works
 */
void TestBasicExtend() {
  _PrintTestName();
  
  // Declear a buffer of size 1
  Buffer buffer{1};
  assert(buffer.GetLength() == 0UL);
  
  // Write 1 byte data into it - this does not cause extend
  buffer.AppendByte('a');
  assert(buffer.GetLength() == 1UL);
  
  // Append data and '\0' to the buffer
  const char *data = "0123456789abcdefghijklmnopqrstuvwxyz";
  size_t data_len = strlen(data);
  
  buffer.Append(data, data_len + 1);
  
  // The length must be data len + 'a' + '\0'
  assert(buffer.GetLength() == data_len + 2);
  
  FILE *fp = fopen(test_file_name, "wb");
  assert(fp != nullptr);
  
  buffer.WriteToFile(fp);
  
  fclose(fp);
  
  char *s = reinterpret_cast<char *>(FileUtility::LoadFile(test_file_name));
  // ISO C++ forbids converting a string constant to ¡®char*¡¯
  const char *result = "a0123456789abcdefghijklmnopqrstuvwxyz";
  
  assert(strcmp(result, s) == 0);
  
  delete[] s;
  
  // Delete the temp file for testing
  unlink(test_file_name);
  
  return;
}

/*
 * Tests whether printf in the buffer works
 */
void TestPrintf() {
  _PrintTestName();
  
  Buffer buffer{};
  
  char t[4096];
  
  buffer.Printf("%c", 'H');
  buffer.Printf("ello, world!\n");
  buffer.Printf("My name is: %s\n", "Ziqi Wang");
  buffer.Printf("My birthday is %04d-%02d-%02d\n", 1993, 6, 1);
  buffer.AppendByte('\0');
  
  int offset = 0;
  
  offset += sprintf(t + offset, "%c", 'H');
  offset += sprintf(t + offset, "ello, world!\n");
  offset += sprintf(t + offset, "My name is: %s\n", "Ziqi Wang");
  offset += sprintf(t + offset, "My birthday is %04d-%02d-%02d\n", 1993, 6, 1);  
  
  // Make sure we did not overflow the buffer (for correctness testing)
  assert(offset + 1 < 4096);
  
  buffer.WriteToFile(stderr); 
  assert(strcmp(t, static_cast<const char *>(buffer.GetData())) == 0);
  
  return;
}

/*
 * TestUtf16() - Tests whether we could convert UTF16 string to UTF8 and 
 *               print it to the buffer
 */
void TestUtf16() {
  _PrintTestName();
  
  Buffer buffer;
  Buffer buffer2;
  
  // UTF16 string with 0x00 0x00 as terminating character
  uint16_t p[] = {0x8FD9, 0x662F, 0x4E00, 0x4E2A, 0x6D4B, 0x8BD5, 0xFF08, 
                  0x0055, 0x0054, 0x0046, 0x002D, 0x0031, 0x0036, 0xFF09, 
                  0x002F, 0x0054, 0x0068, 0x0069, 0x0073, 0x0020, 0x0069, 
                  0x0073, 0x0020, 0x0061, 0x0020, 0x0055, 0x0054, 0x0046, 
                  0x002D, 0x0031, 0x0036, 0x0020, 0x0074, 0x0065, 0x0073, 
                  0x0074, 0x0000};
  
  // Not length prefixed
  Utf16String s{const_cast<unsigned char *>(reinterpret_cast<const unsigned char *>(p)), false};
  
  s.PrintUtf8(&buffer);
  buffer.AppendByte('\n');
  
  buffer2.Append(reinterpret_cast<const unsigned char *>(p), 4);
  buffer2.AppendByte('\n');
  
  // This should be an error because we try to interpret UTF16 in UTF8 format
  dbg_printf("Original bytes: ");
  buffer2.WriteToFile(stderr);
  
  dbg_printf("UTF-8 result: ");
  buffer.WriteToFile(stderr);
  
  return;
}

int main() {
  EnterTestDir();
  
  TestBasicExtend();
  TestPrintf();
  TestUtf16();
  
  return 0;
}
