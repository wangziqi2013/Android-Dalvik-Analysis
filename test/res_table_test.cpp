
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
  unsigned char *data = FileUtility::LoadFile(file_name, &length);
  
  ResourceTable table{data, length, true};
  
  return;
}

int main(int argc, char **argv) {
  EnterTestDir();
  
  Argv args{argc, argv};
  const std::vector<std::string> &arg_list = args.GetArgList();
  if(arg_list.size() == 0UL) {
    TestResourceTableBasic("resources.arsc");
  } else if(arg_list.size() == 1UL) {
    TestResourceTableBasic(arg_list[0].c_str());
  } else {
    assert(false && "Wrong argument list - too many input files!");
  }
  
  return 0; 
}
