
/*
 * xml_test.cpp - Tests whether XML parsing works
 */

#include "test_suite.h"
#include "common.h"
#include "xml.h"
#include "res_table.h"

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
  xml_p->WriteXml(&buffer);
  
  return; 
}

/*
 * ReadResource() - Reads system and app resources and register it into the
 *                  package group
 */
std::pair<ResourceTable *, ResourceTable *> ReadResource() {
  size_t sys_length, app_length;
  
  unsigned char *sys_data_p = \
    FileUtility::LoadFile("resources-system.arsc", &sys_length);
  unsigned char *app_data_p = \
    FileUtility::LoadFile("resources.arsc", &app_length);
  
  ResourceTable *sys_table_p = new ResourceTable{sys_data_p, sys_length, true};
  ResourceTable *app_table_p = new ResourceTable{app_data_p, app_length, true};
  
  return std::make_pair(sys_table_p, app_table_p);
}

int main() {
  EnterTestDir();
  
  auto p = ReadResource();
  
  // This is UTF-16
  BinaryXml *xml_p = TestStringPool("AndroidManifest.xml");
  
  TestOutputXml(xml_p, "AndroidManifest-decoded.xml");
  
  // Frees memory and the data array since the xml object has ownership
  delete xml_p;
  delete p.first;
  delete p.second;
  
  return 0; 
}
