
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
BinaryXml *TestStringPool(const char *file_name) {
  _PrintTestName();
  
  size_t length;
  unsigned char *data = FileUtility::LoadFile(file_name, &length);
  
  dbg_printf("File %s opened. Length = %lu\n", file_name, length);
  
  // Bind the file data to an object and transfer ownership to the object
  BinaryXml *xml_p = new BinaryXml{data, length, true};
  assert(xml_p->IsValidXml() == true);
  
  xml_p->DebugPrintAllStrings();
  
  return xml_p;
}

/*
 * TestOutputXml() - Tests whether XML could be correctly decoded
 *
 * Since the XML file is already decoded 
 */
void TestOutputXml(BinaryXml *xml_p, const char *file_name) {
  _PrintTestName();
  
  Buffer buffer{file_name};
  xml_p->WriteXml(buffer);
  
  return; 
}
 
int main() {
  EnterTestDir();
  
  // This is UTF-16
  BinaryXml *xml_p = TestStringPool("AndroidManifest.xml");
  
  TestOutputXml(xml_p, "AndroidManifest-decoded.xml");
  
  // Frees memory and the data array since the xml object has ownership
  delete xml_p;
  
  return 0; 
}
