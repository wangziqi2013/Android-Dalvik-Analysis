
#pragma once

#ifndef _RES_BASE_H
#define _RES_BASE_H

#include "common.h"
#include "utf.h"

namespace wangziqi2013 {
namespace android_dalvik_analysis { 

/*
 * class ResourceBase - Represents common data structure and procedures for
 *                      parsing the resource file
 *
 * This class serves as the base class for all kinds of resources. Subclasses
 * should inherit this class as its public base class and could use the type
 * defined inside this class without redeclaration
 */
class ResourceBase {
 protected:
  
  /*
   * enum class ChunkType - The type of a chunk
   */
  enum class ChunkType : uint16_t {
    XML_DOCUMENT = 0x0003,
    STRING_POOL = 0x0001,
    RESOURCE_MAP = 0x0180,
    NAME_SPACE_START = 0x0100,
    NAME_SPACE_END = 0x0101,
    ELEMENT_START = 0x0102,
    ELEMENT_END = 0x0103,
    
    RESOURCE_TABLE = 0x0002,
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
     *
     * This function always append UTF-8 string to the buffer no matter
     * whether the original is UTF-8 or UTF-16
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
        
        // This makes sure that mainstream applications including linux terminal
        // could display it properly
        s.PrintUtf8(buffer_p);
      } else {
        Utf16String s{string_p};
        
        s.PrintUtf8(buffer_p);
      }
      
      return;
    }
    
    /*
     * DebugPrint() - Prints a string on screen using debug printing
     *
     * This function is only functional if debug flag is turned on.
     *
     * The format string must carries only one format specifier, %s, which
     * will be replaced by the actual string
     */
    void DebugPrint(size_t index, const char *format) {
#ifndef NDEBUG
      Buffer buffer;
      AppendToBuffer(index, &buffer);
      
      // For UTF-8 it is always the terminating character
      buffer.AppendByte('\0');
      
      dbg_printf(format, buffer.GetData());
#else
      (void)index;
      (void)format;
#endif      
      return;
    }
  };
  
  // This is used to indicate the string is invalid (i.e. does not exist)
  constexpr static uint32_t INVALID_STRING = 0xFFFFFFFF;
  
  /*
   * class ResourceValue - Typed representation of resource values
   */
  class ResourceValue {
   public:
    // The length of this struct
    uint16_t length;
    
    // It is an empty file and should always be 0
    uint8_t zero;
    
    /*
     * enum class DataType - Denotes the type of the data contained in this 
     *                       class instance
     */
    enum class DataType : uint8_t {
      // The 'data' is either 0 or 1, specifying this resource is either
      // undefined or empty, respectively.
      NULL_TYPE = 0x00,
      // The 'data' holds a ResTable_ref, a reference to another resource
      // table entry.
      REFERENCE = 0x01,
      // The 'data' holds an attribute resource identifier.
      ATTRIBUTE = 0x02,
      // The 'data' holds an index into the containing resource table's
      // global value string pool.
      STRING = 0x03,
      // The 'data' holds a single-precision floating point number.
      FLOAT = 0x04,
      // The 'data' holds a complex number encoding a dimension value,
      // such as "100in".
      DIMENSION = 0x05,
      // The 'data' holds a complex number encoding a fraction of a
      // container.
      FRACTION = 0x06,
      // The 'data' holds a dynamic ResTable_ref, which needs to be
      // resolved before it can be used like a TYPE_REFERENCE.
      DYNAMIC_REFERENCE = 0x07,
      // The 'data' holds an attribute resource identifier, which needs to be resolved
      // before it can be used like a TYPE_ATTRIBUTE.
      DYNAMIC_ATTRIBUTE = 0x08,

      // The 'data' is a raw integer value of the form n..n.
      INT_DEC = 0x10,
      // The 'data' is a raw integer value of the form 0xn..n.
      INT_HEX = 0x11,
      // The 'data' is either 0 or 1, for input "false" or "true" respectively.
      INT_BOOLEAN = 0x12,

      // The 'data' is a raw integer value of the form #aarrggbb.
      INT_COLOR_ARGB8 = 0x1c,
      // The 'data' is a raw integer value of the form #rrggbb.
      INT_COLOR_RGB8 = 0x1d,
      // The 'data' is a raw integer value of the form #argb.
      INT_COLOR_ARGB4 = 0x1e,
      // The 'data' is a raw integer value of the form #rgb.
      INT_COLOR_RGB4 = 0x1f,
    };
    
    // This is an enum type
    DataType type;
    uint32_t data;
  } BYTE_ALIGNED;
  
  /*
   * class Attribute - Represents attribute structure
   */
  class Attribute {
   public:
    uint32_t name_space;
    uint32_t name;
    
    // This is a reference to the string pool. 0xFFFFFFFF if not available
    uint32_t raw_value;
    
    // This describes typed value
    ResourceValue resource_value;
  } BYTE_ALIGNED;
 
 // Protected data member
 protected: 
  
  // This points to an external buffer. Whether the buffer belongs to the 
  // class is determined by own_data flag
  unsigned char *raw_data_p;
  
  // This is the length of the data buffer
  size_t length;
  
  // Whether the buffer should be freed during destruction
  // The buffer is freed as unsigned char[]
  bool own_data;
  
  // Pointer to the string pool
  StringPoolHeader *string_pool_header_p;  
  
  // The real string pool
  StringPool string_pool;
  
 // Common functions
 protected: 
 
  /*
   * Constructor
   */
  ResourceBase(unsigned char *p_raw_data_p, 
               size_t p_length, 
               bool p_own_data=false) :
    raw_data_p{p_raw_data_p},
    length{p_length},
    own_data{p_own_data},
    string_pool_header_p{nullptr},
    string_pool{} 
  {}
  
  /*
   * Destructor
   */
  ~ResourceBase() {
    // If the ownership is transferred to this instance then need to free data
    if(own_data == true) {
      delete[] raw_data_p;  
    }
    
    return; 
  }
  
  /*
   * ParseStringPool() - Parses the string pool and constructs the string 
   *                     pool object
   */
  void ParseStringPool(CommonHeader *header_p) {
    string_pool_header_p = reinterpret_cast<StringPoolHeader *>(header_p);
    
    // Do this as a temporary measure because we do not want to deal with it now
    assert(string_pool_header_p->style_count == 0);
    
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
    
    dbg_printf("Finished parsing string pool of %u strings and %u styles\n", 
               string_pool_header_p->string_count,
               string_pool_header_p->style_count);
               
    assert(string_pool_header_p->style_count == 0 && \
           "This is a temporary measure");
    
    return;
  }
 
 public:
  
  /*
   * DebugPrintAllStrings() - Prints all strings inside the string pool to
   *                          stderr for debugging purposes
   */
  void DebugPrintAllStrings() {
    for(size_t i = 0;i < string_pool.string_count;i++) {
      Buffer buffer{};
      
      fprintf(stderr, "string %lu: ", i);
      string_pool.AppendToBuffer(i, &buffer);
      buffer.WriteToFile(stderr);
      fprintf(stderr, "\n");
    }
    
    return;
  }
};

} // namespace android_dalvik_analysis
} // namespace wangziqi2013

#endif
