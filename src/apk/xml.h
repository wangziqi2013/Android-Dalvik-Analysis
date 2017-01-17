
#pragma once

#ifndef _XML_H
#define _XML_H

#include "common.h"
#include "utf.h"
#include <list>

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
    XML_DOCUMENT = 0x0003,
    STRING_POOL = 0x0001,
    RESOURCE_MAP = 0x0180,
    NAME_SPACE_START = 0x0100,
    NAME_SPACE_END = 0x0101,
    ELEMENT_START = 0x0102,
    ELEMENT_END = 0x0103,
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
  
  // This is used to indicate the string is invalid (i.e. does not exist)
  constexpr static uint32_t INVALID_STRING = 0xFFFFFFFF;
  
  // This is just a common header with its data being an array of uint32_t
  // as resource IDs
  using ResourceMapHeader = CommonHeader;
  using ResourceId = uint32_t;
  
  /*
   * class ElementHeader - XML elements all have an extra header that
   *                       describes its property in the original file
   */
  class ElementHeader {
   public:
    uint32_t line_number;
    
    // This is a reference into the string table
    uint32_t comment;
  } BYTE_ALIGNED;
  
  /*
   * class NameSpaceStart - Denotes the start of a namespace. All elements 
   *                        inside this inherits the same ns unless specified
   *                        otherwise
   */
  class NameSpaceStart {
   public: 
    // These two are common for all XML elements
    CommonHeader common_header;
    ElementHeader element_header;
    
    // The following two are indices in the string table
    uint32_t prefix;
    uint32_t uri;
  } BYTE_ALIGNED;
  
  // They have exactly the same layout; just the type field in the common 
  // header is different
  using NameSpaceEnd = NameSpaceStart;
  
  /*
   * class NameSpaceStatus - Describes namespaces we have entered and not
   *                         yet exited
   */
  class NameSpaceStatus {
   public: 
    uint32_t prefix;
    uint32_t uri;
    
    // Whether the name space information has been printed on the most recent
    // element tags
    bool printed;
    
    /*
     * Constructor
     */
    NameSpaceStatus(NameSpaceStart *start_p) :
      prefix{start_p->prefix},
      uri{start_p->uri},
      printed{false}
    {}
  };
  
  /*
   * class ElementStart - The start of an element
   */
  class ElementStart {
   public:
    // The header_length field in common header only covers the following two
    CommonHeader common_header;
    ElementHeader element_header;
    
    // The following two are indices in the string table
    
    // The name space of the element
    uint32_t name_space;
    // The name of the element
    uint32_t name;
    
    // This is relative to the offset from "name_space"
    uint16_t attribute_offset;
    // Byte size of attribute structures
    uint16_t attribute_length;
    // Number of attributes with the element
    uint16_t attribute_count;
    
    // The following also describes attributes, but they are special attributes
    // so they are separately stored. 
    // 0 means non-exist, and the index here starts at 1
    uint16_t id_index;
    uint16_t class_index;
    uint16_t style_index;
  } BYTE_ALIGNED;
  
  /*
   * class ElementEnd - The end of an element which only has its name space
   *                    and element tag 
   */
  class ElementEnd {
   public:
    CommonHeader common_header;
    ElementHeader element_header;
    
    uint32_t name_space;
    uint32_t name;
  } BYTE_ALIGNED;
  
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
  
  // This is actually a pointer to the common header
  ResourceMapHeader *resource_map_header_p;
  
  // # of 32 bit entries in the res map
  size_t resource_map_size;
  
  // Pointer to the first element of the resource map array
  ResourceId *resource_map_p;
  
  // A list of namespaces we are currently in. 
  // These will be stored until they are exited
  std::list<NameSpaceStatus> name_space_list;
  
  // Number of name spaces not yet printed on the most recent element begin
  // This is an optimization to avoid searching the list on every element start
  size_t unprinted_name_space_count;
  
  // Always keep this the last object
  // This is used to hold the results of converting it back to normal XML
  Buffer buffer;
  
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
    string_pool_header_p{nullptr},
    string_pool{},
    resource_map_header_p{nullptr},
    resource_map_size{0UL},
    resource_map_p{nullptr},
    name_space_list{},
    unprinted_name_space_count{0UL} {
    
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
      dbg_printf("XML type 0x%X is wrong (expecting 0x%X)\n", 
                 static_cast<uint32_t>(xml_header_p->type), 
                 static_cast<uint32_t>(ChunkType::XML_DOCUMENT));
      
      return nullptr; 
    } else if(xml_header_p->header_length != sizeof(XmlHeader)) {
      dbg_printf("XML header length 0x%X is wrong (expecting 0x%lX)\n", 
                 xml_header_p->header_length, 
                 sizeof(XmlHeader));
      
      return nullptr;      
    } else if(xml_header_p->total_length != length) {
      dbg_printf("XML total length 0x%X is wrong (expecting 0x%lX)\n", 
                 xml_header_p->total_length, 
                 length);
      
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
    
    return;
  }
  
  /*
   * ParseResourceMap() - Parses the string pool and constructs the string 
   *                      pool object
   *
   * Note that the resource map has the same layout as a general header
   */
  void ParseResourceMap(CommonHeader *header_p) {
    resource_map_header_p = reinterpret_cast<ResourceMapHeader *>(header_p);
    
    // The length recorded in the header must equal the actual length
    // also the length must be a multuple of resource ID
    if((header_p->header_length != sizeof(ResourceMapHeader)) || \
       ((header_p->total_length - header_p->header_length) % \
        sizeof(ResourceId) != 0)) {
      ReportError(CORRUPT_RESOURCE_MAP);
    }
    
    resource_map_size = \
      (header_p->total_length - header_p->header_length) / sizeof(ResourceId);
      
    resource_map_p = reinterpret_cast<ResourceId *>( \
      TypeUtility::Advance(header_p, sizeof(ResourceMapHeader)));
      
    return;
  }
  
  /*
   * ParseNameSpaceStart() - Parse the start of namespace
   */
  void ParseNameSpaceStart(CommonHeader *header_p) {
    assert(header_p->type == ChunkType::NAME_SPACE_START);
    
    NameSpaceStart *name_space_start_p = \
      reinterpret_cast<NameSpaceStart *>(header_p);
    
    // We have seen a new ns and it is not printed yet
    unprinted_name_space_count++;
    // Construct a name space status object and then put that into the list
    // and also the namespace is initialized to be not printed
    name_space_list.emplace_back(name_space_start_p);
    
    return;
  }
  
  /*
   * UriToNameSpace() - This converts an URI string to its associated name 
   *                    space string
   *
   * If the URI is not found for all active name spaces then return 
   * Otherwise it is an error and an exception is thrown
   *
   * Note that we store both URI and name space ID as string index, so we do
   * not have to actually do string comparison
   */
  uint32_t UriToNameSpace(uint32_t uri) {
    for(const NameSpaceStatus &ns_stat : name_space_list) {
      if(ns_stat.uri == uri) {
        return ns_stat.prefix; 
      }
    }
    
    // If debug mode is ON we need to print the URI here for debugging
#ifndef NDEBUG
    Buffer buffer{};
    string_pool.AppendToBuffer(uri, &buffer);
    
    dbg_printf("URI: ");
    buffer.WriteToFile(stderr);
    
    fputc('\n', stderr);
#endif

    ReportError(NAME_SPACE_URI_NOT_FOUND);
    
    // Should never reach here
    assert(false);
    return INVALID_STRING;
  }
  
  /*
   * ParseElementStart() - Parse the start of element
   */
  void ParseElementStart(CommonHeader *header_p) {
    assert(header_p->type == ChunkType::ELEMENT_START);
    
    ElementStart *element_start_p = \
      reinterpret_cast<ElementStart *>(header_p);
    
    // Element opening character
    buffer.AppendByte('<');
    if(element_start_p->name_space != INVALID_STRING) {
      string_pool.AppendToBuffer(element_start_p->name_space, &buffer);
      buffer.AppendByte(':');
    } 
    
    // Then output the tag name
    string_pool.AppendToBuffer(element_start_p->name, &buffer);
    buffer.AppendByte(' ');
    
    return;
  }
  
  /*
   * ParseAttribute() - Parses attribute and print to the buffer
   *
   * This function assumes attributes are of fixed length
   */
  void ParseAttribute(Attribute *attr_p) {
    
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
        break;
      } 
      case ChunkType::RESOURCE_MAP: {
        ParseResourceMap(next_header_p);
        break; 
      }
      case ChunkType::NAME_SPACE_START: {
        ParseNameSpaceStart(next_header_p);
        break; 
      }
      case ChunkType::ELEMENT_START: {
        ParseElementStart(next_header_p);
        break; 
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
      buffer.WriteToFile(stderr);
      fprintf(stderr, "\n");
    }
    
    return;
  }
};

} // namespace android_dalvik_analysis
} // namespace wangziqi2013

#endif
