
/*
 * buffer_test.cpp - Test class Buffer
 */
 
#include "test_suite.h"
#include "common.h" 

static const char *test_file_name = "buffer_test.temp";

/*
 * TestBasicExtend() - Tests whether exted works
 */
void TestBasicExtend() {
  _PrintTestName();
  
  // Declear a buffer of size 1
  Buffer buffer{1};
  assert(buffer.GetLength() == 1UL);
  
  // Write 1 byte data into it - this does not cause extend
  buffer.AppendByte('a');
  assert(buffer.GetLength() == 1UL);
  
  // Append data and '\0' to the buffer
  char *data = "0123456789abcdefghijklmnopqrstuvwxyz";
  size_t data_len = strlen(data);
  
  buffer.Append(data, data_len + 1);
  
  // The length must be data len + 'a' + '\0'
  assert(buffer.GetLength() == data_len + 2);
  
  FILE *fp = fopen(test_file_name, "wb");
  assert(fp != nullptr);
  
  buffer.WriteToFile(fp);
  
  fclose(fp);
  
  char *s = FileUtility::LoadFile(test_file_name);
  
  assert(strcmp("a0123456789abcdefghijklmnopqrstuvwxyz", s) == 0);
  
  delete[] s;
  
  return;
}

int main() {
  TestBasicExtend();
  
  return 0;
}
