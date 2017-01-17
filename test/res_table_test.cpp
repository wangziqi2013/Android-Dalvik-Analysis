
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
void TestResourceTableBasic() {
  size_t length;
  unsigned char *data = FileUtility::LoadFile("resources.arsc", &length);
  
  ResourceTable table{data, length, true};
  
  return;
}

int main() {
  TestResourceTableBasic();
  
  return 0; 
}
