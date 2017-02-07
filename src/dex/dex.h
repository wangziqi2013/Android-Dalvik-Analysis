
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
    
    uint32_t link_size;
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
    
    return;
  }
};

} // namespace android_dalvik_analysis
} // namespace wangziqi2013

#endif
