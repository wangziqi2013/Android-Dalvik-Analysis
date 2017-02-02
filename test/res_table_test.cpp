
/*
 * res_table_test.cpp - Tests resource table
 */

#include "test_suite.h"
#include "common.h"
#include "res_table.h"

using namespace wangziqi2013;
using namespace android_dalvik_analysis;

/*
 * TestResourceTableBasic() - Tests whether basic stuff works
 */
void TestResourceTableBasic(const char *file_name) {
  _PrintTestName();
  
  dbg_printf("File name: %s\n", file_name);
  
  size_t length;
  unsigned char *data = FileUtility::MapFileReadOnly(file_name, &length);
  
  ResourceTable table{data, length, false};
  
  fprintf(stderr, "\n========================================\n");
  fprintf(stderr, "========================================\n");
  fprintf(stderr, "========================================\n");
  
  table.DebugPrintAll();
  
  fprintf(stderr, "\n========================================\n");
  fprintf(stderr, "========================================\n");
  fprintf(stderr, "========================================\n\n");
  
  table.DebugWriteXml();
  
  FileUtility::UnmapFile(data, length); 
  
  return;
}

/*
 * LoadSystemResource() - Loads Android system's resource file first, since
 *                        some app might refer to the resource ID in the systems
 *                        resource file 
 */
unsigned char *LoadSystemResource(size_t *length_p) {
  _PrintTestName();
  
  size_t length;
  unsigned char *data = \
    FileUtility::MapFileReadOnly("resources-system.arsc", &length);
  
  ResourceTable table{data, length, false};
  
  dbg_printf("Writing XML for Android system resource file\n");
  table.DebugWriteXml();
  
  // Return the length for later freeing this
  *length_p = length;
  
  return data;
}

int main(int argc, char **argv) {
  EnterTestDir();
  
  dbg_printf("Opening Android system resource file\n");
  size_t sys_res_length;
  unsigned char *sys_res_data_p = LoadSystemResource(&sys_res_length);
  dbg_printf("Finished opening Android system resource file\n");
  
  Argv args{argc, argv};
  const std::vector<std::string> &arg_list = args.GetArgList();
  if(arg_list.size() == 0UL) {
    TestResourceTableBasic("resources.arsc");
  } else if(arg_list.size() == 1UL) {
    TestResourceTableBasic(arg_list[0].c_str());
  } else {
    assert(false && "Wrong argument list - too many input files!");
  }
  
  FileUtility::UnmapFile(sys_res_data_p, sys_res_length); 
  
  return 0; 
}
