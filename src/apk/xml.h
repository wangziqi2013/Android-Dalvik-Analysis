
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
    
    // This points to the next field
    unsigned char next[0];
  } BYTE_ALIGNED;
  
  // These two are equivalent
  using XmlHeader = CommonHeader;
  
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
    
    // This points to the next field
    unsigned char next[0];
  } BYTE_ALIGNED;
  
 // Private data memver
 private: 
 
  // This points to an external buffer that does not belong to this class
  unsigned char *raw_data_p;
  
  // This is the length of the data buffer
  size_t length;
  
  // This points to the valid document header if there is one
  // Otherwise set to nullptr
  XmlHeader *xml_header_p;
  
 public:
  
  /*
   * Constructor
   */
  BinaryXml(unsigned char *p_raw_data_p, size_t p_length) :
    raw_data_p{p_raw_data_p},
    length{p_length},
    xml_header_p{nullptr} {
    return;  
  }
  
  /*
   * Destructor
   */
  ~BinaryXml() {
    return;
  }
  
  /*
   * VerifyXmlHeader() - Verifies the beginning of the document
   *
   * If the format is not current/unknown then return false and no exception
   * is thrown; Otherwise return true and the 
   */
  bool VerifyXmlHeader() {
    // XML header begins at the first byte of the data 
    xml_header_p = reinterpret_cast<XmlHeader *>(raw_data_p);
    
    // All three fields could be determiend so we just compare value
    // in the data with expected value
    if(xml_header_p->type != XML_DOCUMENT) {
      return false; 
    } else if(xml_header_p->header_length != sizeof(XmlHeader)) {
      return false;      
    } else if(xml_header_p->total_length != length) {
      // We require that the entire document is part of the XML
      // Otherwise we are unable to decode the rest of it
      return false; 
    } else {
      return true; 
    }
  }
};

} // namespace wangziqi2013
} // namespace android_dalvik_analysis

#endif
