
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
   * DebugPrintString() - Prints out all strings inside the DEX
   */
  void DebugPrintString() {
    Buffer buffer; 
    
    int i = 0;
    for(MUtf8String &s : string_list) {
      dbg_printf("String %d: ", i);
      
      s.PrintAscii(&buffer);
      buffer.Append('\n');
      buffer.WriteToFile(stderr);
      buffer.Reset();
      
      i++;
    }
    
    return;
  }
};

} // namespace android_dalvik_analysis
} // namespace wangziqi2013

#endif
