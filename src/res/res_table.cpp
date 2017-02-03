
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
void GetResourceIdStringWrapper(ResourceId id, 
                                const TypeConfig *type_config_p,
                                Buffer *buffer_p) {
  ResourceTable::GetResourceIdString(id, type_config_p, buffer_p);
  
  return;
}

} // namespace android_dalvik_analysis
} // namespace wangziqi2013
