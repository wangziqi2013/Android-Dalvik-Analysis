
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
  } else if(base_type_name == "animator") {
    ProcessAnimatorXml();
  } else if(base_type_name == "interpolator") {
    ProcessInterpolatorXml();
  } else if(base_type_name == "mipmap") {
    ProcessMipmapXml();
  } else if(base_type_name == "integer") {
    WriteIntegerXml("integers.xml");
  } else if(base_type_name == "transition") {
    ProcessTransitionXml();
  } else if(base_type_name == "fraction") {
    WriteFractionXml("fractions.xml");
  } else if(base_type_name == "plurals") {
    WritePluralsXml("plurals.xml");
  } else if(base_type_name == "menu") {
    ProcessMenuXml();
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

/*
 * PrintAttrFormat() - Prints the format of attributes into the buffer
 */
void TYPE::PrintAttrFormat(Buffer *buffer_p, uint32_t format) {
  if(format & ResourceEntryField::TYPE_REFERENCE) {
    buffer_p->Append("reference|");
  }
  
  if(format & ResourceEntryField::TYPE_STRING) {
    buffer_p->Append("string|");
  }
  
  if(format & ResourceEntryField::TYPE_INTEGER) {
    buffer_p->Append("integer|");
  }
  
  if(format & ResourceEntryField::TYPE_BOOLEAN) {
    buffer_p->Append("boolean|");
  }
  
  if(format & ResourceEntryField::TYPE_COLOR) {
    buffer_p->Append("color|");
  }
  
  if(format & ResourceEntryField::TYPE_FLOAT) {
    buffer_p->Append("float|");
  }
  
  if(format & ResourceEntryField::TYPE_DIMENSION) {
    buffer_p->Append("dimension|");
  }
  
  if(format & ResourceEntryField::TYPE_FRACTION) {
    buffer_p->Append("fraction|");
  }
  
  buffer_p->Rewind(1);
  
  return;
}

/*
 * WriteAttrEnumFlags() - Writes nested attr types, i.e. enum or flags
 *                        to the file pointer
 */
void TYPE::WriteAttrEnumFlags(FILE *fp,
                              Buffer *buffer_p, 
                              uint32_t format,
                              ResourceEntryField *field_p,
                              uint32_t entry_count) {
  // Need this to print the name of another entry
  Package *package_p = type_spec_p->package_p;
  
  const char *tag = nullptr; 
  
  if((format & ResourceEntryField::TYPE_ENUM) != 0x00000000) {
    tag = "<enum name=\"";
  } else {
    tag = "<flag name=\"";
  }
  
  for(uint32_t i = 0;i < entry_count;i++) {
    // If it is a type enum then print it at the 2nd ident level
    buffer_p->Append(tag);
    
    ResourceTable *table_p = \
      package_group.GetResourceTable(field_p->name.package_id);
    ResourceEntry *entry_p = \
      table_p->GetResourceEntry(field_p->name, &header_p->config);
      
    package_p->key_string_pool.AppendToBuffer(entry_p->key, buffer_p);
    
    buffer_p->Append("\" value=\"");
    
    table_p->AppendResourceValueToBuffer(&field_p->value, 
                                         buffer_p, 
                                         &header_p->config);
    buffer_p->Append("\" />\n");
    buffer_p->Append('\0');
    
    FileUtility::WriteString(fp, buffer_p->GetCharData(), 2);
    
    field_p++;
    
    // Must do this for every iteration otherwise we repeat the 
    // same content
    buffer_p->Reset(); 
  }
  
  return;
}

/*
 * PrintAttrOther() - Prints everything that is not an attribute flags
 *                    or enum
 *
 * Accepts the pointer to the first field entry, and returns the 
 * number of fields processed
 */
size_t TYPE::PrintAttrOther(ResourceEntryField *field_p,
                            ResourceEntry *entry_p,
                            ResourceTable *table_p,
                            Buffer *buffer_p,
                            uint32_t format_mask) {
  size_t processed_entry = 0UL;
  
  // Try to loop over all entry fields. We may break halfway
  // if there is a resource ID
  for(size_t i = 0;i < entry_p->entry_count;i++) {
    uint32_t name = field_p->name.data;
    
    if(name == ResourceEntryField::ATTR_TYPE) {
      // Only prints if there is at least one type specified
      // It is possible to have none, if there is a flags or enum
      // following
      if((format_mask & \
          ResourceEntryField::TYPE_ANY) != 0x00000000) {
        buffer_p->Append(" format=\"");
        PrintAttrFormat(buffer_p, format_mask);
        buffer_p->Append('\"');
      }
    } else if(name == ResourceEntryField::ATTR_MIN) {
      buffer_p->Append(" min=\"");
      
      table_p->AppendResourceValueToBuffer(&field_p->value, 
                                           buffer_p, 
                                           &header_p->config);
      
      buffer_p->Append('\"');
    } else if(name == ResourceEntryField::ATTR_MAX) {
      buffer_p->Append(" max=\"");
      
      table_p->AppendResourceValueToBuffer(&field_p->value, 
                                           buffer_p, 
                                           &header_p->config);
      
      buffer_p->Append('\"');
    } else if(name == ResourceEntryField::ATTR_L10N) {
      buffer_p->Append(" localization=\"");
      
      // Then print the corresponding value for localization
      if(field_p->value.data == ResourceEntryField::L10N_SUGGESTED) {
        buffer_p->Append("suggested\""); 
      } else if(field_p->value.data == \
                ResourceEntryField::L10N_NOT_REQUIRED) {
        buffer_p->Append("not_required\"");
      } else {
        ReportError(INVALID_ATTR_ENTRY, "localization settings unknown");
      } 
    } else if(name == ResourceEntryField::ATTR_OTHER || \
              name == ResourceEntryField::ATTR_ZERO || \
              name == ResourceEntryField::ATTR_ONE || \
              name == ResourceEntryField::ATTR_TWO || \
              name == ResourceEntryField::ATTR_FEW || \
              name == ResourceEntryField::ATTR_MANY) {
      // Don't know how to process it
      assert(false);
    } else {
      break; 
    }
    
    processed_entry++;
    field_p++;
  } // for 
  
  return processed_entry;
}

/*
 * WriteAttrXml() - Treat this as an attribute type resource and write XML
 *
 * We try to create and enter the directory res/values-???/ where ??? is the
 * type information, and then create an XML file named "attrs.xml"
 *
 * Note that this function takes the file name tobe written since the file
 * name is different from what is recorded in the type string pool
 */
void TYPE::WriteAttrXml(const char *file_name) {
  FILE *fp = SwitchToValuesDir(file_name);
  
  FileUtility::WriteString(fp, XML_HEADER_LINE);
  
  // We re-use this buffer for each entry and rewind it to 
  // the beginning everytime
  Buffer buffer;
  
  // Then loop through all attributes and print them to the file 
  for(size_t i = 0;i < entry_count;i++) {
    if(IsEntryPresent(i) == false) {
      continue;
    } 
    
    ResourceEntry *entry_p = GetEntryPtr(i);
    
    // Attributes must be complex because we want to know its format
    if(entry_p->IsComplex() == false) {
      ReportError(INVALID_ATTR_ENTRY, "Attribute entry must be complex"); 
    } else if(entry_p->entry_count == 0) {
      ReportError(INVALID_ATTR_ENTRY, 
                  "Attribute entry must have at least 1 field"); 
    } else if(entry_p->parent_id.data != 0x00000000) {
      ReportError(INVALID_ATTR_ENTRY, 
                  "Attribute parent ID must be 0"); 
    }
    
    Package *package_p = type_spec_p->package_p;
    ResourceTable *table_p = package_p->table_p;
    
    // First print name:
    buffer.Append("<attr name=\"");
    package_p->key_string_pool.AppendToBuffer(entry_p->key, 
                                              &buffer);
    buffer.Append('\"');
    
    // This points to the only field
    ResourceEntryField *field_p = entry_p->field_data; 
    
    // This describes what will follow
    assert(field_p->name.data == ResourceEntryField::ATTR_TYPE);
    
    // This is a bit mask that specifies the format of data allowed
    uint32_t format_mask = field_p->value.data;
    
    // This flag indicates whether there are enum or flags after all 
    // other entries are processed
    bool has_enum_or_flags = false;
    if((format_mask & \
        (ResourceEntryField::TYPE_ENUM | \
         ResourceEntryField::TYPE_FLAGS)) != 0x00000000) {
      has_enum_or_flags = true;       
    }
    
    // Print all other stuff and then return the number of fields
    // having been processed
    size_t processed_entry = \
      PrintAttrOther(field_p, entry_p, table_p, &buffer, format_mask);
    
    if(has_enum_or_flags == false) {
      buffer.Append(" />\n");
    } else {
      buffer.Append(">\n");
    }
            
    // Terminate this string
    buffer.Append('\0');
    // Write with identation level = 1
    FileUtility::WriteString(fp, buffer.GetCharData(), 1);
    buffer.Reset();
    
    if(has_enum_or_flags == true) {          
      // It is an array, so need to pass the starting element and 
      // entry count
      WriteAttrEnumFlags(fp,
                         &buffer, 
                         format_mask, 
                         field_p + processed_entry, 
                         entry_p->entry_count - processed_entry);
      
      FileUtility::WriteString(fp, "</attr>\n", 1);
      buffer.Reset();
    }
  } // loop through all entries
  
  FileUtility::WriteString(fp, RESOURCE_END_TAG);
  FileUtility::CloseFile(fp);
  
  return;
}

/*
 * WriteDrawableXml() - Writes drawable as a XML file
 *
 * Note that not all drawables are written into the XML. In particular,
 * we do not extract file name represented as strings into the XML
 *
 * A consequence of selected extraction is that we might directly
 * jump the file and does not create the /values directory and the file
 * if there is nothing to be printed for the current configuration
 */
void TYPE::WriteDrawableXml(const char *file_name) {
  // If there is nothing to print just return without creating
  // the path
  if(HasNonStringDrawableEntry() == false) {
    return; 
  }
  
  FILE *fp = SwitchToValuesDir(file_name);
  
  FileUtility::WriteString(fp, XML_HEADER_LINE);
  Buffer buffer;
  
  // Need its string pool
  Package *package_p = type_spec_p->package_p;
  ResourceTable *table_p = package_p->table_p;
  
  // Basically the same for every resource type, just copied from 
  // another function
  for(size_t i = 0;i < entry_count;i++) {
    if(IsEntryPresent(i) == false) {
      continue; 
    }
    
    ResourceEntry *entry_p = GetEntryPtr(i); 
    
    if(entry_p->IsComplex() == true) {
      ReportError(INVALID_DRAWABLE_ENTRY, i);
    } else if(entry_p->value.type == ResourceValue::DataType::STRING) {
      continue;
    }
    
    // After this point we know the entry is not string and is valid
    
    FileUtility::WriteString(fp, "<item type=\"drawable\" name=\"", 1);
    
    package_p->key_string_pool.AppendToBuffer(entry_p->key, &buffer);
    buffer.Append("\">");
    
    table_p->AppendResourceValueToBuffer(&entry_p->value, 
                                         &buffer,
                                         &header_p->config);
    
    buffer.Append("</item>\n");
    buffer.WriteToFile(fp);
    
    // Clear all previous contents
    buffer.Reset();
  }
  
  FileUtility::WriteString(fp, RESOURCE_END_TAG);
  FileUtility::CloseFile(fp);
  
  return;
}

/*
 * WriteArrayXml() - Writes arrays.xml file
 */
void TYPE::WriteArrayXml(const char *file_name) {
  FILE *fp = SwitchToValuesDir(file_name);
  
  FileUtility::WriteString(fp, XML_HEADER_LINE);
  Buffer buffer;
  
  Package *package_p = type_spec_p->package_p;
  
  for(size_t i = 0;i < entry_count;i++) {
    if(IsEntryPresent(i) == false) {
      continue; 
    }
    
    ResourceEntry *entry_p = GetEntryPtr(i); 
    
    // Array entry must be complex, otherwise we could not
    // intepret it
    if(entry_p->IsComplex() == false) {
      ReportError(INVALID_ARRAY_ENTRY, i);
    }
    
    // If there is no entry then we just close the tag at the same line 
    // and continue to the next entry
    if(entry_p->entry_count == 0) {
      buffer.Append("<array name=\"");
      package_p->key_string_pool.AppendToBuffer(entry_p->key, &buffer);
      
      buffer.Append("\" />\n"); 
      buffer.Append('\0');
    
      FileUtility::WriteString(fp, buffer.GetCharData(), 1);
      buffer.Reset();
      
      continue;
    }
    
    // We have already asserted that the array must have at least
    // one element, so we know the first field is valid
    ResourceEntryField *field_p = entry_p->field_data;
    
    // We use this as a sample value to determine the type 
    // of the array
    ResourceValue *value_p = &field_p->value;
    const char *array_tag = nullptr;     
    
    // Next determine the array tag
    if(field_p->value.type == ResourceValue::DataType::REFERENCE) {
      ResourceId id;
      id.data = field_p->value.data;
      
      const Buffer &base_type_name = *GetResourceIdBaseTypeName(id);
        
      if(base_type_name == "string") {
        array_tag = ARRAY_TAG_LIST[STRING_ARRAY_TYPE_INDEX];
      } else if(base_type_name == "integer") {
        array_tag = ARRAY_TAG_LIST[INTEGER_ARRAY_TYPE_INDEX];
      } else {
        array_tag = ARRAY_TAG_LIST[OTHER_ARRAY_TYPE_INDEX]; 
      }
    } else {
      if(value_p->type == ResourceValue::DataType::INT_DEC || \
         value_p->type == ResourceValue::DataType::INT_HEX) {
        array_tag = ARRAY_TAG_LIST[INTEGER_ARRAY_TYPE_INDEX];
      } else if(value_p->type == ResourceValue::DataType::STRING) {
        array_tag = ARRAY_TAG_LIST[STRING_ARRAY_TYPE_INDEX];
      } else {
        // Simple type other than strings and integers
        array_tag = ARRAY_TAG_LIST[OTHER_ARRAY_TYPE_INDEX]; 
      } 
    }
    
    // Write the beginning tag first
    buffer.Printf("<%s name=\"", array_tag);
    package_p->key_string_pool.AppendToBuffer(entry_p->key, &buffer);
    
    buffer.Append("\">\n");
    buffer.Append('\0');
    
    FileUtility::WriteString(fp, buffer.GetCharData(), 1);
    buffer.Reset();
    
    for(size_t j = 0;j < entry_p->entry_count;j++) {
      // Array index is also recorded inside the name of the field
      // if it does not match the actual index then report error
      if(field_p->name.GetArrayIndex() != j) {
        ReportError(WRONG_ARRAY_INDEX, 
                    j, 
                    static_cast<uint64_t>(field_p->name.GetArrayIndex()));
      }
      
      buffer.Append("<item>");
      package_p->table_p->AppendResourceValueToBuffer(&field_p->value, 
                                                      &buffer,
                                                      &header_p->config);
      buffer.Append("</item>");
      
      buffer.Append('\n');
      buffer.Append('\0');
      
      FileUtility::WriteString(fp, buffer.GetCharData(), 2);
      buffer.Reset();
      
      field_p++; 
    }
    
    buffer.Printf("</%s>\n", array_tag);
    buffer.Append('\0');
    
    FileUtility::WriteString(fp, buffer.GetCharData(), 1);
    buffer.Reset();
  }
  
  FileUtility::WriteString(fp, RESOURCE_END_TAG);
  FileUtility::CloseFile(fp);
  
  return;
}

/*
 * WriteBoolXml() - Writes a XML file on boolean constants
 */
void TYPE::WriteBoolXml(const char *file_name) {
  FILE *fp = SwitchToValuesDir(file_name);
  
  FileUtility::WriteString(fp, XML_HEADER_LINE);
  Buffer buffer;
  
  Package *package_p = type_spec_p->package_p;
  ResourceTable *table_p = package_p->table_p;
  
  for(size_t i = 0;i < entry_count;i++) {
    if(IsEntryPresent(i) == false) {
      continue; 
    }
    
    ResourceEntry *entry_p = GetEntryPtr(i); 
    
    // Bool entry must be not complex
    if(entry_p->IsComplex() == true) {
      ReportError(INVALID_BOOL_ENTRY, i);
    }
    
    buffer.Append("<bool name=\"");
    package_p->key_string_pool.AppendToBuffer(entry_p->key, &buffer);
    buffer.Append("\">");
    
    table_p->AppendResourceValueToBuffer(&entry_p->value, 
                                         &buffer, 
                                         &header_p->config);
    
    buffer.Append("</bool>\n");
    buffer.Append('\0');
    
    FileUtility::WriteString(fp, buffer.GetCharData(), 1);
    buffer.Reset();
  }
  
  FileUtility::WriteString(fp, RESOURCE_END_TAG);
  FileUtility::CloseFile(fp);
  
  return;
}

/*
 * WriteIdXml() - Writes all ID information into a XML file
 */
void TYPE::WriteIdXml(const char *file_name) {
  FILE *fp = SwitchToValuesDir(file_name);
  
  FileUtility::WriteString(fp, XML_HEADER_LINE);
  Buffer buffer;
  
  // We need an extra buffer to hold the id value and check
  // whether it is 0 length or not
  Buffer id_value_buffer;
  
  Package *package_p = type_spec_p->package_p;
  ResourceTable *table_p = package_p->table_p;
  
  for(size_t i = 0;i < entry_count;i++) {
    if(IsEntryPresent(i) == false) {
      continue; 
    }
    
    ResourceEntry *entry_p = GetEntryPtr(i); 
    
    // ID entry must not be complex
    if(entry_p->IsComplex() == true) {
      ReportError(INVALID_ID_ENTRY, i);
    }
    
    buffer.Append("<item type=\"id\" name=\"");
    package_p->key_string_pool.AppendToBuffer(entry_p->key, &buffer);
    
    table_p->AppendResourceValueToBuffer(&entry_p->value, 
                                         &id_value_buffer,
                                         &header_p->config);
    if(id_value_buffer.GetLength() == 0UL) {
      buffer.Append("\" />\n");
    } else {
      buffer.Append("\">");
      buffer.Append(id_value_buffer);
      buffer.Append("</item>\n"); 
    }
    
    buffer.Append('\0');
    
    FileUtility::WriteString(fp, buffer.GetCharData(), 1);
    buffer.Reset();
    id_value_buffer.Reset();
  }
  
  FileUtility::WriteString(fp, RESOURCE_END_TAG);
  FileUtility::CloseFile(fp);
  
  return;
}

/*
 * WriteStyleXml() - Writes a style XML file
 */
void TYPE::WriteStyleXml(const char *file_name) {
  FILE *fp = SwitchToValuesDir(file_name);
  
  FileUtility::WriteString(fp, XML_HEADER_LINE);
  Buffer buffer;
  
  Package *package_p = type_spec_p->package_p;
  ResourceTable *table_p = package_p->table_p;
  
  for(size_t i = 0;i < entry_count;i++) {
    if(IsEntryPresent(i) == false) {
      continue; 
    }
    
    ResourceEntry *entry_p = GetEntryPtr(i); 
    
    // Style entry must be complex
    if(entry_p->IsComplex() == false) {
      ReportError(INVALID_STYLE_ENTRY, i);
    }
    
    buffer.Append("<style name=\"");
    package_p->key_string_pool.AppendToBuffer(entry_p->key, &buffer);
    
    // For styles we also need to append parent information as a reference
    // string
    if(entry_p->parent_id.data != 0x00000000) {                   
      buffer.Append("\" parent=\"");
      GetResourceIdString(entry_p->parent_id, 
                          &header_p->config, 
                          &buffer);
    }
    
    // If there is no entry then this is a one line style
    if(entry_p->entry_count == 0) {
      buffer.Append("\" />\n");
      buffer.Append('\0');
      
      FileUtility::WriteString(fp, buffer.GetCharData(), 1);
      buffer.Reset();
      
      continue;
    } else {
      buffer.Append("\">\n");
      buffer.Append('\0');
      
      FileUtility::WriteString(fp, buffer.GetCharData(), 1);
      buffer.Reset();
      
      // For each field there is a item line
      ResourceEntryField *field_p = entry_p->field_data;
      for(size_t i = 0;i < entry_p->entry_count;i++) {
        buffer.Append("<item name=\"");  
        
        // We use the key of the entry as the name of the item
        ResourceEntry *name_entry_p = \
          GetResourceEntry(field_p->name, &header_p->config);
        
        package_p->key_string_pool.AppendToBuffer(name_entry_p->key, 
                                                  &buffer);
                                                  
        buffer.Append("\">");
        
        table_p->AppendResourceValueToBuffer(&field_p->value, 
                                             &buffer, 
                                             &header_p->config);
        buffer.Append("</item>\n");
        buffer.Append('\0');
        
        FileUtility::WriteString(fp, buffer.GetCharData(), 2);
        buffer.Reset();
        
        field_p++;
      }
      
      FileUtility::WriteString(fp, "</style>\n", 1);
    }
  }
  
  FileUtility::WriteString(fp, RESOURCE_END_TAG);
  FileUtility::CloseFile(fp);
  
  return;
}

/*
 * WriteStringXml() - Writes a string xml file
 */
void TYPE::WriteStringXml(const char *file_name) {
  FILE *fp = SwitchToValuesDir(file_name);
  
  FileUtility::WriteString(fp, XML_HEADER_LINE);
  Buffer buffer;
  
  Package *package_p = type_spec_p->package_p;
  ResourceTable *table_p = package_p->table_p;
  
  for(size_t i = 0;i < entry_count;i++) {
    if(IsEntryPresent(i) == false) {
      continue; 
    }
    
    ResourceEntry *entry_p = GetEntryPtr(i); 
    
    // String entry must be not complex
    if(entry_p->IsComplex() == true) {
      ReportError(INVALID_STRING_ENTRY, i);
    }
    
    // Print the tag first
    buffer.Append("<string name=\"");
    package_p->key_string_pool.AppendToBuffer(entry_p->key, &buffer);
    buffer.Append("\">");
    
    table_p->AppendResourceValueToBuffer(&entry_p->value, &buffer);
    
    buffer.Append("</string>\n");
    buffer.Append('\0');
    
    FileUtility::WriteString(fp, buffer.GetCharData(), 1);
    buffer.Reset();
  }
  
  FileUtility::WriteString(fp, RESOURCE_END_TAG);
  FileUtility::CloseFile(fp);
  
  return;
}

/*
 * WriteDimenXml() - Writes a dimension XML file
 */
void TYPE::WriteDimenXml(const char *file_name) {
  FILE *fp = SwitchToValuesDir(file_name);
  
  FileUtility::WriteString(fp, XML_HEADER_LINE);
  Buffer buffer;
  
  Package *package_p = type_spec_p->package_p;
  ResourceTable *table_p = package_p->table_p;
  
  for(size_t i = 0;i < entry_count;i++) {
    if(IsEntryPresent(i) == false) {
      continue; 
    }
    
    ResourceEntry *entry_p = GetEntryPtr(i); 
    
    // Dimension entry must be not complex
    if(entry_p->IsComplex() == true) {
      ReportError(INVALID_DIMEN_ENTRY, i);
    }
    
    // If the type of the value is dimension then we just print
    // a dimension flag
    if(entry_p->value.type == ResourceValue::DataType::DIMENSION) {
      buffer.Append("<dimen name=\"");
      package_p->key_string_pool.AppendToBuffer(entry_p->key, &buffer);
      buffer.Append("\">");
    
      table_p->AppendResourceValueToBuffer(&entry_p->value, &buffer);
      
      buffer.Append("</dimen>\n");
      buffer.Append('\0');
    } else {
      buffer.Append("<item type=\"dimen\" name=\"");
      package_p->key_string_pool.AppendToBuffer(entry_p->key, &buffer);
      buffer.Append("\">");
      
      table_p->AppendResourceValueToBuffer(&entry_p->value, &buffer);
      
      buffer.Append("</item>\n");
      buffer.Append('\0');
    }
    
    FileUtility::WriteString(fp, buffer.GetCharData(), 1);
    buffer.Reset();
  }
  
  FileUtility::WriteString(fp, RESOURCE_END_TAG);
  FileUtility::CloseFile(fp);
  
  return;
}

/*
 * WriteColorXml() - Writes a color XML file
 */
void TYPE::WriteColorXml(const char *file_name) {
  FILE *fp = SwitchToValuesDir(file_name);
  
  FileUtility::WriteString(fp, XML_HEADER_LINE);
  Buffer buffer;
  
  Package *package_p = type_spec_p->package_p;
  ResourceTable *table_p = package_p->table_p;
  
  for(size_t i = 0;i < entry_count;i++) {
    if(IsEntryPresent(i) == false) {
      continue; 
    }
    
    ResourceEntry *entry_p = GetEntryPtr(i); 
    
    // Dimension entry must be not complex
    if(entry_p->IsComplex() == true) {
      ReportError(INVALID_COLOR_ENTRY, i);
    }
    
    ResourceValue::DataType type = entry_p->value.type;
     
    // If the type of the value is dimension then we just print
    // a dimension flag
    if(type == ResourceValue::DataType::INT_COLOR_ARGB8 || \
       type == ResourceValue::DataType::INT_COLOR_RGB8 || \
       type == ResourceValue::DataType::INT_COLOR_ARGB4 || \
       type == ResourceValue::DataType::INT_COLOR_RGB4) {
      WriteColorTagLine(table_p, package_p, entry_p, &buffer, fp);
      
      continue;
    } else if(type == ResourceValue::DataType::REFERENCE) {
      ResourceId id;
      id.data = entry_p->value.data;
      
      // If the simple entry is a reference then we use the value's 
      // data as a refrence ID and get its base type name
      const Buffer &base_type_name = *GetResourceIdBaseTypeName(id);
      
      // If the reference is from "color" type then also identify it as
      // a color (which might be another reference, but we do not 
      // resolve reference at this stage)
      if(base_type_name == "color") {
        WriteColorTagLine(table_p, package_p, entry_p, &buffer, fp);
        
        continue;
      }
    }
      
    buffer.Append("<item type=\"color\" name=\"");
    package_p->key_string_pool.AppendToBuffer(entry_p->key, &buffer);
    buffer.Append("\">");
    
    table_p->AppendResourceValueToBuffer(&entry_p->value, &buffer);
    
    buffer.Append("</item>\n");
    buffer.Append('\0');
    
    FileUtility::WriteString(fp, buffer.GetCharData(), 1);
    buffer.Reset();
  }
  
  FileUtility::WriteString(fp, RESOURCE_END_TAG);
  FileUtility::CloseFile(fp);
  
  return;
}

/*
 * WriteIntegerXml() - Writes XML for integer type
 */
void TYPE::WriteIntegerXml(const char *file_name) {
  FILE *fp = SwitchToValuesDir(file_name);
  
  FileUtility::WriteString(fp, XML_HEADER_LINE);
  Buffer buffer;
  
  Package *package_p = type_spec_p->package_p;
  ResourceTable *table_p = package_p->table_p;
  
  for(size_t i = 0;i < entry_count;i++) {
    if(IsEntryPresent(i) == false) {
      continue; 
    }
    
    ResourceEntry *entry_p = GetEntryPtr(i); 
    
    // Dimension entry must be not complex
    if(entry_p->IsComplex() == true) {
      ReportError(INVALID_INTEGER_ENTRY, i);
    }
    
    ResourceValue::DataType type = entry_p->value.type;
    
    // If the type of the value is dimension then we just print
    // a dimension flag
    if(type == ResourceValue::DataType::INT_DEC || \
       type == ResourceValue::DataType::INT_HEX) {
      buffer.Append("<integer name=\"");
      package_p->key_string_pool.AppendToBuffer(entry_p->key, &buffer);
      buffer.Append("\">");
    
      table_p->AppendResourceValueToBuffer(&entry_p->value, &buffer);
      
      buffer.Append("</integer>\n");
      buffer.Append('\0');
    } else if(type == ResourceValue::DataType::REFERENCE) {
      buffer.Append("<item type=\"integer\" name=\"");
      package_p->key_string_pool.AppendToBuffer(entry_p->key, &buffer);
      buffer.Append("\">");
      
      table_p->AppendResourceValueToBuffer(&entry_p->value, &buffer);
      
      buffer.Append("</item>\n");
      buffer.Append('\0');
    } else {
      dbg_printf("Unknown value type for integer type XML: 0x%04X\n",
                 static_cast<uint32_t>(type));
                 
      ReportError(INVALID_INTEGER_ENTRY, i);
    }
    
    FileUtility::WriteString(fp, buffer.GetCharData(), 1);
    buffer.Reset();
  }
  
  FileUtility::WriteString(fp, RESOURCE_END_TAG);
  FileUtility::CloseFile(fp);
  
  return;
}

/*
 * WriteFractionXml() - Writes XML for fraction type
 */
void TYPE::WriteFractionXml(const char *file_name) {
  FILE *fp = SwitchToValuesDir(file_name);
  
  FileUtility::WriteString(fp, XML_HEADER_LINE);
  Buffer buffer;
  
  Package *package_p = type_spec_p->package_p;
  ResourceTable *table_p = package_p->table_p;
  
  for(size_t i = 0;i < entry_count;i++) {
    if(IsEntryPresent(i) == false) {
      continue; 
    }
    
    ResourceEntry *entry_p = GetEntryPtr(i); 
    
    // Dimension entry must be not complex
    if(entry_p->IsComplex() == true) {
      ReportError(INVALID_FRACTION_ENTRY, i);
    }
    
    ResourceValue::DataType type = entry_p->value.type;
    
    // If the type of the value is dimension then we just print
    // a dimension flag
    if(type == ResourceValue::DataType::FRACTION) {
      buffer.Append("<fraction name=\"");
      package_p->key_string_pool.AppendToBuffer(entry_p->key, &buffer);
      buffer.Append("\">");
    
      table_p->AppendResourceValueToBuffer(&entry_p->value, &buffer);
      
      buffer.Append("</fraction>\n");
      buffer.Append('\0');
    } else if(type == ResourceValue::DataType::REFERENCE) {
      buffer.Append("<item type=\"fraction\" name=\"");
      package_p->key_string_pool.AppendToBuffer(entry_p->key, &buffer);
      buffer.Append("\">");
      
      table_p->AppendResourceValueToBuffer(&entry_p->value, &buffer);
      
      buffer.Append("</item>\n");
      buffer.Append('\0');
    } else {
      dbg_printf("Unknown value type for fraction type XML: 0x%04X\n",
                 static_cast<uint32_t>(type));
                 
      ReportError(INVALID_FRACTION_ENTRY, i);
    }
    
    FileUtility::WriteString(fp, buffer.GetCharData(), 1);
    buffer.Reset();
  }
  
  FileUtility::WriteString(fp, RESOURCE_END_TAG);
  FileUtility::CloseFile(fp);
  
  return;
}

/*
 * WritePluralsXml() - Writes plurals XML file
 */
void TYPE::WritePluralsXml(const char *file_name) {
  FILE *fp = SwitchToValuesDir(file_name);
  
  FileUtility::WriteString(fp, XML_HEADER_LINE);
  Buffer buffer;
  
  Package *package_p = type_spec_p->package_p;
  ResourceTable *table_p = package_p->table_p;
  
  for(size_t i = 0;i < entry_count;i++) {
    if(IsEntryPresent(i) == false) {
      continue; 
    }
    
    ResourceEntry *entry_p = GetEntryPtr(i); 
    
    // Dimension entry must be not complex
    if(entry_p->IsComplex() == false) {
      ReportError(INVALID_PLURALS_ENTRY, i);
    }
    
    buffer.Append("<plurals name=\"");
    package_p->key_string_pool.AppendToBuffer(entry_p->key, &buffer);
    buffer.Append("\">\n");
    buffer.Append('\0');
    
    // Write the plurals tag as first level line
    FileUtility::WriteString(fp, buffer.GetCharData(), 1);
    buffer.Reset();
    
    // Each entry field is a line in the XML file denoting the 
    // type of plural
    ResourceEntryField *field_p = entry_p->field_data;
    
    // Loop through all fields
    for(size_t j = 0;j < entry_p->entry_count;j++) {
      buffer.Append("<item quantity=\"");
      
      switch(field_p->name.data) {
        case ResourceEntryField::ATTR_OTHER: buffer.Append("other\">"); break;
        case ResourceEntryField::ATTR_ZERO: buffer.Append("zero\">"); break;
        case ResourceEntryField::ATTR_ONE: buffer.Append("one\">"); break;
        case ResourceEntryField::ATTR_TWO: buffer.Append("two\">"); break;
        case ResourceEntryField::ATTR_FEW: buffer.Append("few\">"); break;
        case ResourceEntryField::ATTR_MANY: buffer.Append("many\">"); break;
        default: {
          dbg_printf("Invalid field: unknown quantity type 0x%08X\n", 
                     field_p->name.data);
          
          ReportError(INVALID_PLURALS_ENTRY, i);
        } // default
      } // switch
      
      // Then append the actual value to the buffer and then terminate the line
      table_p->AppendResourceValueToBuffer(&field_p->value, &buffer);
      buffer.Append("</item>\n");
      buffer.Append('\0');
      
      // Write it at second level
      FileUtility::WriteString(fp, buffer.GetCharData(), 2);
      
      buffer.Reset();
      field_p++;
    }
    
    // Finish the block
    FileUtility::WriteString(fp, "</plurals>\n", 1);
  }
  
  FileUtility::WriteString(fp, RESOURCE_END_TAG);
  FileUtility::CloseFile(fp);
  
  return;
}

/*
 * ProcessLayoutXml() - This file does not write layouts.xml file
 *                      but instead it decompiles the binary XML 
 *                      indicated inside resource entries
 */
void TYPE::ProcessLayoutXml() {
  // TO BE IMPLEMENTED
  return;
}

/*
 * ProcessAnimXml() - Decompile the animation XML file recorded inside 
 *                    the entries
 */
void TYPE::ProcessAnimXml() {
  // TO BE IMPLEMENTED
  return;
}

/*
 * ProcessXmlTypeXml() - Decompile the XML type XML file recorded inside 
 *                       the entries
 */
void TYPE::ProcessXmlTypeXml() {
  // TO BE IMPLEMENTED
  return;
}

/*
 * ProcessRawType() - Decompile the raw type file recorded inside 
 *                    the entries
 */
void TYPE::ProcessRawType() {
  // TO BE IMPLEMENTED
  return;
}

/*
 * ProcessAnimatorXml() - Processes animator type XMLs
 */
void TYPE::ProcessAnimatorXml() {
  // TO BE IMPLEMENTED
  return;
}

/*
 * ProcessInterpolatorXml() - Processes interpolator type XMLs
 */
void TYPE::ProcessInterpolatorXml() {
  // TO BE IMPLEMENTED
  return;
}

/*
 * ProcessMipmapXml() - Processes mipmap type XMLs
 */
void TYPE::ProcessMipmapXml() {
  // TO BE IMPLEMENTED
  return;
}

/*
 * ProcessTransitionXml() - Processes transition type XMLs
 */
void TYPE::ProcessTransitionXml() {
  // TO BE IMPLEMENTED
  return;
}

/*
 * ProcessMenuXml() - Processes menu type XMLs
 */
void TYPE::ProcessMenuXml() {
  // TO BE IMPLEMENTED
  return;
}

} // namespace android_dalvik_analysis
} // namespace wangziqi2013
