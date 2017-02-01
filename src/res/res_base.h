
#pragma once

#ifndef _RES_BASE_H
#define _RES_BASE_H

#include "common.h"
#include "utf.h"
#include "res_value.h"

namespace wangziqi2013 {
namespace android_dalvik_analysis { 

/*
 * class ResourceId - Resource identifier in 32-bit field
 */
union ResourceId {
  // The following two are used for array type resource's complex
  // resource name
  
  // This indicates the resource ID represents an element inside an array
  static constexpr uint32_t ARRAY_ELEMENT_FLAG = 0x02000000;
  // This flags out the array notion and leaves out array index
  static constexpr uint32_t ARRAY_INDEX_MASK = 0x00FFFFFF;
  
 public:
  // The structural of resource ID is 0xpptteeee
  // so we start declaring the entry ID at low address and then
  // type ID and then package ID
  struct {
    uint16_t entry_id;
    uint8_t type_id;
    uint8_t package_id;
  } BYTE_ALIGNED;
  
  // 32 bit identifier used as a whole
  uint32_t data;
  
  /*
   * IsArrayElement() - Whether this resource represents an array element
   */
  inline bool IsArrayElement() const {
    return static_cast<bool>(data & 0x02000000);
  }
  
  /*
   * GetArrayIndex() - If the resource ID is of array type then return 
   *                   its index
   *
   * If not array type then result is undefined and under debug mode assertion
   * would fail
   */
  inline uint32_t GetArrayIndex() const {
    assert(IsArrayElement() == true);
    
    return data & ARRAY_INDEX_MASK;
  }
} BYTE_ALIGNED;

// Make sure the size of the union is always correct
static_assert(sizeof(ResourceId) == sizeof(uint32_t), 
              "Invalid size of resource ID");

// This is implemented in res_table.cpp
void GetResourceIdStringWrapper(ResourceId id, Buffer *buffer_p);

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
    PACKAGE = 0x0200,
    TYPE_SPEC = 0x0202,
    TYPE = 0x0201,
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
    // We may want to access meta-data such as total size of the string pool
    StringPoolHeader *header_p;
   
    // Number of strings stored in the pool
    size_t string_count;
    
    // Number of styles in the pool
    size_t style_count;
    
    // The pointer to the array that stores offset of each string
    uint32_t *string_index_p;
    
    // Points to the starting of style uint32_t offset table
    uint32_t *style_index_p;
    
    // The pointer to the table that strings are stored
    // Begin offset of each string
    unsigned char *string_start;
    
    // Points to the style content start
    unsigned char *style_start;
    
    // Whether the string is represented in UTF-format or not
    // UTF-8 string has a one byte prefix denoting the length. However
    // in this format there are two such bytes with replicated values
    // (more precisely they are UTF-8 and UTF-16 length respectively)
    bool is_utf8;
    
    /*
     * AppendToBuffer() - Appends the string at a given index to the buffer
     *
     * This function always append UTF-8 string to the buffer no matter
     * whether the original is UTF-8 or UTF-16, because UTF-8 is widely 
     * supported on all platforms and on most applications (even like 
     * Linux terminal)
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
      
      fprintf(stderr, format, buffer.GetData());
#else
      (void)index;
      (void)format;
#endif      
      return;
    }
  };
  
  // This is used to indicate the string is invalid (i.e. does not exist)
  static constexpr uint32_t INVALID_STRING = 0xFFFFFFFF;
  
  // This is the first line of any XML file we are outputting
  static const char XML_HEADER_LINE[];
  static const char RESOURCE_END_TAG[];
  
  /*
   * class Attribute - Represents XML attribute structure
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
   * ConstructStringPool() - Constructs a string pool given a pointer
   */
  static void ConstructStringPool(CommonHeader *header_p,
                                  StringPool *string_pool_p) {
    // Since it is a static function this does not mask off
    // the member because anyway the member is not accessible from here
    StringPoolHeader *string_pool_header_p = \
      reinterpret_cast<StringPoolHeader *>(header_p);
    
    // This is for accessing metadata of the pool such as size
    string_pool_p->header_p = string_pool_header_p;
         
    // This is an array of uint32_t that stores the offset into string 
    // content table. It is located right after the header
    string_pool_p->string_index_p = \
      reinterpret_cast<uint32_t *>(
        TypeUtility::Advance(header_p, header_p->header_length));
    
    // The style index table is just located after the string index table
    string_pool_p->style_index_p = \
      string_pool_p->string_index_p + string_pool_header_p->string_count;
    
    // This is a relative offset to the first byte of the header
    string_pool_p->string_start = \
      reinterpret_cast<unsigned char *>(
        TypeUtility::Advance(header_p, string_pool_header_p->string_offset));
    
    // Also get style data region's pointer    
    string_pool_p->style_start = \
      reinterpret_cast<unsigned char *>(
        TypeUtility::Advance(header_p, string_pool_header_p->style_offset));
        
    string_pool_p->string_count = string_pool_header_p->string_count;
    string_pool_p->style_count = string_pool_header_p->style_count;
    
    // If it is UTF-8 then each ASCII is represented using only 1 byte
    string_pool_p->is_utf8 = \
      !!(string_pool_header_p->flags & StringPoolHeader::Flags::UTF8);
    
    dbg_printf("    Finished parsing string pool of %u strings and %u styles\n", 
               string_pool_header_p->string_count,
               string_pool_header_p->style_count);
    
    return;                                  
  }
  
  /*
   * ParseStringPool() - Parses the string pool and constructs the string 
   *                     pool object
   *
   * Note that this function writes to the current string pool object inside
   * the class instance
   */
  void ParseStringPool(CommonHeader *header_p) {
    // First assign header pointer
    string_pool_header_p = reinterpret_cast<StringPoolHeader *>(header_p);
    
    // Construct a string pool on the embedded string pool object
    ConstructStringPool(header_p, &string_pool);
    
    return;
  }
  
  /*
   * AppendResourceValueToBuffer() - This appends the given resource value to
   *                                 a buffer
   *
   * This function is a wrapper over ResourceValue's AppendToBuffer() because
   * the string type should be handled outside of the class
   */
  void AppendResourceValueToBuffer(ResourceValue *value_p, 
                                   Buffer *buffer_p, 
                                   bool resolve_reference=true) {
    ResourceValue::DataType type = value_p->type;
    
    // If it is string type then just grab the string from the current
    // global string pool
    if(type == ResourceValue::DataType::STRING) {
      string_pool.AppendToBuffer(value_p->data, buffer_p);
    } else if(type == ResourceValue::DataType::REFERENCE) {
      if(resolve_reference == true) {
        // Cast it to a resource ID and then use package ID to locate
        // the package as well as the table
        ResourceId id;
        id.data = value_p->data;
        
        GetResourceIdStringWrapper(id, buffer_p);
      } else {
        buffer_p->Printf("@0x%08X", value_p->data); 
      }
    } else {
      value_p->AppendToBuffer(buffer_p); 
    }
    
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
