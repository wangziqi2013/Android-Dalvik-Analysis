
/*
 * xml_test.cpp - Tests whether XML parsing works
 */

#include "test_suite.h"
#include "common.h"
#include "xml.h"

using namespace wangziqi2013;
using namespace android_dalvik_analysis;

/*
 * TestStringPool() - Tests whether StringPool works and whether we could
 *                    print strings correctly
 */
void TestStringPool(const char *file_name) {
  _PrintTestName();
  
  size_t length;
  unsigned char *data = FileUtility::LoadFile(file_name, &length);
  
  dbg_printf("File %s opened. Length = %lu\n", file_name, length);
  
  // Bind the file data to an object and transfer ownership to the object
  BinaryXml xml{data, length, true};
  assert(xml.IsValidXml() == true);
  
  xml.DebugPrintAllStrings();
  
  return;
}
 
int main() {
  EnterTestDir();
  
  // This is UTF-16
  //TestStringPool("AndroidManifest.xml");
  // This is UTF-8
  TestStringPool("fragment_edit_dns.xml");
  
  return 0; 
}
