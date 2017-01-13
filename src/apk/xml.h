
#pragma once

#ifndef _XML_H
#define _XML_H

#include "common.h"

namespace wangziqi2013 {
namespace android_dalvik_analysis { 

/*
 * class BinaryXml - Class for representing and parsing binary XML data
 */
class BinaryXml {
 private:
  
  /*
   * enum class ChunkType - The type of a chunk
   */
  enum class ChunkType : uint16_t {
    XML_DOCUMENT = 0x0000,
    STRING_POOL = 0x0001,
  };
  
  /*
   * class CommonHeader - A common header to denote the type of the following 
   *                      XML node
   */
  class CommonHeader {
   public:
    // This enum has the same value as recorded inside the binary XML
    ChunkType type; 
    
    // The size of the header (may be larger than 8 because there will be 
    // extra fields in extended header)
    uint16_t header_size;
    
    // The total size of the chunk including the header
    // If this equals header size then it is an indication of empty data
    uint32_t total_size;
  } BYTE_ALIGNED;
  
  /*
   * class StringPoolHeader - This class represents the string pool header
   */
  class StringPoolHeader {
   public:
    // Use this to decide chunk length
    CommonHeader common_header;
    
    uint32_t string_count;
    uint32_t style_count;
    uint32_t flags;
    uint32_t string_offset;
    uint32_t style_offset;
  } BYTE_ALIGNED;
};

} // namespace wangziqi2013
} // namespace android_dalvik_analysis

#endif
