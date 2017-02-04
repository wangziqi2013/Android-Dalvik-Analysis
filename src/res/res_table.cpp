
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

/*
 * WriteColorTagLine() - Writes a tag line strarting with "color"
 *
 * This function actually writes into file and will clear the buffer
 * after the line being written
 */
void ResourceTable::Type::WriteColorTagLine(ResourceTable *table_p,
                                            Package *package_p, 
                                            ResourceEntry *entry_p,
                                            Buffer *buffer_p,
                                            FILE *fp) {
  buffer_p->Append("<color name=\"");
  package_p->key_string_pool.AppendToBuffer(entry_p->key, buffer_p);
  buffer_p->Append("\">");

  table_p->AppendResourceValueToBuffer(&entry_p->value, buffer_p);
  
  buffer_p->Append("</color>\n");
  buffer_p->Append('\0');
  
  FileUtility::WriteString(fp, buffer_p->GetCharData(), 1);
  buffer_p->Reset();
  
  return;
}


} // namespace android_dalvik_analysis
} // namespace wangziqi2013
