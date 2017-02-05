
/*
 * res_table_test.cpp - Tests resource table
 */

#include "test_suite.h"
#include "common.h"
#include "res_table.h"

using namespace wangziqi2013;
using namespace android_dalvik_analysis;

// If this is turned on we print out the package after loading them
bool print_flag = false;

// If this is turned on we also print system package content
bool print_system_flag = false;

/*
 * TestResourceTableBasic() - Tests whether basic stuff works
 */
void TestResourceTableBasic(const char *file_name) {
  _PrintTestName();
  
  dbg_printf("File name: %s\n", file_name);
  
  size_t length;
  unsigned char *data = FileUtility::MapFileReadOnly(file_name, &length);
  
  // Must be allocated on the heap because we need to free the memory
  // manually before memory is unmapped
  ResourceTable *table_p = new ResourceTable{data, length, false};
  
  if(print_flag == true) {
    fprintf(stderr, "\n========================================\n");
    fprintf(stderr, "========================================\n");
    fprintf(stderr, "========================================\n");
    
    table_p->DebugPrintAll();
  }
  
  fprintf(stderr, "\n========================================\n");
  fprintf(stderr, "========================================\n");
  fprintf(stderr, "========================================\n\n");
  
  table_p->DebugWriteXml();
  
  delete table_p;
  FileUtility::UnmapFile(data, length); 
  
  return;
}

/*
 * LoadSystemResource() - Loads Android system's resource file first, since
 *                        some app might refer to the resource ID in the systems
 *                        resource file 
 */
unsigned char *LoadSystemResource(size_t *length_p, 
                                  ResourceTable **table_p_p) {
  _PrintTestName();
  
  size_t length;
  unsigned char *data = \
    FileUtility::MapFileReadOnly("resources-system.arsc", &length);
  
  // This must be on the heap to avoid overwriting
  ResourceTable *table_p = new ResourceTable{data, length, false};
  
  if(print_flag == true && print_system_flag == true) {
    fprintf(stderr, "\n========================================\n");
    fprintf(stderr, "========================================\n");
    fprintf(stderr, "========================================\n");
    
    table_p->DebugPrintAll();
  }
  
  fprintf(stderr, "\n========================================\n");
  fprintf(stderr, "========================================\n");
  fprintf(stderr, "========================================\n\n");
  
  dbg_printf("Writing XML for Android system resource file\n");
  table_p->DebugWriteXml();
  
  // Return the length for later freeing this
  *length_p = length;
  *table_p_p = table_p;
  
  return data;
}

int main(int argc, char **argv) {
  EnterTestDir();
  
  Argv args{argc, argv};
  
  // Enabling flags optionally depending on the command line 
  if(args.Exists("print") == true) {
    print_flag = true; 
  }
  
  if(args.Exists("print-system") == true) {
    print_system_flag = true; 
  }
  
  dbg_printf("Opening Android system resource file\n");
  size_t sys_res_length;
  ResourceTable *table_p = nullptr;
  
  unsigned char *sys_res_data_p = \
    LoadSystemResource(&sys_res_length, &table_p);
    
  dbg_printf("Finished opening Android system resource file\n");
  
  const std::vector<std::string> &arg_list = args.GetArgList();
  if(arg_list.size() == 0UL) {
    TestResourceTableBasic("resources.arsc");
  } else if(arg_list.size() == 1UL) {
    TestResourceTableBasic(arg_list[0].c_str());
  } else {
    assert(false && "Wrong argument list - too many input files!");
  }
  
  // This does not try to free the mapped file region
  delete table_p;
  FileUtility::UnmapFile(sys_res_data_p, sys_res_length); 
  
  dbg_printf("print_flag = %d; print_system_flag = %d\n", 
             print_flag, 
             print_system_flag);
  
  return 0; 
}
