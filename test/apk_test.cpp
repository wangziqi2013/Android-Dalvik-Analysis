
/*
 * apk_test.cpp - This file tests basic APK operation
 */ 

#include "apk.h"
#include "test_suite.h"

using namespace wangziqi2013;
using namespace android_dalvik_analysis;

/*
 * TestReadFileHeader() - Tests whether file header could be read
 */
void TestReadFileHeader() {
  _PrintTestName();
  
  ApkArchive apk{"./test.apk"};
  apk.DebugPrintAllFileName();
  
  return;
}

/*
 * TestExtractAll() - Tests whether we could extract all files
 */
void TestExtractAll() {
  _PrintTestName();
  
  ApkArchive apk{"./test.apk"};
  apk.ExtractAll("./test_apk_root/actual_test/");
 
  return;
}

/*
 * TestMalformedFile() - Tests whether malformed file could be recognized
 */
void TestMalformedFile() {
  _PrintTestName();
  
  try {
    // This would fail since there is no EOF central file record
    ApkArchive apk("./corrupt.apk");
  } catch(int) {
    fprintf(stderr, "Successfully caught the exception\n"); 
    
    return;
  } 
  
  assert(false);
}

/*
 * ZlibVersionTest() - As name suggests 
 */
void ZlibVersionTest() {
  _PrintTestName();
  
  dbg_printf("ZLib version: %s\n", zlibVersion());
  
  return;
}

int main() {
  TestMalformedFile();
  TestReadFileHeader();
  TestExtractAll();
  ZlibVersionTest();
  
  return 0; 
}