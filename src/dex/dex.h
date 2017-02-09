
#pragma once

#ifndef _DEX_H
#define _DEX_H

#include "dex_common.h"

namespace wangziqi2013 {
namespace android_dalvik_analysis { 
namespace dex {

/*
 * class DexFile - The Dalvik bytecode container file
 *
 * This file is responsible for decoding the dalvik format headers and directory
 * for byte code disassembly there is another class dedicated to that
 */
class DexFile {
 private:
  
  /* 
   * enum class FileMapItemType - The item type inside file map
   */
  enum class FileMapItemType : uint16_t {
    DEX_HEADER = 0x0,
    STRING_ID,
    TYPE_ID,
    PROTOTYPE_ID,
    FIELD_ID,
    METHOD_ID,
    CLASS_DEF,
    MAP_LIST = 0x1000,
    TYPE_LIST,
    ANNOTATION_LIST,
    ANNOTATION_REF,
    CLASS_DATA_ITEM = 0x2000,
    CODE,
    STRING_DATA,
    DEBUG_INFO,
    ANNOTATION,
    ENCODED_ARRAY,
    ANNOTATION_DIR,
  };
  
  /*
   * FileMapItem - An entry in file map
   */
  class FileMapItem {
   public:
    FileMapItemType type;
    uint16_t unused;
    
    uint32_t length;
    uint32_t offset;
  } BYTE_ALIGNED;
  
  /*
   * FileMap - A map of the structure of the file
   */
  class FileMap {
   public: 
    uint32_t entry_count;
    
    // This points to the first element of items
    FileMapItem item_list[0];
  } BYTE_ALIGNED;
  
  /*
   * class FileHeader - Describes the general information about the file
   */ 
  class FileHeader {
   public:
     
    // This serves as the starting address of the array
    unsigned char start[0];
    
    // File signature - should be "dex\n037\0"
    char magic[8];
    
    // Alder32 checksum - we do not rely on this
    uint32_t checksum;
    
    // 20 byte SHA-1 signature of the entire file
    uint8_t signature[20];
    
    // Must equal actual file size
    uint32_t file_length;
    // Must equal 0x70 because header is of fixed length
    uint32_t header_length;
    // For little-endian it should be 0x12345678
    uint32_t endian_tag;
    
    // Note that in all fields below:
    //   "size" is the number of elements
    //   "length" is the number of bytes a section occupy
    //   "offset" is the offset from the beginning of the header (i.e. also
    //      from the beginning of the file)
    
    // This is never presented in normal APKs, so do not need to worry about it
    uint32_t link_length;
    uint32_t link_offset;
    
    uint32_t map_offset;
    
    uint32_t string_ids_size;
    uint32_t string_ids_offset;
    
    uint32_t type_ids_size;
    uint32_t type_ids_offset;
    
    uint32_t proto_ids_size;
    uint32_t proto_ids_offset;
    
    uint32_t field_ids_size;
    uint32_t field_ids_offset;
    
    uint32_t method_ids_size;
    uint32_t method_ids_offset;
    
    uint32_t class_defs_size;
    uint32_t class_defs_offset;
    
    uint32_t data_length;
    uint32_t data_offset;
  } BYTE_ALIGNED;
  
  /* 
   * class TypeList - Records a list of types
   *
   * This is used to represent function parameters
   */
  class TypeList {
   public: 
    // Number of entries 
    uint32_t entry_count;
    ShortTypeId type_list[0]; 
    
    /*
     * begin() - Returns begin iterator
     */
    inline ShortTypeId *begin() {
      return type_list;  
    }
    
    /*
     * begin() - Returns end iterator
     */
    inline ShortTypeId *end() {
      return type_list + entry_count;
    }
    
    /*
     * begin() - Returns constant begin iterator
     */
    inline const ShortTypeId *begin() const {
      return type_list; 
    }
    
    /*
     * begin() - Returns constant end iterator
     */
    inline const ShortTypeId *end() const {
      return type_list + entry_count;
    }
  } BYTE_ALIGNED;
  
  /*
   * class ProtoIdItem - Prototype items
   */
  class ProtoIdItem {
   public:
    // Shortened form of the prototype
    StringId name_id;  
    TypeId return_type_id;
    
   private: 
    // This is an offset to the file header, and the target is a 
    // TypeList instance
    uint32_t type_list_offset;
    
   public: 
    /*
     * HasTypeList() - Whether there is a type list as parameter list
     */
    inline bool HasTypeList() const {
      return type_list_offset != NO_OFFSET;
    }
    
    /*
     * GetTypeList() - Returns a pointer to the type list representing the 
     *                 parameter list
     */
    inline TypeList *GetTypeList(FileHeader *header_p) {
      assert(HasTypeList() == true);
      
      return reinterpret_cast<TypeList *>(header_p->start + type_list_offset);
    }
    
    /*
     * GetatgumentType() - Returns the type of a certain argument
     */
    inline TypeId GetArgumentType(uint32_t index, FileHeader *header_p) {
      TypeList *type_list_p = GetTypeList(header_p);
      assert(index < type_list_p->entry_count);
      
      return type_list_p->type_list[index];
    }
  } BYTE_ALIGNED;
  
  /*
   * class FieldIdItem - Field information
   */
  class FieldIdItem {
   public:
    // Type of the containing class
    ShortTypeId class_type_id;
    // Type of the field
    ShortTypeId field_type_id;
    // Sting of the name of the field
    StringId name_id;
  } BYTE_ALIGNED;
  
  /*
   * class MethodIdItem - Represents a method
   */
  class MethodIdItem {
   public:
    ShortTypeId class_type_id;
    ShortProtoId proto_id;
    
    StringId name_id; 
  } BYTE_ALIGNED;
  
  /*
   * class ClassDefItem - Class definition
   */
  class ClassDefItem {
   public:
    // The type ID of the current class
    TypeId type_id;
    
    // This is a bit mask about the access status of the class
    uint32_t access_flags;
    
   // The following is declared as private to 
   private:  
    // Super class's type ID; could be NO_INDEX if it has no parent type ID
    TypeId parent_type_id;
   
    // Offset from the beginning of the file to a list of
    // interfaces (class TypeList instance)
    // If equals 0 then this class does not have any interface
    uint32_t interface_offset;
    
    // A string about the source file's file name. Could be NO_INDEX
    StringId source_file_id;
    
    // Offset from the file header to annotations for this class
    uint32_t annotation_offset;
    
    // Offset to class data
    uint32_t data_offset;
    
    // Offset to class static value
    uint32_t static_value_offset;
    
   public:
     
    /*
     * HasParentType() - Whether the class has a parent class
     */
    inline bool HasParentTypeId() const {
      return parent_type_id != NO_INDEX;
    }
    
    /*
     * GetParentTypeId() - Returns the parent ID if there is one
     */
    inline TypeId GetParentTypeId() const {
      assert(HasParentTypeId() == true);
      
      return parent_type_id; 
    }
    
    /*
     * HasInterfaceTypeList() - Returns whether there is an interface list
     */
    inline bool HasInterfaceTypeList() const {
      return interface_offset != NO_OFFSET; 
    }
    
    /*
     * GetInterfaceTypeList() - Returns the interface type list
     */
    TypeList *GetInterfaceTypeList(FileHeader *header_p) {
      assert(HasInterfaceTypeList() == true);
      
      return reinterpret_cast<TypeList *>(header_p->start + interface_offset);
    }
    
    /*
     * HasData() - Whether the class has any class data
     */
    inline bool HasData() const {
      return data_offset != NO_OFFSET;
    }
    
    /*
     * GetData() - Returns a pointer to class data field
     */
    inline uint8_t *GetData(FileHeader *header_p) {
      assert(HasData() == true);
      
      return header_p->start + data_offset;
    }
  } BYTE_ALIGNED;
  
  // This is defined by the header itself
  static constexpr size_t FILE_HEADER_LENGTH = 0x70UL; 
  static constexpr uint32_t LITTLE_ENDIAN_TAG = 0x12345678; 
  
  static const char FILE_SIGNATURE_037[8];
  static const char FILE_SIGNATURE_035[8];
  
  static const char *FILE_SIGNATURE_LIST[2];
  
 public:
  
  unsigned char *data_p;
  size_t length;
  bool own_data;
  
  FileHeader *header_p;
  FileMap *file_map_p;
  
  // This is a list of strings that are represented in a special form of
  // UTF-8
  // For now we only support ASCII string
  std::vector<MUtf8String> string_list;
  
  // This is an array of uint32_t offsets into the string table for denoting 
  // types. We could just use the ID as the type's identity
  TypeId *type_list;
  
  // This is an array of prototypes
  ProtoIdItem *proto_list;
  
  // An array of class FieldIdItem
  FieldIdItem *field_list;
  
  // An array of class MethodIdItem
  MethodIdItem *method_list;
  
  // A list of decoded class instances that store class metadata
  // This is different from the wrapper class becuase it contains
  // all data in a class in an easy to use manner
  std::vector<ClassInfo> class_list;
  
 public: 
  /*
   * Constructor
   */
  DexFile(unsigned char *p_data_p, 
          size_t p_length,
          bool p_own_data) :
    data_p{p_data_p},
    length{p_length},
    own_data{p_own_data} {
    VerifyFileHeader();
    
    ParseStringIds();
    ParseTypeIds();
    ParseProtoIds();
    ParseFieldIds();
    ParseMethodIds();
    ParseClassDefs();
    
    return;
  }  
  
  /*
   * Destructor
   *
   * Note that we only destroy the data array if ownership is transferred to 
   * the instance
   */
  ~DexFile() {
    if(own_data == true) {
      delete[] data_p;
    }
    
    return;
  }
  
  /*
   * VerifyFileHeader() - Verifies the file header for some consistency 
   *                      conditions
   *
   * This function also sets up multiple pointers for later use
   */
  void VerifyFileHeader() {
    header_p = reinterpret_cast<FileHeader *>(data_p);
    
    // File size in the header and actual size must match
    if(header_p->file_length != length) {
      ReportError(WRONG_DEX_HEADER, "Inconsistent file size"); 
    }
    
    if(header_p->header_length != FILE_HEADER_LENGTH) {
      ReportError(WRONG_DEX_HEADER, "Inconsistent DEX header size"); 
    }
    
    if(header_p->endian_tag != LITTLE_ENDIAN_TAG) {
      ReportError(WRONG_DEX_HEADER, "Unknown endian tag"); 
    }
    
    // Since there are many possible signatures we need to enumerate
    bool magic_verified = false;
    for(size_t i = 0;i < sizeof(FILE_SIGNATURE_LIST) / sizeof(void *);i++) {
      if(memcmp(FILE_SIGNATURE_LIST[i], 
                header_p->magic, 
                sizeof(header_p->magic)) == 0) {
        magic_verified = true;
      }  
    }
    
    if(magic_verified == false) {
      ReportError(WRONG_DEX_HEADER, "Invalid file signature");  
    }
    
    // Link section should not be present in a normal DEX file
    if(header_p->link_length != 0 || header_p->link_offset != 0) {
      ReportError(WRONG_DEX_HEADER, "Do not support link section");
    }
    
    // Assign pointer to file map 
    file_map_p = \
      reinterpret_cast<FileMap *>(header_p->start + header_p->map_offset);
    
    return;
  }
  
  /*
   * GetStringCount() - Returns the number of strings
   */
  inline size_t GetStringCount() const {
    return header_p->string_ids_size; 
  }
  
  /*
   * GetTypeCount() - Returns the number of types
   */
  inline size_t GetTypeCount() const {
    return header_p->type_ids_size; 
  }
  
  /*
   * GetProtoCount() - Returns the number of protos
   */
  inline size_t GetProtoCount() const {
    return header_p->proto_ids_size; 
  }
  
  /*
   * GetFieldCount() - Returns the number of fields
   */
  inline size_t GetFieldCount() const {
    return header_p->field_ids_size; 
  }
  
  /*
   * GetMethodCount() - Returns the number of methods
   */
  inline size_t GetMethodCount() const {
    return header_p->method_ids_size;
  }
  
  inline size_t GetClassCount() const {
    return header_p->class_defs_size; 
  }
  
  /*
   * ParseStringIds() - Parses the string pool and stores the starting address
   *                    of strings in the list
   *
   * This function for now only supports ASCII strings, which happens to be 
   * the case for DEX if only normal characters are used.
   *
   * All strings are compatible to C string if this function passes. Otherwise
   * we have an abnormal case and may need human intervention
   */
  void ParseStringIds() {
    uint32_t *string_ids_p = \
      reinterpret_cast<uint32_t *>(header_p->start + 
                                   header_p->string_ids_offset);
    
    // reserve that many slots for MUtf8 instance
    string_list.reserve(header_p->string_ids_size);
    
    for(uint32_t i = 0;i < header_p->string_ids_size;i++) {
      uint32_t offset = string_ids_p[i];
      
      // Use the string offset to construct a MUtf8String instance
      // The instance will decode ULEB128 length field and adjust
      // the pointer to the correct location
      string_list.emplace_back(MUtf8String{header_p->start + offset});
    }
    
    return;
  }
  
  /*
   * ParseTypeIds() - Parse type list
   */
  inline void ParseTypeIds() {
    type_list = \
      reinterpret_cast<TypeId *>(header_p->start + 
                                 header_p->type_ids_offset);
                                   
    return;
  }
  
  /*
   * ParseProtoIds() - Sets the pointer pointing to the proto list
   */
  inline void ParseProtoIds() {
    proto_list = \
      reinterpret_cast<ProtoIdItem *>(header_p->start + 
                                      header_p->proto_ids_offset);
                                    
    return;
  }
  
  /*
   * ParseFieldIds() - Sets the pointer pointing to field list
   */
  inline void ParseFieldIds() {
    field_list = \
      reinterpret_cast<FieldIdItem *>(header_p->start + 
                                      header_p->field_ids_offset);

    return;
  }
  
  /*
   * ParseMethodIds() - Sets the pointer pointing to method list
   */
  inline void ParseMethodIds() {
    method_list = \
      reinterpret_cast<MethodIdItem *>(header_p->start + 
                                       header_p->method_ids_offset);

    return;
  }
  
  /*
   * ParseEncodedFieldList() - Parses encoded field list
   *
   * This function returns the data pointer after parsing the list
   */
  uint8_t *ParseEncodedFieldList(std::vector<FieldInfo> *field_list_p, 
                                 uint8_t *p, 
                                 uint32_t count) {
    // Since type ID is encoded in a differntial fomat we need
    // to accumulate it on a counter each time
    TypeId type_id = 0x00000000;
    for(uint32_t i = 0;i < count;i++) {
      FieldInfo *info_p = &field_list_p->at(i);
      
      info_p->id = (type_id += EncodingUtility::ReadLEB128(&p));
      info_p->access_flags = EncodingUtility::ReadLEB128(&p);
    }
    
    return p;
  }
  
  /*
   * ParseEncodedMethodList() - Parses encoded method list metadata
   */
  uint8_t *ParseEncodedMethodList(std::vector<MethodInfo> *method_list_p, 
                                  uint8_t *p, 
                                  uint32_t count) {
    MethodId method_id = 0x00000000;
    for(uint32_t i = 0;i < count;i++) {
      MethodInfo *info_p = &method_list_p->at(i);
      
      info_p->id = (method_id += EncodingUtility::ReadLEB128(&p));
      info_p->access_flags = EncodingUtility::ReadLEB128(&p);
      
      // Stores an absolute pointer to the code section
      uint32_t code_offset = EncodingUtility::ReadLEB128(&p);
      if(code_offset == NO_OFFSET) {
        info_p->code_p = nullptr;
      } else {
        info_p->code_p = header_p->start + code_offset;
      }
      
      // TODO: PARSE CODE SECTION HERE
    }
    
    return p;
  }
  
  /*
   * VerifyClassData() - Makes sure all methods and fields actually referes to
   *                     the containing class
   *
   * This function always returns true if verification succeeds
   */
  bool VerifyClassData(ClassInfo *info_p) {
    for(const FieldInfo &info : info_p->static_field_list) {
      TypeId class_id = \
        static_cast<TypeId>(field_list[info.id].class_type_id);
      
      if(class_id != info_p->id) {
        ReportError(INCORRECT_FIELD_DATA);
      }
    }
    
    for(const FieldInfo &info : info_p->instance_field_list) {
      TypeId class_id = \
        static_cast<TypeId>(field_list[info.id].class_type_id);
      
      if(class_id != info_p->id) {
        ReportError(INCORRECT_FIELD_DATA);
      }
    }
    
    for(const MethodInfo &info : info_p->direct_method_list) {
      TypeId class_id = \
        static_cast<TypeId>(method_list[info.id].class_type_id);
      
      if(class_id != info_p->id) {
        ReportError(INCORRECT_METHOD_DATA);
      }
    }
    
    for(const MethodInfo &info : info_p->virtual_method_list) {
      TypeId class_id = \
        static_cast<TypeId>(method_list[info.id].class_type_id);
      
      if(class_id != info_p->id) {
        ReportError(INCORRECT_METHOD_DATA);
      }
    }
    
    return true;
  }
  
  /*
   * ParseClassData() - Parses class field and method metadata and store
   *                    them into class ClassInfo
   */
  void ParseClassData(ClassInfo *class_p, uint8_t *class_data_p) {
    // It will move class data pointer
    uint32_t static_field_count = EncodingUtility::ReadLEB128(&class_data_p); 
    uint32_t instance_field_count = EncodingUtility::ReadLEB128(&class_data_p);
    uint32_t direct_method_count = EncodingUtility::ReadLEB128(&class_data_p);
    uint32_t virtual_method_count = EncodingUtility::ReadLEB128(&class_data_p);
    
    // Optimization: Allocate a chunk of memory and extend the vector first
    // Such that we could directly access internal data using index
    class_p->static_field_list.resize(static_field_count);
    class_p->instance_field_list.resize(instance_field_count);
    class_p->direct_method_list.resize(direct_method_count);
    class_p->virtual_method_list.resize(virtual_method_count);
    
    class_data_p = \
      ParseEncodedFieldList(&class_p->static_field_list, 
                            class_data_p, 
                            static_field_count);
                            
    class_data_p = \
      ParseEncodedFieldList(&class_p->instance_field_list, 
                            class_data_p, 
                            instance_field_count);
                            
    class_data_p = \
      ParseEncodedMethodList(&class_p->direct_method_list, 
                             class_data_p, 
                             direct_method_count);
                            
    class_data_p = \
      ParseEncodedMethodList(&class_p->virtual_method_list, 
                             class_data_p, 
                             virtual_method_count);
    
    // This only has effect under debug mode
    assert(VerifyClassData(class_p) == true);
    
    return;
  }
  
  /*
   * ParseClassDefs() - Parse class definition and stores class object
   */
  void ParseClassDefs() {
    ClassDefItem *item_p = \
      reinterpret_cast<ClassDefItem *>(header_p->start + \
                                       header_p->class_defs_offset);
    
    // To avoid excessive memory allocation because we already know the 
    // size of this array
    class_list.reserve(GetClassCount());
     
    // Go over all class defs and then parse each part                                  
    for(uint32_t i = 0;i < GetClassCount();i++) {
      class_list.emplace_back();
      ClassInfo *class_p = &class_list.back();
      
      class_p->id = item_p->type_id;
      
      // Assign parent type ID if there is one
      class_p->has_parent = item_p->HasParentTypeId();
      if(class_p->has_parent == true) {
        class_p->parent_id = item_p->GetParentTypeId();
      }
      
      // If there is an interface type list then we just copy it to
      // the class also
      if(item_p->HasInterfaceTypeList() == true) {
        TypeList *type_list_p = item_p->GetInterfaceTypeList(header_p);
        class_p->interface_list.reserve(type_list_p->entry_count);
        
        // then push all type IDs into the vector
        for(ShortTypeId id : *type_list_p) {
          class_p->interface_list.push_back(id); 
        }
      }
      
      // If there is class data defined then parse them
      if(item_p->HasData() == true) {
        ParseClassData(class_p, item_p->GetData(header_p)); 
      }
      
      // TODO: PARSE ANNOTATION AND STATIC DATA
      
      // Move to the next class item for parsing
      item_p++;
    }
    
    return;
  }
  
  /*
   * DebugPrintString() - Prints string into a buffer
   */
  inline void DebugPrintString(uint32_t index, Buffer *buffer_p) {
    string_list[index].PrintAscii(buffer_p);
    
    return;
  }
  
  /*
   * DebugPrintTypeString() - Prints the string representation of a type
   *
   * The index argument refers to the type list
   */
  inline void DebugPrintTypeString(uint32_t index, Buffer *buffer_p) {
    DebugPrintString(type_list[index], buffer_p);
    return;
  }
  
  /*
   * DebugPrinAlltStrings() - Prints out all strings inside the DEX
   */
  void DebugPrintAllStrings(FILE *fp) {
    Buffer buffer; 
    
    StringId i = 0;
    for(MUtf8String &s : string_list) {
      dbg_printf("String %u: ", i);
      
      s.PrintAscii(&buffer);
      buffer.WriteLineReset(fp);
      
      i++;
    }
    
    return;
  }
  
  /*
   * DebugPrintAllTypes() - Prints all types
   */
  void DebugPrintAllTypes(FILE *fp) {
    Buffer buffer;
    for(TypeId i = 0;i < GetTypeCount();i++) {
      uint32_t id = type_list[i];
      
      dbg_printf("Type %u: \"", i);
      DebugPrintString(id, &buffer);
      buffer.WriteLineReset(fp, "\"");
    }
    
    return;
  }
  
  /*
   * DebugPrintAllProtos() - Prints all prototypes
   */
  void DebugPrintAllProtos(FILE *fp) {
    Buffer buffer;
    
    for(ProtoId i = 0;i < GetProtoCount();i++) {
      ProtoIdItem *item_p = &proto_list[i];
      
      dbg_printf("Proto: \"");
      DebugPrintString(item_p->name_id, &buffer);
      buffer.WriteLineReset(fp, "\"");
      
      dbg_printf("    Return type: ");
      DebugPrintTypeString(item_p->return_type_id, &buffer);
      buffer.WriteLineReset(fp);
      
      // There is no argument
      if(item_p->HasTypeList() == false) {
        continue; 
      }
      
      dbg_printf("    Arguments: ");
      TypeList *type_list_p = item_p->GetTypeList(header_p);
      for(ShortTypeId type_id : *type_list_p) {
        DebugPrintTypeString(type_id, &buffer);
        buffer.Append(' ');
      }
      
      buffer.WriteLineReset(fp);
    }
    
    return;
  }
  
  /*
   * DebugPrintAllFields() - Prints all fields
   */
  void DebugPrintAllFields(FILE *fp) {
    Buffer buffer;
    
    for(uint32_t i = 0;i < GetFieldCount();i++) {
      FieldIdItem *item_p = field_list + i;
      
      dbg_printf("Field %u: type = \"", i);
      DebugPrintTypeString(item_p->field_type_id, &buffer);
      buffer.Append("\" class = \"");
      DebugPrintTypeString(item_p->class_type_id, &buffer);
      buffer.Append("\" name = \"");
      DebugPrintString(item_p->name_id, &buffer);
      
      buffer.WriteLineReset(fp, "\"");
    }
    
    return;
  }
  
  /*
   * DebugPrintAllMethods() - Prints methods
   */
  void DebugPrintAllMethods(FILE *fp) {
    Buffer buffer;
    
    for(uint32_t i = 0;i < GetMethodCount();i++) {
      MethodIdItem *item_p = method_list + i;
      
      dbg_printf("Method %u: class = \"", i);
      DebugPrintTypeString(item_p->class_type_id, &buffer);
      buffer.Append("\" name = \"");
      DebugPrintString(item_p->name_id, &buffer);
      buffer.WriteLineReset(fp, "\"");
      
      dbg_printf("    ");
      assert(item_p->proto_id < GetProtoCount());
      ProtoIdItem *proto_p = proto_list + item_p->proto_id;
      buffer.Append("rtype = \"");
      DebugPrintTypeString(proto_p->return_type_id, &buffer);
      buffer.Append("\" Short name = \"");
      DebugPrintString(proto_p->name_id, &buffer);
      
      buffer.WriteLineReset(fp, "\"");
    }
    
    return;
  }
};

} // namespace dex
} // namespace android_dalvik_analysis
} // namespace wangziqi2013

#endif
