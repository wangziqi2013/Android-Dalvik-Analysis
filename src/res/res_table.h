
#pragma once

#ifndef _RES_TBALE_H
#define _RES_TBALE_H

#include "common.h"
#include "res_base.h"
#include "package_group.h"

namespace wangziqi2013 {
namespace android_dalvik_analysis { 

/*
 * class ResourceTable - Represents binary resource table
 */
class ResourceTable : public ResourceBase {
 // type declarations
 private: 

  /*
   * class TableHeader - The resource table header which is at the beginning of 
   *                     the table
   */
  class TableHeader {
   public: 
    CommonHeader common_header;
    
    // Number of packages included in this resource table
    uint32_t package_count; 
  } BYTE_ALIGNED;
  
  /*
   * class PackageHeader - Package header that records metadata about package
   */
  class PackageHeader {
   public:
    CommonHeader common_header; 
     
    // Base package's ID (also in the res identifier)
    // If 0 then it is not a base package; it is always 0x7F for application
    // package type
    uint32_t id; 
    
    // UTF-16 encoded name of the package
    // The length is fixed 256 bytes encoded in UTF16 though UTF16 itself
    // is a variable length encoding scheme
    unsigned char name_utf16[128 * sizeof(char16_t)];
    
    // An offset from the beginning of this struct to the string pool for types
    uint32_t type_string_pool_offset;
    
    // Index into string pool to indicate the last type visible
    uint32_t last_public_type;
    
    // Same - an offset from this struct as the key string pool
    uint32_t key_string_pool_offset;
    
    // The last index of a key that is visible
    uint32_t last_public_key;
    
  } BYTE_ALIGNED;
  
  /*
   * class TypeSpecHeader - Type specification header
   *
   * This header is followed by an array of uint32_t integers denoting the 
   * configuration of resources available for a certain resource of this
   * type. There is excatly one entry for each resource instance
   */
  class TypeSpecHeader {
   public: 
    CommonHeader common_header;
    
    // Low 8 bits of a 32 bit integer represents the type id
    uint8_t id;
    uint8_t zero1;
    uint16_t zero2;
    
    // Number of entries after this struct
    uint32_t entry_count;
    
    // The pointer to the first entry (it does not consume struct space)
    uint32_t data[0];
  } BYTE_ALIGNED;
  
  /*
   * class TypeHeader - Header of the type instance chunk
   */
  class TypeHeader {
   public: 
    CommonHeader common_header;
    
    // This is the ID of the type being described here
    uint8_t type_id;
    uint8_t zero1;
    uint16_t zero2;
    
    // Number of resource value instances inside this chunk
    uint32_t entry_count;
    
    // The offset to the starting address of the data part
    // Between this structure and the header is an array of uint32_t
    // that records the offset of each instance into the data part
    uint32_t data_offset;
    
    // This structure records the config of this group of values
    // i.e. they are used under common configurations
    TypeConfig config;
  } BYTE_ALIGNED;
  
  // We need the pointer type before it is declared
  class TypeSpec;
  
  /*
   * class ResourceEntryField - If a resource entry is a complicated one then
   *                            this represents fields inside the resource entry
   */
  class ResourceEntryField {
   public:
    
    /*
     * Res_MAKEINTERNAL() - Makes special name field values for attr types
     */
    #define Res_MAKEINTERNAL(entry) (0x01000000 | (entry&0xFFFF))
    
    // Special values for 'name' when defining attribute resources.
    enum {
      // This entry holds the attribute's type code.
      ATTR_TYPE = Res_MAKEINTERNAL(0),

      // For integral attributes, this is the minimum value it can hold.
      ATTR_MIN = Res_MAKEINTERNAL(1),

      // For integral attributes, this is the maximum value it can hold.
      ATTR_MAX = Res_MAKEINTERNAL(2),

      // Localization of this resource is can be encouraged or required with
      // an aapt flag if this is set
      ATTR_L10N = Res_MAKEINTERNAL(3),

      // for plural support, see android.content.res.PluralRules#attrForQuantity(int)
      ATTR_OTHER = Res_MAKEINTERNAL(4),
      ATTR_ZERO = Res_MAKEINTERNAL(5),
      ATTR_ONE = Res_MAKEINTERNAL(6),
      ATTR_TWO = Res_MAKEINTERNAL(7),
      ATTR_FEW = Res_MAKEINTERNAL(8),
      ATTR_MANY = Res_MAKEINTERNAL(9), 
    };

    #undef Res_MAKEINTERNAL

    // Bit mask of allowed types, for use with ATTR_TYPE.
    enum {
      // No type has been defined for this attribute, use generic
      // type handling.  The low 16 bits are for types that can be
      // handled generically; the upper 16 require additional information
      // in the bag so can not be handled generically for TYPE_ANY.
      TYPE_ANY = 0x0000FFFF,

      // Attribute holds a references to another resource.
      TYPE_REFERENCE = 1<<0,

      // Attribute holds a generic string.
      TYPE_STRING = 1<<1,

      // Attribute holds an integer value.  ATTR_MIN and ATTR_MIN can
      // optionally specify a constrained range of possible integer values.
      TYPE_INTEGER = 1<<2,

      // Attribute holds a boolean integer.
      TYPE_BOOLEAN = 1<<3,

      // Attribute holds a color value.
      TYPE_COLOR = 1<<4,

      // Attribute holds a floating point value.
      TYPE_FLOAT = 1<<5,

      // Attribute holds a dimension value, such as "20px".
      TYPE_DIMENSION = 1<<6,

      // Attribute holds a fraction value, such as "20%".
      TYPE_FRACTION = 1<<7,

      // Attribute holds an enumeration.  The enumeration values are
      // supplied as additional entries in the map.
      TYPE_ENUM = 1<<16,

      // Attribute holds a bitmaks of flags.  The flag bit values are
      // supplied as additional entries in the map.
      TYPE_FLAGS = 1<<17
    };

    // Enum of localization modes, for use with ATTR_L10N.
    enum {
        L10N_NOT_REQUIRED = 0,
        L10N_SUGGESTED    = 1
    }; 
     
    // This field has different interpretations
    ResourceId name;
    ResourceValue value; 
  } BYTE_ALIGNED;
  
  /*
   * class ResourceEntry - Represents resource entry in the body of type chunk
   */
  class ResourceEntry {
   public: 
    // The length of only this structure (i.e. not responsible for 
    // data after this structure)
    uint16_t entry_length;
  
    /*
     * enum Flags
     */
    enum Flags : uint16_t {
      // This flag decides how the following data is organized
      // For a simple entry the following data is just a ResourceValue instance
      // Otherwise it is followed by a mapping descriptor and several maps
      // to form a composite value
      COMPLEX = 0x0001,
      PUBLIC = 0x0002,
      // If set, this is a weak resource and may be overriden by strong
      // resources of the same name/type. This is only useful during
      // linking with other resource tables.
      FLAG_WEAK = 0x0004,
    };
    
    // As defined above
    Flags flags;

    // A string into key string table of the package denoting the name of the
    // resource entry
    uint32_t key;
    
    // Different interpretation of the following bytes depending on
    // whether the type is complex or not
    // Note that the following structure is not counted as the length in
    // the common header
    
    union {
      // This struct is used only if the resource is a complex one
      struct {
        // The resource ID of its parent which refers to another resource
        // 0x00000000 if there is no parent
        ResourceId parent_id;
      
        // The number of key-value pairs after the body
        uint32_t entry_count;
        
        // This is the starting address of entry fields, there are 
        // entry_count of them
        ResourceEntryField field_data[0];
      } BYTE_ALIGNED;
      
      // If the resource is not a complex one then use this one
      ResourceValue value;
    } BYTE_ALIGNED;
    
    /*
     * IsComplex() - Whether the resource entry is composite
     */
    inline bool IsComplex() const {
      return flags & Flags::COMPLEX;
    }
    
    /*
     * IsPublic() - Returns true if the entry is in the public name space
     */
    inline bool IsPublic() const {
      return flags & Flags::PUBLIC; 
    }
  } BYTE_ALIGNED;
  
  class Package;
  
  /*
   * class Type - Represents a certain type of resource and all its contents
   */
  class Type {
   private: 
    // This is the number of bytes we use to initialize the buffer
    static constexpr size_t INIT_BUFFER_LENGTH = 16UL;
    
   public:
    // If the offset table entry has value like this then the entry 
    // does not exist
    // Note that this is 32 bit
    static constexpr uint32_t ENTRY_NOT_PRESENT = 0xFFFFFFFF;
  
   public: 
     
    // Original pointer to the header
    TypeHeader *header_p;
    
    // A pointer to the spec object (not the header!)
    TypeSpec *type_spec_p;
     
    // This stores readable names of the type
    // Note that we need to specify a length for buffer objects because by
    // default the buffer uses 64 KB internal storage on initialization
    // Note that this name does not have any type information, so when
    // use this to create directory we need to prepend the directory name
    //
    // Note: This is not always a valid C-String. If there is no special
    // configuration then it is empty buffer with length being 0
    //
    // Note 2: If this buffer is empty then it means this is the default
    // type config. Sometimes if we could not find a specific type config
    // we just resort to the default
    Buffer readable_name;
    
    // Number of entries in this type table
    size_t entry_count;
    
    // This points to the offset table
    uint32_t *offset_table;
    
    // Points to the resource entry indexed by the offset table
    unsigned char *data_p;
    
    /*
     * Constructor
     */
    Type() :
      header_p{nullptr},
      type_spec_p{nullptr},
      readable_name{INIT_BUFFER_LENGTH},
      entry_count{0UL},
      offset_table{nullptr},
      data_p{nullptr}
    {}
    
    /*
     * HasDefaultTypeConfig() - Whether the type config of this type object
     *                          is the default one
     *
     * We use the size of the readable name as an indicator
     */
    inline bool HasDefaultTypeConfig() const {
      return readable_name.GetLength() == 0UL;
    }
    
    /*
     * IsEntryPresent() - Whether an entry is present or not
     */
    inline bool IsEntryPresent(uint16_t entry_id) const {
      assert(entry_id < entry_count);
      
      return offset_table[entry_id] != ENTRY_NOT_PRESENT;
    }
    
    /*
     * GetEntryPtr() - Given the entry ID, return a pointer to the entry
     */
    inline ResourceEntry *GetEntryPtr(size_t entry_id) {
      assert(entry_id < entry_count);
      
      // Lookup the offset and get the pointer to it
      return reinterpret_cast<ResourceEntry *>(data_p + 
                                               offset_table[entry_id]);
    }
    
    /*
     * WriteXml() - Writes an XML file that represents the structure of the
     *              current type
     *
     * The XML file is written according to the type of the resource. We
     * hardcode the format for each type and its identifier, e.g. attr, string,
     * drawable, etc.
     */
    void WriteXml() {
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
    
    static constexpr const char *RES_PATH = "res";
    static constexpr const char *VALUE_PATH_PREFIX = "values";
    static constexpr const char *XML_SUFFIX = ".xml";
    
    /*
     * SwitchToValuesDir() - Switch to the values directory and opens the 
     *                       file for current base type
     *
     * This function takes care of possible postfix of "values"; The CWD
     * is not changed after returning. If file open or directory operations
     * returns error then exception is thrown
     */
    FILE *SwitchToValuesDir(const char *file_name) {
      // Save current directory first to get back after we have finished this 
      const char *cwd = FileUtility::GetCwd();
      
      // Enters 'res' first
      FileUtility::CreateOrEnterDir(RES_PATH);
      
      // So the total length we need is "values-" + length of the readable name
      // and if readable name is empty just omit the dash after "values"
      // so need 1 more bytes for '\0' and 1 byte for the possible '-'
      size_t value_path_length = strlen(VALUE_PATH_PREFIX) + \
                                 readable_name.GetLength() + \
                                 2;
      
      Buffer value_path{value_path_length};
      value_path.Append("values");
      // If there is a special name then append them also after the dash
      if(readable_name.GetLength() != 0UL) {
        value_path.Append('-');
        value_path.Append(readable_name); 
      }
      
      // Make it a valid C string
      value_path.Append('\0');
      
      // And then enters the dir or creates it if first time
      FileUtility::CreateOrEnterDir(value_path.GetCharData());

      FILE *fp = FileUtility::OpenFile(file_name, "w");
      
      // Frees current directory after switching back
      FileUtility::EnterDir(cwd);
      delete[] cwd;
      
      return fp;
    }
    
    /*
     * PrintAttrFormat() - Prints the format of attributes into the buffer
     */
    void PrintAttrFormat(Buffer *buffer_p, uint32_t format) {
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
    void WriteAttrEnumFlags(FILE *fp,
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
    size_t PrintAttrOther(ResourceEntryField *field_p,
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
    void WriteAttrXml(const char *file_name) {
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
     * HasNonStringDrawableEntry() - Whether the current type object contains
     *                               any non-string drawable entry
     *
     * If not return false and the caller should not create an XML file for it
     */
    bool HasNonStringDrawableEntry() {
      // We only print non-string entry
      size_t printable_entry_count = 0UL;
      
      for(size_t i = 0;i < entry_count;i++) {
        // Skip non-existing entries
        if(IsEntryPresent(i) == false) {
          continue; 
        }
        
        // This is the pointer to resource entry field
        ResourceEntry *entry_p = GetEntryPtr(i); 
        
        // We don't know how to deal with complex drawable entry
        if(entry_p->IsComplex() == true) {
          ReportError(INVALID_DRAWABLE_ENTRY, i);
        } else if(entry_p->value.type != ResourceValue::DataType::STRING) {
          printable_entry_count++; 
        }
      }
      
      // If the entry has nothing to print just return without creating the
      // path
      if(printable_entry_count == 0UL) {
#ifndef NDEBUG
        dbg_printf("Skip resource type \"");
        type_spec_p->GetBaseTypeName()->WriteToFile(stderr);
        if(HasDefaultTypeConfig() == false) {
          fputc('-', stderr);
          readable_name.WriteToFile(stderr); 
        }
        
        fprintf(stderr, "\" because it has no non-string entry\n");
#endif
        return false; 
      }
      
      // There is something to print
      return true;
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
    void WriteDrawableXml(const char *file_name) {
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
    
    // Store common array tag here
    static constexpr const char *ARRAY_TAG_LIST[] = {
      "string-array",
      "integer-array",
      "array",
    };
        
    static constexpr int STRING_ARRAY_TYPE_INDEX = 0;
    static constexpr int INTEGER_ARRAY_TYPE_INDEX = 1;
    static constexpr int OTHER_ARRAY_TYPE_INDEX = 2;
    
    /*
     * WriteArrayXml() - Writes arrays.xml file
     */
    void WriteArrayXml(const char *file_name) {
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
    void WriteBoolXml(const char *file_name) {
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
    void WriteIdXml(const char *file_name) {
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
    void WriteStyleXml(const char *file_name) {
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
    void WriteStringXml(const char *file_name) {
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
    void WriteDimenXml(const char *file_name) {
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
    
    // Writes the color tag line into file
    // Implemented in the CPP file
    void WriteColorTagLine(ResourceTable *table_p,
                           Package *package_p, 
                           ResourceEntry *entry_p,
                           Buffer *buffer_p,
                           FILE *fp);
    
    /*
     * WriteColorXml() - Writes a color XML file
     */
    void WriteColorXml(const char *file_name) {
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
     * WriteLayoutXml() - This file does not write layouts.xml file
     *                    but instead it decompiles the binary XML 
     *                    indicated inside resource entries
     */
    void ProcessLayoutXml() {
      // TO BE IMPLEMENTED
      return;
    }
    
    /*
     * WriteAnimXml() - Decompile the animation XML file recorded inside 
     *                  the entries
     */
    void ProcessAnimXml() {
      // TO BE IMPLEMENTED
      return;
    }
    
    /*
     * WriteXmlTypeXml() - Decompile the XML type XML file recorded inside 
     *                     the entries
     */
    void ProcessXmlTypeXml() {
      // TO BE IMPLEMENTED
      return;
    }
    
    /*
     * ProcessRawType() - Decompile the raw type file recorded inside 
     *                    the entries
     */
    void ProcessRawType() {
      // TO BE IMPLEMENTED
      return;
    }
  };
  
  /*
   * class TypeSpec - General type specification on configurations
   */
  class TypeSpec {
    static constexpr size_t BASE_TYPE_NAME_INIT_LENGTH = 16UL;
   public: 
    // This points to the type spec header
    TypeSpecHeader *header_p;
    
    // This points to the containing package instance (not the package header)
    Package *package_p;
    
    // Type ID - begins at 1, and 0 means invalid (so whenever we use this
    // to probe the string table we need to decrease it by 1)
    uint32_t type_id;
    
    // Number of entries in the table
    size_t entry_count;
    
    // Pointer to the configuration table about configurations of different
    // value instances (i.e. a bit field indicating which kind of resources 
    // are available)
    uint32_t *config_table;
    
    // Type values of different configurations
    std::vector<Type> type_list;
    
    // ASCII representation of the type name
    // This is used by many routines to identify a type because type ID
    // does not identify types across packages
    Buffer base_type_name;
    
    /*
     * Constructor
     */
    TypeSpec() :
      header_p{nullptr},
      package_p{nullptr},
      entry_count{0UL},
      config_table{nullptr},
      type_list{},
      base_type_name{BASE_TYPE_NAME_INIT_LENGTH}
    {}
    
    /*
     * GetBaseTypeName() - Returns the base type name buffer pointer
     */
    inline Buffer *GetBaseTypeName() {
      return &base_type_name;
    }
    
    /*
     * GetDefaultConfigType() - Returns the Type object pointer if it has
     *                          a default config
     *
     * If default config type does not exist then return nullptr which is
     * usually treated as an error
     */
    Type *GetDefaultConfigType() {
      for(size_t i = 0;i < type_list.size();i++) {
        if(type_list[i].HasDefaultTypeConfig() == true) {
          return &type_list[i];
        }
      }
      
      // Not found
      return nullptr;
    }
    
    /*
     * GetConfigType() - Given a configuration, returns its type pointer
     *
     * This function loops through all available type objects and matches 
     * the type config object. If the object is not found, nullptr is returned
     */
    Type *GetConfigType(const TypeConfig &type_config) {
      for(size_t i = 0;i < type_list.size();i++) {
        if(type_config == type_list[i].header_p->config) {
          return &type_list[i];
        }
      }
      
      return nullptr;
    }
  };
  
  /*
   * class Package - Represents internals of a package
   */
  class Package {
   public: 
    // Pointer to the package header
    PackageHeader *header_p;
    
    // These two are used to store headers
    StringPoolHeader *type_string_pool_header_p;
    StringPoolHeader *key_string_pool_header_p;
    
    // Two string pools indicated in the header
    StringPool type_string_pool;
    StringPool key_string_pool;
    
    // A list of type spec objects
    std::vector<TypeSpec> type_spec_list;
    
    // This points to the resource table
    ResourceTable *table_p;
    
    /*
     * GetTypeCount() - Returns the number of types defined as resources
     *
     * Since the number of strings in the string pool defines all types
     * we could treat this as the number of types
     */
    size_t GetTypeCount() const {
      return type_string_pool.string_count;
    }
  };

 // Data members  
 private:
  TableHeader *table_header_p;
  
  // This is a mapping between package ID and the package pointer stored
  // also inside this class. We have two access methods for packages: 
  // either through this mapping using the package ID or through the 
  // array of package objects
  std::unordered_map<uint8_t, Package *> package_map;
  
  // A list of packages
  // We do reserve space for this vector such that only 1 malloc() is done
  std::vector<Package> package_list;
 public:
  
  /*
   * Constructor
   */
  ResourceTable(unsigned char *p_raw_data_p, 
                size_t p_length, 
                bool p_own_data=false) :
    ResourceBase{p_raw_data_p, p_length, p_own_data},
    table_header_p{nullptr} {
    
    // Check whether all header fields are valid; if not just exit
    CommonHeader *next_header_p = VerifyTableHeader();  
    if(IsValidTable() == false) {
      table_header_p = nullptr;
      
      return; 
    }
    
    while(next_header_p != nullptr) {
      next_header_p = ParseNext(next_header_p);
    }
    
    assert(string_pool_header_p != nullptr);
    
    return;    
  }
  
  /*
   * Destructor
   */
  ~ResourceTable() {
    return; 
  }
  
  /*
   * GetResourceIdString() - Returns the string representation of a resource ID
   *
   * This is used in the resource file to refer to other resources. This 
   * function does not clear the buffer, so the buffer could contain some other
   * contents before calling this function
   */
  static void GetResourceIdString(ResourceId id, 
                                  const TypeConfig *type_config_p,
                                  Buffer *buffer_p) {
    // Maybe in the future we will need this
    (void)type_config_p;
    
    uint8_t package_id = id.package_id;
    uint8_t type_id = id.type_id;
    uint16_t entry_id = id.entry_id;
    
    uint8_t type_index = type_id - 1;
    uint16_t entry_index = entry_id;
    
    // Since this function is static we always use the global package group
    ResourceTable *table_p = package_group.GetResourceTable(package_id);
    auto it = table_p->package_map.find(package_id);
    if(it == table_p->package_map.end()) {
      ReportError(PACKAGE_ID_NOT_FOUND, static_cast<uint32_t>(package_id)); 
    }
    
    // This is the package pointer found
    Package *package_p = it->second;
    
    // Then verify whether the type ID is correct
    if(type_index >= package_p->GetTypeCount()) {
      ReportError(INVALID_TYPE_ID, static_cast<uint32_t>(type_id));
    }
    
    // This is a pointer to the type spec header
    TypeSpec *type_spec_p = &package_p->type_spec_list[type_index];
    
    // Then for each type in the type spec's list loop to find the most
    // appropriate one
    for(Type &type : type_spec_p->type_list) {
      if(entry_index >= type.entry_count) {
        dbg_printf("Entry index is greater than the"
                   " entry count of type instance\n");
        continue;
      }
      
      // Entry is not present - try next
      if(type.IsEntryPresent(entry_index) == false) {
        continue;
      }
      
      // This is the entry whose name we are using
      ResourceEntry *entry_p = type.GetEntryPtr(entry_index);
      
      if(package_id == 0x01) {
        buffer_p->Append("@android:");
      } else {
        buffer_p->Append('@');
      }
      
      buffer_p->Append(*type.type_spec_p->GetBaseTypeName());
      buffer_p->Append('/');
    
      // Append the name of the entry as the last component
      package_p->key_string_pool.AppendToBuffer(entry_p->key, buffer_p);
      
      return;
    }
    
    ReportError(RESOURCE_ID_NOT_AVAILABLE, id.data);
    
    return;
  }
  
  /*
   * GetResourceIdBaseTypeName() - Returns a buffer indicating the type name
   *                               of a certain resource ID
   *
   * Since type name is stored inside the TypeSpec class, we do not iterate 
   * over types, which is an optimization over the traditional 
   * GetResourceEntry()
   */
  static Buffer *GetResourceIdBaseTypeName(ResourceId id) {
    uint8_t package_id = id.package_id;
    uint8_t type_id = id.type_id;
    
    // Type index starts at 1 because we use type = 0 as invalid indicator
    uint8_t type_index = type_id - 1;
    
    // Since this function is static we always use the global package group
    ResourceTable *table_p = package_group.GetResourceTable(package_id);
    auto it = table_p->package_map.find(package_id);
    if(it == table_p->package_map.end()) {
      ReportError(PACKAGE_ID_NOT_FOUND, static_cast<uint32_t>(package_id)); 
    }
    
    // This is the package pointer found
    Package *package_p = it->second;
    
    // Then verify whether the type ID is correct
    if(type_index >= package_p->GetTypeCount()) {
      ReportError(INVALID_TYPE_ID, static_cast<uint32_t>(type_id));
    }
    
    // This is a pointer to the type spec header
    TypeSpec *type_spec_p = &package_p->type_spec_list[type_index];
    
    return &type_spec_p->base_type_name; 
  }
  
  /*
   * GetResourceEntry() - Returns the resource entry pointer given an ID
   *
   * Since each resource has a unique resource identifier we could locate them
   * using the identifier and print its name. Whether or not the resource
   * is a complex one does not matter since we return ResourceEntry pointer
   *
   * In the case that there are multiple resource types for different 
   * configurations, we need a type config object to match all possible
   * configurations, and in the case none matches, just fall back to the
   * default
   *
   * Note that this function only search the current resource table. If
   * the package ID is not in the current resource table then we need to
   * check external packages using package group instance
   *
   * This function also accepts an optional type pointer for getting the actual
   * type instance being used.
   *
   * If type_config is nullptr then we do not match type and always use
   * the default resource
   */
  static ResourceEntry *GetResourceEntry(ResourceId id, 
                                         const TypeConfig *type_config_p,
                                         Type **type_p_p = nullptr) {
    uint8_t package_id = id.package_id;
    uint8_t type_id = id.type_id;
    uint16_t entry_id = id.entry_id;
    
    uint8_t type_index = type_id - 1;
    uint16_t entry_index = entry_id;
    
    // Since this function is static we always use the global package group
    ResourceTable *table_p = package_group.GetResourceTable(package_id);
    auto it = table_p->package_map.find(package_id);
    if(it == table_p->package_map.end()) {
      ReportError(PACKAGE_ID_NOT_FOUND, static_cast<uint32_t>(package_id)); 
    }
    
    // This is the package pointer found
    Package *package_p = it->second;
    
    // Then verify whether the type ID is correct
    if(type_index >= package_p->GetTypeCount()) {
      ReportError(INVALID_TYPE_ID, static_cast<uint32_t>(type_id));
    }
    
    // This is a pointer to the type spec header
    TypeSpec *type_spec_p = &package_p->type_spec_list[type_index];
    // If the type spec list has no element then we know the type spec
    // has problem
    if(type_spec_p->type_list.size() == 0UL) {
      ReportError(NO_TYPE_IN_TYPE_SPEC, id.data);
    }
    
    // Use this to indicate whether we have already used the default type
    bool use_default_type = false;
    
    // make it nullptr to distinguish the case that there is no type config
    Type *type_p = nullptr;
    
    // Match the configure index if it is supplied
    if(type_config_p != nullptr) {
      // Try to match the type config - if not found just use the default one
      type_p = type_spec_p->GetConfigType(*type_config_p);
    }
    
    if(type_p == nullptr) {
      dbg_printf("Type config not matched - using default config type\n");
      
      // Get the default config type (usually has a buffer length of 0)
      type_p = type_spec_p->GetDefaultConfigType();
      use_default_type = true;
      
      if(type_p == nullptr) {
        ReportError(DEFAULT_CONFIG_TYPE_NOT_FOUND, 
                    static_cast<uint32_t>(type_id)); 
      }
    }
    
    // If entry ID is out of bound then just report error
    if(entry_index >= type_p->entry_count) {
      ReportError(INVALID_ENTRY_ID, entry_id); 
    } else if(type_p->IsEntryPresent(entry_index) == false) {
      
      if(use_default_type == false) {
        dbg_printf("Type config index matched but entry is not present"
                   " - using default config type\n");
        
        type_p = type_spec_p->GetDefaultConfigType();
        use_default_type = true;
        
        if(type_p == nullptr) {
          ReportError(DEFAULT_CONFIG_TYPE_NOT_FOUND, 
                      static_cast<uint32_t>(type_id)); 
        }
      } else {
        ReportError(ENTRY_NOT_PRESENT_IN_DEFAULT_TYPE, 
                    static_cast<uint32_t>(type_id), 
                    static_cast<uint32_t>(entry_id)); 
      }
      
      // After switching to default type if the entry is still not 
      // present then report error
      if(type_p->IsEntryPresent(entry_index) == false) {
        ReportError(ENTRY_NOT_PRESENT_IN_DEFAULT_TYPE, 
                    static_cast<uint32_t>(type_id), 
                    static_cast<uint32_t>(entry_id)); 
      }
      
      // After fetching the default type, if entry ID is in correct then 
      // report error
      if(entry_index >= type_p->entry_count) {
        ReportError(INVALID_ENTRY_ID, entry_id);
      }
    }
    
    // After this we have fetched the correct type object pointer and know
    // that the entry ID is valid
    
    // Also optionally output the type pointer actually being used because
    // there is no backward pointer inside the resource entry object
    if(type_p_p != nullptr) {
      *type_p_p = type_p;
    }
    
    return type_p->GetEntryPtr(entry_index);
  }
  
  /*
   * IsValidTable() - Whether the resource table is valid after verification
   */
  inline bool IsValidTable() const {
    return table_header_p != nullptr; 
  }
  
  /*
   * VerifyTableHeader() - Check fields in the table header
   *
   * If all check pass then return the byte next to table header. Otherwise 
   * return nullptr.
   *
   * This function always sets table_header_p to the first byte of the stream.
   * However if check fails the caller is responsible for setting the pointer
   * to nullptr to indicate that the resource table is not valid 
   *
   * This function has exactly the same structure as VerifyXmlHeader()
   */
  CommonHeader *VerifyTableHeader() {
    table_header_p = reinterpret_cast<TableHeader *>(raw_data_p);
    
    if(table_header_p->common_header.type != ChunkType::RESOURCE_TABLE) {
      dbg_printf("Resource table type 0x%X is wrong (expecting 0x%X)\n", 
                 static_cast<uint32_t>(table_header_p->common_header.type), 
                 static_cast<uint32_t>(ChunkType::RESOURCE_TABLE));
      
      return nullptr; 
    } else if(table_header_p->common_header.header_length != \
              sizeof(TableHeader)) {
      dbg_printf("Resource table length 0x%X is wrong (expecting 0x%lX)\n", 
                 table_header_p->common_header.header_length, 
                 sizeof(TableHeader));
      
      return nullptr;      
    } else if(table_header_p->common_header.total_length != length) {
      dbg_printf("XML total length 0x%X is wrong (expecting 0x%lX)\n", 
                 table_header_p->common_header.total_length, 
                 length);
      
      // We require that the entire document is part of the XML
      // Otherwise we are unable to decode the rest of it
      return nullptr; 
    }
    
    dbg_printf("Verified resource table header; package count = %u\n",
               table_header_p->package_count);
               
    // This serves as an optimization such that we only allocate once
    // for the parsing process
    package_list.reserve(table_header_p->package_count);
    
    // Return the next byte and cast it as common header for later parsing
    return reinterpret_cast<CommonHeader *>(
             TypeUtility::Advance(table_header_p, sizeof(TableHeader))); 
  }
  
  /*
   * InitPackage() - Initialize a package object
   *
   * This function initializes the type spec list with proper number
   * of slots to hold type spec, since we know the number of type spec objects
   * is exactly the size of the string pool
   */
  void InitPackage(Package *package_p, PackageHeader *package_header_p) {
    package_p->header_p = package_header_p;
    
    // Also assign the resource table's instance with the package
    package_p->table_p = this;
    
    package_p->type_string_pool_header_p = \
      reinterpret_cast<StringPoolHeader *>( \
        TypeUtility::Advance(package_header_p, 
                             package_header_p->type_string_pool_offset));
    
    ConstructStringPool( \
      reinterpret_cast<CommonHeader *>(package_p->type_string_pool_header_p), 
      &package_p->type_string_pool);
                        
    package_p->key_string_pool_header_p = \
      reinterpret_cast<StringPoolHeader *>( \
        TypeUtility::Advance(package_header_p, 
                             package_header_p->key_string_pool_offset));
    
    ConstructStringPool( \
      reinterpret_cast<CommonHeader *>(package_p->key_string_pool_header_p), 
      &package_p->key_string_pool);
    
    // This is done to only allocate exactly GetTypeCount() slots for type
    // spec objects (i.e. string pool size)
    package_p->type_spec_list.resize(package_p->GetTypeCount());
    
    return;
  }
  
  /*
   * DebugPrintPackageTypeString() - Prints all type strings in a package
   *
   * This function is meant for debugging
   */
  void DebugPrintPackageTypeString(Package *package_p) {
    dbg_printf("    Resource types: ");
    
    if(package_p->GetTypeCount() > 0) {
      package_p->type_string_pool.DebugPrint(0, "%s");
      // Print out all types in debug output; if debug is turned off this will
      // be optimized out                   
      for(size_t i = 1;i < package_p->GetTypeCount();i++) {
        fprintf(stderr, " | ");
        package_p->type_string_pool.DebugPrint(i, "%s");
      }
      
      fprintf(stderr, "\n");
    } else {
      fprintf(stderr, "[None]\n"); 
    }
    
    return;
  }
  
  /*
   * ParsePackage() - Parses the package header and push a package
   *                  object to the package list
   *
   * This function locates all type specs inside the package and then parses
   * all type headers after the type spec header. Each typespec stores metadata
   * about a type and each type object stores the resource value under a certain
   * configuration (e.g. language, screen resolution, etc.)
   */
  void ParsePackage(CommonHeader *header_p) {
    PackageHeader *package_header_p = \
      reinterpret_cast<PackageHeader *>(header_p);
      
    dbg_printf("    Package ID = 0x%02X\n", package_header_p->id);  
    
    // Even in Android runtime this is not taken care, so ....
    assert(package_header_p->type_string_pool_offset != 0);
    assert(package_header_p->key_string_pool_offset != 0);
    
    // Construct a packge object at the back of the vector
    // This saves the cost of copying the object
    package_list.emplace_back();
    // This points to the package object we just inserted
    Package *package_p = &package_list.back();
    
    // Initialize members inside the package object
    InitPackage(package_p, package_header_p);
    
    // Also put the package object into the vector (we do not support
    // non-base packages)
    if(package_p->header_p->id == 0x00000000) {
      ReportError(ONLY_SUPPORT_BASE_PACKAGE);  
    }
    
    package_map[static_cast<uint8_t>(package_p->header_p->id)] = package_p;
    
    // Also register the package within package group global object
    // using the package ID as well as the resource table instance
    package_group.RegisterPackage(package_header_p->id, this);
    
#ifndef NDEBUG
    DebugPrintPackageTypeString(package_p);
#endif

    // The first type spec chunk must be after the key string pool
    // so use its total size to determine (hopefully string pool is aligned)
    CommonHeader *type_spec_header_p = \
      reinterpret_cast<CommonHeader *>( \
        TypeUtility::Advance(
          package_p->key_string_pool_header_p, 
          package_p->key_string_pool_header_p->common_header.total_length));
     
    // Each type will have a type spec chunk, so just use the number of 
    // elements in type string pool     
    for(size_t i = 0;i < package_p->GetTypeCount();i++) {
      uint32_t type_id = ParseTypeSpecHeader(type_spec_header_p, package_p);
      
      // Use its length field to find the following type spec chunk
      type_spec_header_p = \
        TypeUtility::Advance(type_spec_header_p, 
                             type_spec_header_p->total_length);

      // Then loop to parse types
      // Note that here everytime we speculate a type header we need to
      // check whether the pointer is still in the range of the data area
      while(IsValidPointer(type_spec_header_p) == true && \
            type_spec_header_p->type == ChunkType::TYPE) {
        ParseTypeHeader(type_spec_header_p, package_p, type_id);
        
        type_spec_header_p = \
          TypeUtility::Advance(type_spec_header_p, 
                               type_spec_header_p->total_length);
      }
    }
                        
    return;
  }
  
  /*
   * ParseTypeSpecHeader() - Parses type specification header and
   *                         returns the type ID
   */
  uint32_t ParseTypeSpecHeader(CommonHeader *header_p, Package *package_p) {
    dbg_printf("Parsing TypeSpec chunk @ offset 0x%lX\n", 
               TypeUtility::GetPtrDiff(raw_data_p, header_p));
    assert(header_p->type == ChunkType::TYPE_SPEC);
    
    TypeSpecHeader *type_spec_header_p = \
      reinterpret_cast<TypeSpecHeader *>(header_p);
    
    // Get the type ID which also represents its position in the vector
    // NOTE: The real ID is always 1 less then the recodrd ID
    uint32_t type_id = static_cast<uint32_t>(type_spec_header_p->id);
    
    // It could not be 0 and also could not exceed the maximum
    if(type_id == 0 || type_id > package_p->GetTypeCount()) {
      ReportError(INVALID_TYPE_ID, type_id);
    }
    
    // This is the type spec object already allocated on the type spec list
    TypeSpec *type_spec_p = &package_p->type_spec_list[type_id - 1];
    
    // Assert the type has never be seen
    assert(type_spec_p->header_p == nullptr);
    assert(type_spec_p->type_list.size() == 0);
    
    // Assign data members
    type_spec_p->header_p = type_spec_header_p;
    type_spec_p->package_p = package_p; 
    type_spec_p->type_id = type_id;
    type_spec_p->entry_count = type_spec_header_p->entry_count;
    type_spec_p->config_table = type_spec_header_p->data;
    
    package_p->type_string_pool.AppendToBuffer(type_id - 1,
                                               &type_spec_p->base_type_name);

    return type_id;
  }

  /*
   * DebugPrintAllTypeEntryNames() - Prints on stderr names of all entries in the
   *                                 type chunk body
   */
  void DebugPrintAllTypeEntryNames(Package *package_p, Type *type_p) {
	  for(size_t i = 0;i < type_p->entry_count;i++) {
      // Resource entry does not exist for current configuration
      if(type_p->IsEntryPresent(i) == false) {
        continue;
      }

      // This is a pointer to the resource entry
      ResourceEntry *resource_entry_p = type_p->GetEntryPtr(i);

      dbg_printf("        Name %lu = ", i);
      Buffer buffer{128};

      package_p->key_string_pool.AppendToBuffer(resource_entry_p->key, &buffer);

      if(resource_entry_p->IsComplex() == true || \
         resource_entry_p->IsPublic() == true) {
        buffer.Append(" (");
      }

      // For complex types it has two more fields - parent resource ID and
      // count of the key value pair that follows
      if(resource_entry_p->IsComplex() == true) {
        assert(resource_entry_p->entry_length == 16UL);
        buffer.Append("COMPLEX ");
      } else {
        assert(resource_entry_p->entry_length == 8UL);
      }

      if(resource_entry_p->IsPublic() == true) {
        buffer.Append("PUBLIC ");
      }

      if(resource_entry_p->IsComplex() == true || \
         resource_entry_p->IsPublic() == true) {
        // Eat back the last space character
        buffer.Rewind(1);
        buffer.Append(')');
      }

      buffer.Append('\n');
      buffer.WriteToFile(stderr);

      // In the next line print out the extra complex field
      if(resource_entry_p->IsComplex() == true) {
        dbg_printf("            * Parent ID = 0x%X; entry count = %u\n",
                   resource_entry_p->parent_id.data,
                   resource_entry_p->entry_count);

        // This is the starting address of the array of ResourceEntryField
        ResourceEntryField *entry_field_p = resource_entry_p->field_data;

        // Loop through each entry to see its internal data
        for(uint32_t j = 0;j < resource_entry_p->entry_count;j++) {
          // This is the current entry filed being processed
          ResourceEntryField *field_p = entry_field_p + j;
          
          // Print out the 32 bit integer resource ID
          dbg_printf("            "
                     "entry name = 0x%.8X; type = 0x%.4X, data = 0x%.8X\n",
                     field_p->name.data,
                     static_cast<uint32_t>(field_p->value.type),
                     field_p->value.data);
          
          buffer.Reset();
          // Append value but do not resolve reference
          AppendResourceValueToBuffer(&field_p->value, &buffer, nullptr, false);
          dbg_printf("                Printed value \"");
          buffer.Append("\"\n");
          buffer.WriteToFile(stderr);
          
          // If the type ID is not attr then the field name must have
          // a ATTR type ID
          //if(type_p->type_spec_p->type_id != 0x01) {
          //  assert(field_p->name.type_id == 0x01);
          //}
        } // Loop through entry fields
      } else {
        dbg_printf("            "
                   "entry type = 0x%.4X, data = 0x%.8X\n",
                   static_cast<uint32_t>(resource_entry_p->value.type),
                   resource_entry_p->value.data);
        
        buffer.Reset();
        AppendResourceValueToBuffer(&resource_entry_p->value, 
                                    &buffer, 
                                    nullptr, 
                                    false);
        dbg_printf("            Printed value \"");
        buffer.Append("\"\n");
        buffer.WriteToFile(stderr);
      } // If is complex type then ... else ...
	  } // for resource entry for the current type

    return;
  }
  
  /*
   * DebugWriteTypeXml() - Writes a XML file for a given type
   */
  void DebugWriteTypeXml(Type *type_p) {
    type_p->WriteXml();
    
    return;
  }

  /*
   * ParseTypeHeader() - Parses type header
   *
   * Note that the ID of the type recorded in the type spec header is
   * passed in to verify that the type header has the same ID
   */
  void ParseTypeHeader(CommonHeader *header_p, 
                       Package *package_p, 
                       uint32_t type_id) {
    TypeHeader *type_header_p = \
      reinterpret_cast<TypeHeader *>(header_p);
    
    // This points to the type spec object
    TypeSpec *type_spec_p = &package_p->type_spec_list[type_id - 1];
    
    // Construct a new type object in-place and grab a pointer to it
    type_spec_p->type_list.emplace_back();
    Type *type_p = &type_spec_p->type_list.back();
    
    type_p->header_p = type_header_p;
    type_p->type_spec_p = type_spec_p;
    type_header_p->config.GetName(&type_p->readable_name);
    type_p->entry_count = type_header_p->entry_count;
    
    // The offset table is just located after the header
    // Note that since we are not certain about the length of the config
    // structure, currently just use header length to determine the end
    // of the header
    type_p->offset_table = reinterpret_cast<uint32_t *>( \
      TypeUtility::Advance(type_header_p, 
                           type_header_p->common_header.header_length));
    
    // This is the base address of offset_table and its offset itself is
    // specified by a header field
    type_p->data_p = reinterpret_cast<unsigned char *>( \
      TypeUtility::Advance(type_header_p, 
                           type_header_p->data_offset));
    
    // The ID must match
    assert(static_cast<uint32_t>(type_header_p->type_id) == type_id);
    
    return;
  }
  
  /*
   * IsValidPointer() - Checks whether a pointer is still in the range of the 
   *                    data area
   */
  inline bool IsValidPointer(void *p) const {
    return TypeUtility::GetPtrDiff(raw_data_p, p) < length;
  }
  
  /*
   * ParseNext() - Parse the contents of the resource table
   *
   * This function has the same structure as the one in class BinaryXml
   *
   * If we have reached the end then just return nullptr and that's it
   */ 
  CommonHeader *ParseNext(CommonHeader *next_header_p) {
    assert(next_header_p != nullptr);
    
    if(IsValidPointer(next_header_p) == false) {
      return nullptr;
    }
    
    CommonHeader *ret_header_p = \
      TypeUtility::Advance(next_header_p, next_header_p->total_length);
    
    dbg_printf("Parsing header of type %u @ offset 0x%lX\n",
               (uint32_t)next_header_p->type,
               TypeUtility::GetPtrDiff(raw_data_p, next_header_p));
    
    switch(next_header_p->type) {
      case ChunkType::RESOURCE_TABLE: {
        ReportError(UNEXPECTED_START_OF_RESOURCE_TABLE);
        break;
      } 
      case ChunkType::STRING_POOL: {
        ParseStringPool(next_header_p);
        break;
      } 
      case ChunkType::PACKAGE: {
        // This function also needs to recognize the end of data
        ParsePackage(next_header_p);
        break; 
      }
      default: {
        ReportError(UNKNOWN_CHUNK_TYPE, 
                    static_cast<uint32_t>(next_header_p->type),
                    (size_t)next_header_p - (size_t)raw_data_p); 
        break;
      }
    } // switch type
    
    return ret_header_p;
  }
  
  /*
   * DebugPrintAll() - Prints everything in this resource table
   */
  void DebugPrintAll() {
    for(Package &package : package_list) {
      for(TypeSpec &type_spec : package.type_spec_list) {
        TypeSpecHeader *type_spec_header_p = type_spec.header_p;
        dbg_printf("Type id = %u; entry_count = %u (type name = ", 
                   static_cast<uint32_t>(type_spec_header_p->id),
                   static_cast<uint32_t>(type_spec_header_p->entry_count));
    
        Buffer buffer{128};
        package.type_string_pool.AppendToBuffer(type_spec.type_id - 1, &buffer);
        buffer.Append(")\n");
        buffer.WriteToFile(stderr);
        
        // Here we try to find default value every type we need
        bool default_found = false;
        
        for(Type &type : type_spec.type_list) {
          dbg_printf("    Type config name = ");
          
          if(type.readable_name.GetLength() == 0UL) {
            fprintf(stderr, "[Default]\n");
            default_found = true;  
          } else {
            type.readable_name.WriteToFile(stderr);
            fputc('\n', stderr);
          }
          
          dbg_printf("    Entry count = %lu\n", type.entry_count);
          
          DebugPrintAllTypeEntryNames(&package, &type);
        }
        
        // Print as a warning
        if(default_found == false) {
          dbg_printf("*** No default type found for typespec (ID = %u)!\n",
                     type_spec.type_id);
        }
      }
    }
    
    return;
    
          
  }
  
  /*
   * DebugWriteXml() - Writes XML into the corresponding file under /res folder
   */
  void DebugWriteXml() {
    for(Package &package : package_list) {
      for(TypeSpec &type_spec : package.type_spec_list) {        
        for(Type &type : type_spec.type_list) {
          type.WriteXml();
        }
      }
    }
    
    return;
  }
};

} // namespace android_dalvik_analysis
} // namespace wangziqi2013

#endif
