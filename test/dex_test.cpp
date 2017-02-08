
/*
 * dex_test.cpp - Tests unpacking dex file
 */

#include "test_suite.h"
#include "common.h"
#include "dex.h"

using namespace wangziqi2013;
using namespace android_dalvik_analysis;
using namespace dex;

/*
 * TestDexBasic() - Tests basic structural parsing of DEX file
 */
void TestDexBasic() {
  _PrintTestName();
  
  size_t length;
  unsigned char *data_p = FileUtility::LoadFile("classes.dex", &length);
  
  DexFile dex_file{data_p, length, true};
  FILE *fp = stderr;
  
  dbg_printf("==============================\n");
  dbg_printf("String List\n");
  dbg_printf("==============================\n");
  
  fp = FileUtility::OpenFile("string.log", "wb");
  FileUtility::RedirectStderrTo(fp);
  dex_file.DebugPrintAllStrings(fp);
  FileUtility::CloseFile(fp);
  FileUtility::RestoreStderr();
  
  dbg_printf("==============================\n");
  dbg_printf("Type List\n");
  dbg_printf("==============================\n");
  
  fp = FileUtility::OpenFile("type.log", "wb");
  FileUtility::RedirectStderrTo(fp);
  dex_file.DebugPrintAllTypes(fp);
  FileUtility::CloseFile(fp);
  FileUtility::RestoreStderr();
  
  dbg_printf("==============================\n");
  dbg_printf("Proto List\n");
  dbg_printf("==============================\n");
  
  fp = FileUtility::OpenFile("proto.log", "wb");
  FileUtility::RedirectStderrTo(fp);
  dex_file.DebugPrintAllProtos(fp);
  FileUtility::CloseFile(fp);
  FileUtility::RestoreStderr();
  
  dbg_printf("==============================\n");
  dbg_printf("Field List\n");
  dbg_printf("==============================\n");
  
  fp = FileUtility::OpenFile("field.log", "wb");
  FileUtility::RedirectStderrTo(fp);
  dex_file.DebugPrintAllFields(fp);
  FileUtility::CloseFile(fp);
  FileUtility::RestoreStderr();
  
  dbg_printf("==============================\n");
  dbg_printf("Method List\n");
  dbg_printf("==============================\n");
  
  fp = FileUtility::OpenFile("method.log", "wb");
  FileUtility::RedirectStderrTo(fp);
  dex_file.DebugPrintAllMethods(fp);
  FileUtility::CloseFile(fp);
  FileUtility::RestoreStderr();
  
  return; 
}

int main() {
  EnterTestDir();
  
  TestDexBasic();
  
  return 0; 
}
