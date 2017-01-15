
#pragma once

#ifndef _XML_H
#define _XML_H

#include "common.h"
#include "utf.h"

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
    uint16_t header_length;
    
    // The total size of the chunk including the header
    // If this equals header size then it is an indication of empty data
    uint32_t total_length;
    
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
    
    /*
     * enum class Flags - The meaning of individual bits in the flag
     */
    enum Flags : uint32_t {
      SORTED = 0x1 << 0,
      UTF8 = 0x1 << 8,
    };
  } BYTE_ALIGNED;
  
  /*
   * class StringPool - The class that actually stores string pool data
   */
  class StringPool {
   public: 
    // Number of strings stored in the pool
    size_t string_count;
    
    // The pointer to the array that stores offset of each string
    uint32_t *string_index_p;
    
    // The pointer to the table that strings are stored
    // Begin offset of each string
    unsigned char *string_start;
    
    // Whether the string is represented in UTF-format or not
    // UTF-8 string has a one byte repfix denoting the length. However
    // in this format there are two such bytes with replicated values
    // (more precisely they are UTF-8 and UTF-16 length respectively)
    bool is_utf8;
    
    /*
     * AppendToBuffer() - Appends the string at a given index to the buffer
     */
    void AppendToBuffer(size_t index, Buffer *buffer_p) {
      if(index >= string_count) {
        ReportError(STRING_POOL_INDEX_TOO_LARGE, index, string_count); 
      }
      
      // This is the byte offset relative to the beginning of the data area
      uint32_t offset = string_index_p[index];
      // This points to the actual string
      unsigned char *string_p = string_start + offset;
       
      if(is_utf8 == true) {
        // Since UTF8 string has 2 length fields for both UTF16 length and UTF8 
        // length we should call an extra DecodeLength() to make sure the
        // pointer moves to the correct position
        Utf8String s{string_p};
        
        // This will discard the previous result
        s.DecodeLength();
        
        s.PrintAscii(buffer_p);
      } else {
        Utf16String s{string_p};
        
        s.PrintAscii(buffer_p);
      }
      
      return;
    }
  };
  
 // Private data memver
 private: 
 
  // This points to an external buffer. Whether the buffer belongs to the 
  // class is determined by own_data flag
  unsigned char *raw_data_p;
  
  // This is the length of the data buffer
  size_t length;
  
  // Whether the buffer should be freed during destruction
  // The buffer is freed as unsigned char[]
  bool own_data;
  
  // This points to the valid document header if there is one
  // Otherwise set to nullptr
  XmlHeader *xml_header_p;

  // Pointer to the string pool
  StringPoolHeader *string_pool_header_p;  
  
  // The real string pool
  StringPool string_pool;
  
 public:
  
  /*
   * Constructor
   */
  BinaryXml(unsigned char *p_raw_data_p, 
            size_t p_length, 
            bool p_own_data=false) :
    raw_data_p{p_raw_data_p},
    length{p_length},
    own_data{p_own_data},
    xml_header_p{nullptr},
    string_pool_header_p{nullptr} {
    
    CommonHeader *next_header_p = VerifyXmlHeader();  
    if(next_header_p == nullptr) {
      xml_header_p = nullptr;
      
      return; 
    }
    
    // This is only temporary measure to parse the string pool
    ParseNext(next_header_p);
    assert(string_pool_header_p != nullptr);
    
    return;  
  }
  
  /*
   * Destructor
   */
  ~BinaryXml() {
    // If the ownership is transferred to this instance then need to free data
    if(own_data == true) {
      delete[] raw_data_p;  
    }
    
    return;
  }
  
  /*
   * IsValidXml() - Returns true if the xml has a valid header
   *
   * We use xml_header_p to denote whether XML is valid or at least has 
   * a valid header
   */
  bool IsValidXml() {
    return xml_header_p != nullptr;
  }
  
  /*
   * VerifyXmlHeader() - Verifies the beginning of the document
   *
   * If the format is not current/unknown then return nullptr. Otherwise
   * the next byte after the XML header (which is of known length)
   *
   * Note that this function will set xml_header_p if it returns non nullptr. 
   * After this returns there could not be any XML header type until the parsing
   * finishes
   *
   * The caller should set xml_header_p to nullptr if this function returns 
   * false
   */
  CommonHeader *VerifyXmlHeader() {
    // XML header begins at the first byte of the data 
    xml_header_p = reinterpret_cast<XmlHeader *>(raw_data_p);
    
    // All three fields could be determiend so we just compare value
    // in the data with expected value
    if(xml_header_p->type != ChunkType::XML_DOCUMENT) {
      return nullptr; 
    } else if(xml_header_p->header_length != sizeof(XmlHeader)) {
      return nullptr;      
    } else if(xml_header_p->total_length != length) {
      // We require that the entire document is part of the XML
      // Otherwise we are unable to decode the rest of it
      return nullptr; 
    } else {
      // Return the next byte and cast it as common header for later parsing
      return reinterpret_cast<CommonHeader *>(
               TypeUtility::Advance(xml_header_p, sizeof(XmlHeader))); 
    }
    
    assert(false);
    return nullptr;
  }
  
  /*
   * ParseStringPool() - Parses the string pool and constructs the string 
   *                     pool object
   */
  void ParseStringPool(CommonHeader *header_p) {
    string_pool_header_p = reinterpret_cast<StringPoolHeader *>(header_p);
    
    // This is an array of uint32_t that stores the offset into string 
    // content table. It is located right after the header
    string_pool.string_index_p = reinterpret_cast<uint32_t *>(
        TypeUtility::Advance(header_p, header_p->header_length));
    
    // This is a relative offset to the first byte of the header
    string_pool.string_start = \
      reinterpret_cast<unsigned char *>(
        TypeUtility::Advance(header_p, string_pool_header_p->string_offset));
        
    string_pool.string_count = string_pool_header_p->string_count;
    
    // If it is UTF-8 then each ASCII is represented using only 1 byte
    string_pool.is_utf8 = \
      !!(string_pool_header_p->flags & StringPoolHeader::Flags::UTF8);
    
    return;
  }
  
  /*
   * ParseNext() - Central scheduling function that acts as a state machine and 
   *               calls sorresponding routine based on its type in the common 
   *               header
   *
   * This function returns the first byte after the current chunk
   * 
   * If parsing fails because of unparsable data an integer exception will 
   * be thrown. It is assumed that the verification function has not yet been
   * called.
   */
  CommonHeader *ParseNext(CommonHeader *next_header_p) {
    assert(next_header_p != nullptr);
    
    // The return value is the next header pointer (hopefully)
    CommonHeader *ret_header_p = \
      TypeUtility::Advance(next_header_p, next_header_p->total_length);
    
    switch(next_header_p->type) {
      case ChunkType::XML_DOCUMENT: {
        // The header needs to be validated outside this function 
        ReportError(UNEXPECTED_START_OF_XML);
        break;
      } 
      case ChunkType::STRING_POOL: {
        ParseStringPool(next_header_p);
      } 
      default: {
        ReportError(UNKNOWN_CHUNK_TYPE, 
                    static_cast<uint32_t>(next_header_p->type)); 
        break;
      }
    } // switch type
    
    return ret_header_p;
  }
  
  /*
   * DebugPrintAllStrings() - Prints all strings inside the string pool to
   *                          stderr for debugging purposes
   */
  void DebugPrintAllStrings() {
    for(size_t i = 0;i < string_pool.string_count;i++) {
      Buffer buffer{};
      
      fprintf(stderr, "string %lu: ", i);
      string_pool.AppendToBuffer(i, &buffer);
      fprintf(stderr, "\n");
    }
    
    return;
  }
};

} // namespace android_dalvik_analysis
} // namespace wangziqi2013

#endif
