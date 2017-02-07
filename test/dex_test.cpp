
/*
 * dex_test.cpp - Tests unpacking dex file
 */

#include "test_suite.h"
#include "common.h"
#include "dex.h"

using namespace wangziqi2013;
using namespace android_dalvik_analysis;

/*
 * TestDexBasic() - Tests basic structural parsing of DEX file
 */
void TestDexBasic() {
  _PrintTestName();
  
  size_t length;
  unsigned char *data_p = FileUtility::LoadFile("classes.dex", &length);
  
  DexFile dex_file{data_p, length, true};
  dex_file.DebugPrintString();
  
  return; 
}

int main() {
  EnterTestDir();
  
  TestDexBasic();
  
  return 0; 
}
