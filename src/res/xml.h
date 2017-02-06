
#pragma once

#ifndef _XML_H
#define _XML_H

#include "common.h"
#include "res_base.h"

#include <unordered_map>
#include <cstddef>

namespace wangziqi2013 {
namespace android_dalvik_analysis { 

/*
 * class BinaryXml - Class for representing and parsing binary XML data
 */
class BinaryXml : public ResourceBase {
 private:
   
  // This is just a common header with its data being an array of uint32_t
  // as resource IDs
  using ResourceMapHeader = CommonHeader;
  using ResourceId = uint32_t;
  
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
  
  /*
   * class ElementHeader - This is the header for all general XML headers
   */
  class ElementHeader {
   public:
    uint32_t line_number;
    
    // This is a reference into the string table
    uint32_t comment;
  } BYTE_ALIGNED;
  
  /*
   * class NameSpaceStartHeader - Denotes the start of a namespace. All elements 
   *                              inside this inherits the same ns unless 
   *                              specified otherwise
   */
  class NameSpaceStartHeader {
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
  using NameSpaceEndHeader = NameSpaceStartHeader;
  
  /*
   * class NameSpaceStatus - Describes namespaces we have entered and not
   *                         yet exited
   */
  class NameSpaceStatus {
   public: 
    uint32_t prefix;
    uint32_t uri;
    
    // This is marked if the name space status has not yet been stored
    // into an Element instance
    bool printed;
    
    // If the name space end tag is encountered this is set to true
    bool ended;
    
    /*
     * Constructor
     */
    NameSpaceStatus(NameSpaceStartHeader *start_p) :
      prefix{start_p->prefix},
      uri{start_p->uri},
      printed{false},
      ended{false}
    {}
  };
  
  /*
   * class ElementStartHeader - The start of an element
   */
  class ElementStartHeader {
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
    
    // The index of few special attributes into the above attribute array
    // so they are separately stored. 
    // 0 means non-exist, and the index here starts at 1
    uint16_t id_index;
    uint16_t class_index;
    uint16_t style_index;
  } BYTE_ALIGNED;
  
  /*
   * class ElementEndHeader - The end of an element which only has its name 
   *                          space and element tag 
   */
  class ElementEndHeader {
   public:
    CommonHeader common_header;
    ElementHeader element_header;
    
    uint32_t name_space;
    uint32_t name;
  } BYTE_ALIGNED;
  
  /*
   * class Element - The class that describes XML element
   *
   * We build a tree of XML elements, so each element has a 
   */
  class Element {
   public:
    // This points to the actual element start header
    // and has all information we need to parse the element
    ElementStartHeader *header_p; 
     
    // These three describes the element
    std::vector<Element *> child_list;
    std::vector<NameSpaceStatus> name_space_list;
    std::vector<Attribute *> attribute_list;
    std::vector<CommonHeader *> cdata_list;
    
    /*
     * Constructor
     */
    Element(CommonHeader *p_header_p) :
      header_p{reinterpret_cast<ElementStartHeader *>(p_header_p)},
      child_list{},
      name_space_list{},
      attribute_list{},
      cdata_list{}
    {}
    
    /*
     * Default Constructor
     *
     * This is used by the embedded "fake root" instance
     */
    Element() :
      header_p{nullptr},
      child_list{},
      name_space_list{},
      attribute_list{},
      cdata_list{}
    {}
  };
  
 // Private data memver
 private: 
 
  // This points to the valid document header if there is one
  // Otherwise set to nullptr
  XmlHeader *xml_header_p;
  
  // This is actually a pointer to the common header
  ResourceMapHeader *resource_map_header_p;
  
  // # of 32 bit entries in the res map
  size_t resource_map_entry_count;
  
  // Pointer to the first element of the resource map array
  ResourceId *resource_map_p;
  
  // This maps from URI to the name space status structure that contains meta
  // information about namespaces
  // Note that this stores all namespaces we have seen. Only those not
  // associated with element tag are marked as not printed and will be 
  // printed the next time we encounter an element tag
  std::unordered_map<uint32_t, NameSpaceStatus> name_space_map;
  
  // This is the number of namespaces that have not been printed
  size_t unprinted_name_space_count;
  
  // This is the root element of XML
  Element root_element;
  
  // This is a stack of element nodes we are working on
  // Since we do not do recursive traversal, we need to maintain a 
  // stack manually
  // When we see an element end tag, we just pop the top from the stack and
  // use it as the current element pointer
  std::vector<Element *> element_stack;
  
  // This represents the current element
  // Whenever we have seen an element start tag it will be pushed into
  // the list inside the current element
  Element *current_element_p;
  
 public:
  
  /*
   * Constructor - Inside the constructor we will try to print everything
   *               into the buffer as parsing proceeds
   */
  BinaryXml(unsigned char *p_raw_data_p, 
            size_t p_length, 
            bool p_own_data=false) :
    ResourceBase{p_raw_data_p, p_length, p_own_data},
    xml_header_p{nullptr},
    resource_map_header_p{nullptr},
    resource_map_entry_count{0UL},
    resource_map_p{nullptr},
    name_space_map{},
    unprinted_name_space_count{0UL},
    root_element{},
    element_stack{},
    current_element_p{&root_element} {
    
    // This sets up xml_header_p, and also verifies basic
    // properties of the bianry file
    CommonHeader *next_header_p = VerifyXmlHeader();  
    if(next_header_p == nullptr) {
      xml_header_p = nullptr;
      
      return; 
    }
    
    // Loop until ParseNext() declares the end of parsing stage
    while(next_header_p != nullptr) {
      next_header_p = ParseNext(next_header_p);
    }
    
    // Check consistency
    VerifyElementStack();
    VerifyNameSpace();
        
    return;  
  }
  
  /*
   * Destructor
   *
   * Note that array ownership and destruction has already been handled in the
   * base class destructor
   */
  ~BinaryXml() {
    return;
  }
  
  /*
   * IsValidXml() - Returns true if the xml has a valid header
   *
   * We use xml_header_p to denote whether XML is valid or at least has 
   * a valid header
   */
  inline bool IsValidXml() {
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
   * verifyElementStack() - Verifies that the element stack is empty after
   *                        parsing all nodes
   */
  void VerifyElementStack() {
    if(element_stack.size() != 0UL) {
      ReportError(ELEMENT_UNCLOSED); 
    }
    
    return;
  }
  
  /*
   * VerifyNameSpace() - Checks whether name spaces are all closed
   */
  void VerifyNameSpace() {
    for(const auto &p : name_space_map) {
      if(p.second.ended == false || p.second.printed == false) {
        ReportError(NAME_SPACE_UNCLOSED);
      }
    }
    
    return;
  }
  
  /*
   * ParseResourceMap() - Parses the string pool and constructs the string 
   *                      pool object
   *
   * Note that the resource map has the same layout as a general header. We
   * also verify the size of the resource map using the number of entry here
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
    
    // This is the number of entries inside the resource map
    resource_map_entry_count = \
      (header_p->total_length - header_p->header_length) / sizeof(ResourceId);
      
    resource_map_p = reinterpret_cast<ResourceId *>( \
      TypeUtility::Advance(header_p, sizeof(ResourceMapHeader)));
      
    return;
  }
  
  /*
   * ParseNameSpaceStart() - Parse the start of namespace
   *
   * This function adds a new class Element instance to the parent element
   * i.e. current_element_p, and then pushes name spaces, attributes into the
   * new element, and then switch pointer to this element
   */
  void ParseNameSpaceStart(CommonHeader *header_p) {
    assert(header_p->type == ChunkType::NAME_SPACE_START);
    
    NameSpaceStartHeader *name_space_start_p = \
      reinterpret_cast<NameSpaceStartHeader *>(header_p);
    
    // The new name space must not be printed here
    unprinted_name_space_count++;
    
    uint32_t uri = name_space_start_p->uri;
    // Construct a name space status object and then put that into the list
    // and also the namespace is initialized to be not printed
    auto ret = \
      name_space_map.insert( \
        std::make_pair(uri, NameSpaceStatus{name_space_start_p}));
    
    // If there is already the URI then we declare error because name spaces
    // could not clash in one file                   
    if(ret.second == false) {
      ReportError(NAME_SPACE_ALREADY_EXIST, uri); 
    }
    
    return;
  }
  
  /*
   * UriToNameSpacePrefix() - This converts an URI string to its associated name 
   *                          space prefix string
   *
   * If the URI is not found for all active name spaces then return 
   * Otherwise it is an error and an exception is thrown
   *
   * Note that we store both URI and name space ID as string index, so we do
   * not have to actually do string comparison
   */
  uint32_t UriToNameSpacePrefix(uint32_t uri) {
    auto it = name_space_map.find(uri);
    if(it != name_space_map.end()) {
      return it->second.prefix; 
    }
    
    // If debug mode is ON we need to print the URI here for debugging
#ifndef NDEBUG
    Buffer buffer{};
    string_pool.AppendToBuffer(uri, &buffer);
    buffer.Append('\n');
    
    dbg_printf("prefix for not found. URI = ");
    buffer.WriteToFile(stderr);
#endif

    ReportError(NAME_SPACE_URI_NOT_FOUND);
    
    // Should never reach here
    assert(false);
    return INVALID_STRING;
  }
  
  /*
   * PrintOptionalNameSpace() - This prints the name space using its URI if 
   *                            the URI is not invalid
   *
   * This function prints the namespace as:
   *   "namespace:" 
   * where namespace is the name space string translated using URI
   */
  void PrintOptionalNameSpace(uint32_t uri, Buffer *buffer_p) {
    if(uri != INVALID_STRING) {
      // Since it is URI we need to convert it to prefix
      uint32_t ns_prefix = UriToNameSpacePrefix(uri);
      
      string_pool.AppendToBuffer(ns_prefix, buffer_p);
      buffer_p->Append(':');
    }
    
    return;
  }
  
  /*
   * AddNameSpace() - Prints all name spaces that have not been printed
   *                  yet to the body of the element tag
   *
   * This function adds all name spaces that have not been associated with any
   * tag yet to the current element, and then set the flag to true
   */
  void AddNameSpace() {
    // As an optimization, we first check whether there is any
    // unprinted name spaces. If not then return directly without 
    // looping through all items
    if(unprinted_name_space_count == 0UL) {
      return; 
    }
    
    // Loop through all emlements in the hash table
    for(auto &it : name_space_map) {
      NameSpaceStatus &ns_status = it.second;
      
      // Push the unprinted name space into the current element
      if(ns_status.printed == false) {
        current_element_p->name_space_list.push_back(ns_status);
        
        // Set the mark to avoid it from being printed in the next tag
        ns_status.printed = true;
        
        // This could not be negative
        assert(unprinted_name_space_count != 0UL);
        unprinted_name_space_count--;
      } // if not printed yet
    } // for it in all elements
    
    return;
  }
  
  /*
   * AddAttribute() - Adds attributes into the attribute list for the current
   *                  element node
   */
  void AddAttribute(CommonHeader *header_p) {
    // Make sure it is element start type
    assert(header_p->type == ChunkType::ELEMENT_START);
    
    ElementStartHeader *element_start_p = \
      reinterpret_cast<ElementStartHeader *>(header_p);
    
    // It is attribute offset + relative offset of name_space field inside 
    // the structure
    Attribute *attribute_p = \
      reinterpret_cast<Attribute *>(
        TypeUtility::Advance(
          header_p, 
          element_start_p->attribute_offset + \
            offsetof(ElementStartHeader, name_space)));
    
    // For each attribute add it to the attribute list of the current element
    for(uint16_t i = 0;i < element_start_p->attribute_count;i++) {
      current_element_p->attribute_list.push_back(attribute_p + i);
    }
    
    return;
  }
  
  /*
   * ParseElementStart() - Parse the start of element
   *
   * This function explicitly adds name spaces as well as attributes to a 
   * newly created element node. Children nodes are added non-explicitly
   * by repeated call of this function
   */
  void ParseElementStart(CommonHeader *header_p) {
    assert(header_p->type == ChunkType::ELEMENT_START);
    
    // Create a new instance and then switch pointer after pushing
    // the current one into the stack
    Element *new_element_p = new Element{header_p};
    
    current_element_p->child_list.push_back(new_element_p);
    
    // Save the current one
    element_stack.push_back(current_element_p);
    // And then switch pointer
    current_element_p = new_element_p;
    
    // Add name space to the newly created element
    AddNameSpace();
    // Add attributes
    AddAttribute(header_p);
    
    return;
  }
  
  /*
   * PrintAttribute() - Prints attribute value into a buffer object
   */
  void PrintAttribute(Attribute *attr_p, Buffer *buffer_p) {
    // This prints the optional ns with a colon after it
    PrintOptionalNameSpace(attr_p->name_space, buffer_p);
    
    string_pool.AppendToBuffer(attr_p->name, buffer_p);
    buffer_p->Append('=');
    
    buffer_p->Append('\"');
    if(attr_p->raw_value != INVALID_STRING) {
      string_pool.AppendToBuffer(attr_p->raw_value, buffer_p);
    } else {
      // Otherwise the value is typed and the type needs to be considered
      //ParseResourceValue(&attr_p->resource_value);  
    }
    
    buffer_p->Append('\"');
    buffer_p->Append(' ');
    
    return;
  }
  
  /*
   * ParseNameSpaceEnd() - Parses name space end tag
   *
   * Currently this function is empty because there is nothing we need to do
   * regarding name space end, since we keep all name spaces in a centralized 
   * map
   */
  void ParseNameSpaceEnd(CommonHeader *header_p) {
    NameSpaceEndHeader *end_header_p = \
      reinterpret_cast<NameSpaceEndHeader *>(header_p);
      
    auto it = name_space_map.find(end_header_p->uri);
    if(it == name_space_map.end()) {
      ReportError(UNMATCHED_NAME_SPACE_END);
    }
    
    // The name space has been ended
    it->second.ended = true;
      
    return;
  }
  
  /*
   * ParseElementEnd() - Parses the end of an element
   *
   * When we see an end of element, just pop it out of the stack and 
   * switch to it. This allows us working on the parent node
   */
  void ParseElementEnd(CommonHeader *header_p) {
    ElementEndHeader *end_header_p = \
      reinterpret_cast<ElementEndHeader *>(header_p);
    
    if(element_stack.size() == 0UL) {
      ReportError(UNEXPECTED_ELEMENT_END);
    }
    
    // If the end tag does not match previous begin tag then we are in trouble
    if(end_header_p->name != current_element_p->header_p->name || \
       end_header_p->name_space != current_element_p->header_p->name_space) {
      ReportError(UNMATCHED_ELEMENT_END);
    }
    
    // Switch to the upper level parent element object by popping from
    // the back
    current_element_p = element_stack.back();
    element_stack.pop_back();
    
    return;
  }
  
  /*
   * ParseCData() - Parses CDATA chunk
   *
   * CDATA chunk is not common, so we handle this in a simple manner
   */
  void ParseCData(CommonHeader *header_p) {
    current_element_p->cdata_list.emplace_back(header_p);
    
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
    
    // If we have reached the end of the chunk just return nullptr
    if(static_cast<size_t>((reinterpret_cast<unsigned char *>(next_header_p) - \
        raw_data_p)) >= length) {
      return nullptr;      
    }
    
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
      case ChunkType::NAME_SPACE_END: {
        ParseNameSpaceEnd(next_header_p);
        break; 
      }
      case ChunkType::ELEMENT_END: {
        ParseElementEnd(next_header_p);
        break; 
      }
      case ChunkType::CDATA: {
        ParseCData(next_header_p);
        break; 
      }
      default: {        
        ReportError(UNKNOWN_CHUNK_TYPE, 
                    static_cast<uint32_t>(next_header_p->type),
                    (size_t)next_header_p - (size_t)raw_data_p); 
        break;
      }
    } // switch type
    
    return ret_header_p;
  }
};

} // namespace android_dalvik_analysis
} // namespace wangziqi2013

#endif
