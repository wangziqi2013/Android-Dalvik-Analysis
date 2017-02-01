
#include "res_table.h"

namespace wangziqi2013 {
namespace android_dalvik_analysis {

/*
 * GetResourceIdStringWrapper() - Wraps the corresponding function in class 
 *                                ResourceTable
 *
 * This is declared for class ResourceBase to call a static member function
 * inside class ResourceTable
 */
void GetResourceIdStringWrapper(ResourceId id, Buffer *buffer_p) {
  ResourceTable::GetResourceIdString(id, buffer_p);
  
  return;
}

} // namespace android_dalvik_analysis
} // namespace wangziqi2013
