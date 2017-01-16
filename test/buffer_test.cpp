
/*
 * buffer_test.cpp - Test class Buffer
 */
 
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

int main() {
  TestBasicExtend();
  
  return 0;
}
