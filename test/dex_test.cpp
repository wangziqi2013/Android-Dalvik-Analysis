
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
  
  dbg_printf("==============================\n");
  dbg_printf("String List\n");
  dbg_printf("==============================\n");
  
  dex_file.DebugPrintAllStrings();
  
  dbg_printf("==============================\n");
  dbg_printf("Type List\n");
  dbg_printf("==============================\n");
  
  dex_file.DebugPrintAllTypes();
  
  dbg_printf("==============================\n");
  dbg_printf("Proto List\n");
  dbg_printf("==============================\n");
  
  dex_file.DebugPrintAllProtos();
  
  return; 
}

int main() {
  EnterTestDir();
  
  TestDexBasic();
  
  return 0; 
}
