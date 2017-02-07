
#pragma once

#ifndef _XML_H
#define _XML_H

#include "common.h"

namespace wangziqi2013 {
namespace android_dalvik_analysis { 

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
  
  // This is the ID of a run time type
  using StringId = uint32_t;
  using TypeId = uint32_t;
  // In the type list, ID is short int
  using ShortTypeId = uint16_t;
  using ProtoId = uint32_t;
  
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
  } BYTE_ALIGNED;
  
  /*
   * class ProtoIdItem - Prototype items
   */
  class ProtoIdItem {
   public:
    // Shortened form of the prototype
    StringId short_desciptor;  
    TypeId return_type;
    
    // This is an offset to the file header, and the target is a 
    // TypeList instance
    uint32_t type_list_offset;
    
    /*
     * HasTypeList() - Whether there is a type list as parameter list
     */
    inline bool HasTypeList() const {
      return type_list_offset != 0;
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
   * GetTypeCount() - Returns the number of strings
   */
  inline size_t GetTypeCount() const {
    return header_p->type_ids_size; 
  }
  
  /*
   * GetProtoCount() - Returns the number of strings
   */
  inline size_t GetProtoCount() const {
    return header_p->proto_ids_size; 
  }
  
  /*
   * GetFieldCount() - Returns the number of strings
   */
  inline size_t GetFieldCount() const {
    return header_p->field_ids_size; 
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
  void DebugPrintAllStrings() {
    Buffer buffer; 
    
    StringId i = 0;
    for(MUtf8String &s : string_list) {
      dbg_printf("String %u: ", i);
      
      s.PrintAscii(&buffer);
      buffer.WriteLineReset(stderr);
      
      i++;
    }
    
    return;
  }
  
  /*
   * DebugPrintAllTypes() - Prints all types
   */
  void DebugPrintAllTypes() {
    Buffer buffer;
    for(TypeId i = 0;i < GetTypeCount();i++) {
      uint32_t id = type_list[i];
      
      dbg_printf("Type %u: \"", i);
      DebugPrintString(id, &buffer);
      buffer.WriteLineReset(stderr, "\"");
    }
    
    return;
  }
  
  /*
   * DebugPrintAllProtos() - Prints all prototypes
   */
  void DebugPrintAllProtos() {
    Buffer buffer;
    
    for(ProtoId i = 0;i < GetProtoCount();i++) {
      ProtoIdItem *item_p = &proto_list[i];
      
      dbg_printf("Proto: \"");
      DebugPrintString(item_p->short_desciptor, &buffer);
      buffer.WriteLineReset(stderr, "\"");
      
      dbg_printf("    Return type: ");
      DebugPrintTypeString(item_p->return_type, &buffer);
      buffer.WriteLineReset(stderr);
      
      // There is no argument
      if(item_p->HasTypeList() == false) {
        continue; 
      }
      
      dbg_printf("    Arguments: ");
      TypeList *type_list_p = item_p->GetTypeList(header_p);
      for(uint32_t j = 0;j < type_list_p->entry_count;j++) {
        DebugPrintTypeString(item_p->GetArgumentType(j, header_p), &buffer);
        buffer.Append(' ');
      }
      
      buffer.WriteLineReset(stderr);
    }
    
    return;
  }
  
  /*
   * DebugPrintAllFields() - Prints all fields
   */
  void DebugPrintAllFields() {
    Buffer buffer;
    
    for(uint32_t i = 0;i < GetFieldCount();i++) {
      FieldIdItem *item_p = field_list + i;
      
      dbg_printf("Field %d: ");
      DebugPrintTypeString(item_p->field_type_id, &buffer);
      buffer.Append(' ');
      DebugPrintTypeString(item_p->class_type_id, &buffer);
      buffer.Append("::");
      DebugPrintString(item_p->name_id, &buffer);
      
      buffer.WriteLineReset(stderr);
    }
    
    return;
  }
};

} // namespace android_dalvik_analysis
} // namespace wangziqi2013

#endif
