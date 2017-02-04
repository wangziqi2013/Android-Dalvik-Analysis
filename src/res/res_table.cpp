
#include "res_table.h"

namespace wangziqi2013 {
namespace android_dalvik_analysis {

// This simplifies the scope resolution operator to a shorter string
#ifdef TYPE
#error "Macro TYPE has already been defined"
#else
#define TYPE ResourceTable::Type
#endif



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
 * WriteXml() - Writes an XML file that represents the structure of the
 *              current type
 *
 * The XML file is written according to the type of the resource. We
 * hardcode the format for each type and its identifier, e.g. attr, string,
 * drawable, etc.
 */
void TYPE::WriteXml() {
  // Retrieve the base type name
  const Buffer &base_type_name = *type_spec_p->GetBaseTypeName();
  
  if(base_type_name == "attr") {
    WriteAttrXml("attrs.xml");
  } else if(base_type_name == "drawable") {
    WriteDrawableXml("drawbles.xml");
  } else if(base_type_name == "layout") {
    // Decompile binary XML
    ProcessLayoutXml();
  } else if(base_type_name == "anim") {
    // Decompile binary XML
    ProcessAnimXml();
  } else if(base_type_name == "xml") {
    // Decompile binary XML
    ProcessXmlTypeXml();
  } else if(base_type_name == "raw") {
    // Do nothing regarding raw file type
    ProcessRawType();
  } else if(base_type_name == "array") {
    WriteArrayXml("arrays.xml");
  } else if(base_type_name == "bool") {
    WriteBoolXml("bools.xml");
  } else if(base_type_name == "color") {
    WriteColorXml("colors.xml");
  } else if(base_type_name == "id") {
    WriteIdXml("ids.xml");
  } else if(base_type_name == "style") {
    WriteStyleXml("styles.xml"); 
  } else if(base_type_name == "string") {
    WriteStringXml("strings.xml");
  } else if(base_type_name == "dimen") {
    WriteDimenXml("dimens.xml");
  } else {
#ifndef NDEBUG
    dbg_printf("Unknown attribute name: ");
    base_type_name.WriteToFile(stderr);
    fprintf(stderr, "\n");
#endif

    ReportError(UNKNOWN_TYPE_TO_WRITE_XML); 
  }
  
  return;
}

/*
 * WriteColorTagLine() - Writes a tag line strarting with "color"
 *
 * This function actually writes into file and will clear the buffer
 * after the line being written
 *
 * This function has to be implemented after all data structure declaration
 * because it uses forwarded declarated functions which could not be
 * decuced from previous code
 */
void TYPE::WriteColorTagLine(ResourceTable *table_p,
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
