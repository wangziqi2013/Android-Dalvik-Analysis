
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
  
  ApkArchive("test.apk");
  
  return;
}

int main() {
  TestReadFileHeader();
  
  return 0; 
}
