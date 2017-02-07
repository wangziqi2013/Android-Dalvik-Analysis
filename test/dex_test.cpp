
/*
 * dex_test.cpp - Tests unpacking dex file
 */

#include "test_suite.h"
#include "common.h"
#include "dex.h"

/*
 * TestDexBasic() - Tests basic structural parsing of DEX file
 */
void TestDexBasic() {
  _PrintTestName();
  
  size_t length;
  unsigned char *data_p = FileUtility::LoadFile("classes.dex", &length);
  
  DexFile dex_file{data_p, length, true};
  
  return; 
}

int main() {
  TestDexBasic();
  
  return 0; 
}
