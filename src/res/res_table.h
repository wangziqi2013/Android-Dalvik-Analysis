
#pragma once

#ifndef _RES_TBALE_H
#define _RES_TBALE_H

#include "common.h"
#include "res_base.h"

namespace wangziqi2013 {
namespace android_dalvik_analysis { 

/*
 * class ResourceTable - Represents binary resource table
 */
class ResourceTable : public ResourceBase {
 // type declarations
 private: 

  /*
   * class TableHeader - The resource table header which is at the beginning of 
   *                     the table
   */
  class TableHeader {
   public: 
    CommonHeader common_header;
    
    // Number of packages included in this resource table
    uint32_t package_count; 
  } BYTE_ALIGNED;
  
  /*
   * class PackageHeader - Package header that records metadata about package
   */
  class PackageHeader {
   public:
    CommonHeader common_header; 
     
    // Base package's ID (also in the res identifier)
    // If 0 then it is not a base package; it is always 0x7F for application
    // package type
    uint32_t id; 
    
    // UTF-16 encoded name of the package
    // The length is fixed 256 bytes encoded in UTF16 though UTF16 itself
    // is a variable length encoding scheme
    unsigned char name_utf16[128 * sizeof(char16_t)];
    
    // An offset from the beginning of this struct to the string pool for types
    uint32_t type_string_pool_offset;
    
    // Index into string pool to indicate the last type visible
    uint32_t last_public_type;
    
    // Same - an offset from this struct as the key string pool
    uint32_t key_string_pool_offset;
    
    // The last index of a key that is visible
    uint32_t last_public_key;
    
  } BYTE_ALIGNED;
  
  /*
   * class TypeSpecHeader - Type specification header
   *
   * This header is followed by an array of uint32_t integers denoting the 
   * configuration of resources available for a certain resource of this
   * type. There is excatly one entry for each resource instance
   */
  class TypeSpecHeader {
   public: 
    CommonHeader common_header;
    
    // Low 8 bits of a 32 bit integer represents the type id
    uint8_t id;
    uint8_t zero1;
    uint16_t zero2;
    
    // Number of entries after this struct
    uint32_t entry_count;
    
    // The pointer to the first entry (it does not consume struct space)
    uint32_t data[0];
  } BYTE_ALIGNED;
  
  /*
   * class TypeConfig - The configuration under which values should be used
   *
   * This structure is a complete set of possible configurations a Android
   * device could be configured
   */
  class TypeConfig {
   public:
    
  } BYTE_ALIGNED;
  
  /*
   * class TypeHeader - Header of the type instance chunk
   */
  class TypeHeader {
   public: 
    CommonHeader common_header;
    
    // Number of resource value instances inside this chunk
    size_t entry_count;
    
    // The offset to the starting address of the data part
    // Between this structure and the header is an array of uint32_t
    // that records the offset of each instance into the data part
    size_t data_offset;
    
    // This structure records the config of this group of values
    // i.e. they are used under common configurations
    TypeConfig config;
  } BYTE_ALIGNED;
  
  /*
   * class Package - Represents internals of a package
   */
  class Package {
   public: 
    // Pointer to the package header
    PackageHeader *header_p;
    
    // These two are used to store headers
    StringPoolHeader *type_string_pool_header_p;
    StringPoolHeader *key_string_pool_header_p;
    
    // Two string pools indicated in the header
    StringPool type_string_pool;
    StringPool key_string_pool;
    
    // This list contains type spec header for each type, one header
    // for exactly one type. Type i has an entry on index i - 1
    std::vector<TypeSpecHeader *> type_spec_header_list;
  };

 // Data members  
 private:
  TableHeader *table_header_p;
  
  // A list of packages
  // We do reserve space for this vector such that only 1 malloc() is done
  std::vector<Package> package_list;
 public:
  
  /*
   * Constructor
   */
  ResourceTable(unsigned char *p_raw_data_p, 
                size_t p_length, 
                bool p_own_data=false) :
    ResourceBase{p_raw_data_p, p_length, p_own_data},
    table_header_p{nullptr} {
    
    // Check whether all header fields are valid; if not just exit
    CommonHeader *next_header_p = VerifyTableHeader();  
    if(IsValidTable() == false) {
      table_header_p = nullptr;
      
      return; 
    }
    
    while(next_header_p != nullptr) {
      next_header_p = ParseNext(next_header_p);
    }
    
    assert(string_pool_header_p != nullptr);
    
    return;    
  }
  
  /*
   * Destructor
   */
  ~ResourceTable() {
    return; 
  }
  
  /*
   * IsValidTable() - Whether the resource table is valid after verification
   */
  inline bool IsValidTable() const {
    return table_header_p != nullptr; 
  }
  
  /*
   * VerifyTableHeader() - Check fields in the table header
   *
   * If all check pass then return the byte next to table header. Otherwise 
   * return nullptr.
   *
   * This function always sets table_header_p to the first byte of the stream.
   * However if check fails the caller is responsible for setting the pointer
   * to nullptr to indicate that the resource table is not valid 
   *
   * This function has exactly the same structure as VerifyXmlHeader()
   */
  CommonHeader *VerifyTableHeader() {
    table_header_p = reinterpret_cast<TableHeader *>(raw_data_p);
    
    if(table_header_p->common_header.type != ChunkType::RESOURCE_TABLE) {
      dbg_printf("Resource table type 0x%X is wrong (expecting 0x%X)\n", 
                 static_cast<uint32_t>(table_header_p->common_header.type), 
                 static_cast<uint32_t>(ChunkType::RESOURCE_TABLE));
      
      return nullptr; 
    } else if(table_header_p->common_header.header_length != \
              sizeof(TableHeader)) {
      dbg_printf("Resource table length 0x%X is wrong (expecting 0x%lX)\n", 
                 table_header_p->common_header.header_length, 
                 sizeof(TableHeader));
      
      return nullptr;      
    } else if(table_header_p->common_header.total_length != length) {
      dbg_printf("XML total length 0x%X is wrong (expecting 0x%lX)\n", 
                 table_header_p->common_header.total_length, 
                 length);
      
      // We require that the entire document is part of the XML
      // Otherwise we are unable to decode the rest of it
      return nullptr; 
    }
    
    dbg_printf("Verified resource table header; package count = %u\n",
               table_header_p->package_count);
               
    // This serves as an optimization such that we only allocate once
    // for the parsing process
    package_list.reserve(table_header_p->package_count);
    
    // Return the next byte and cast it as common header for later parsing
    return reinterpret_cast<CommonHeader *>(
             TypeUtility::Advance(table_header_p, sizeof(TableHeader))); 
  }
  
  /*
   * InitPackage() - Initialize a package object
   *
   * We could have done this in the package constructor
   */
  void InitPackage(Package *package_p, PackageHeader *package_header_p) {
    package_p->header_p = package_header_p;
    
    package_p->type_string_pool_header_p = \
      reinterpret_cast<StringPoolHeader *>( \
        TypeUtility::Advance(package_header_p, 
                             package_header_p->type_string_pool_offset));
    
    ConstructStringPool( \
      reinterpret_cast<CommonHeader *>(package_p->type_string_pool_header_p), 
      &package_p->type_string_pool);
                        
    package_p->key_string_pool_header_p = \
      reinterpret_cast<StringPoolHeader *>( \
        TypeUtility::Advance(package_header_p, 
                             package_header_p->key_string_pool_offset));
    
    ConstructStringPool( \
      reinterpret_cast<CommonHeader *>(package_p->key_string_pool_header_p), 
      &package_p->key_string_pool);
    
    // Give each type a type spec header slot in the list inside class Package
    // Resize it and initialize all elements to null pointer to identify
    // whether it has been initialized or not
    package_p->type_spec_header_list.resize( \
      package_p->type_string_pool.string_count, nullptr);
    
    return;
  }
  
  /*
   * DebugPrintPackageTypeString() - Prints all type strings in a package
   *
   * This function is meant for debugging
   */
  void DebugPrintPackageTypeString(Package *package_p) {
    dbg_printf("    Resource types: ");
    
    if(package_p->type_string_pool.string_count > 0) {
      package_p->type_string_pool.DebugPrint(0, "%s");
      // Print out all types in debug output; if debug is turned off this will
      // be optimized out                   
      for(size_t i = 1;i < package_p->type_string_pool.string_count;i++) {
        fprintf(stderr, " | ");
        package_p->type_string_pool.DebugPrint(i, "%s");
      }
      
      fprintf(stderr, "\n");
    } else {
      fprintf(stderr, "[None]\n"); 
    }
    
    return;
  }
  
  /*
   * ParsePackage() - Parses the package header and push a package
   *                  object to the package list
   */
  void ParsePackage(CommonHeader *header_p) {
    PackageHeader *package_header_p = \
      reinterpret_cast<PackageHeader *>(header_p);
      
    dbg_printf("    Package ID = 0x%02X\n", package_header_p->id);  
    
    // Even in Android runtime this is not taken care, so ....
    assert(package_header_p->type_string_pool_offset != 0);
    assert(package_header_p->key_string_pool_offset != 0);
    
    // Construct a packge object at the back of the vector
    // This saves the cost of copying the object
    package_list.emplace_back();
    // This points to the package object we just inserted
    Package *package_p = &package_list.back();
    
    // Initialize members inside the package object
    InitPackage(package_p, package_header_p);
    
#ifndef NDEBUG
    DebugPrintPackageTypeString(package_p);
#endif

    // The first type spec chunk must be after the key string pool
    // so use its total size to determine (hopefully string pool is aligned)
    CommonHeader *type_spec_header_p = \
      reinterpret_cast<CommonHeader *>( \
        TypeUtility::Advance(
          package_p->key_string_pool_header_p, 
          package_p->key_string_pool_header_p->common_header.total_length));
     
    // Each type will have a type spec chunk, so just use the number of 
    // elements in type string pool     
    for(size_t i = 0;i < package_p->type_string_pool.string_count;i++) {
      ParseTypeSpec(type_spec_header_p, package_p);
      
      // Use its length field to find the following type spec chunk
      type_spec_header_p = \
        TypeUtility::Advance(type_spec_header_p, 
                             type_spec_header_p->total_length);
    }
                        
    return;
  }
  
  /*
   * ParseTypeSpec() - Parses type specification header and body
   */
  void ParseTypeSpec(CommonHeader *header_p, Package *package_p) {
    dbg_printf("Parsing TypeSpec chunk @ offset 0x%lX\n", 
               TypeUtility::GetPtrDiff(raw_data_p, header_p));
    assert(header_p->type == ChunkType::TYPE_SPEC);
    
    TypeSpecHeader *type_spec_header_p = \
      reinterpret_cast<TypeSpecHeader *>(header_p);
    
    // Get the type ID which also represents its position in the vector
    uint32_t type_id = static_cast<uint32_t>(type_spec_header_p->id);
    assert(type_id != 0);
    
    // Each type only has one type spec header?
    //package_p->type_spec_header_p = type_spec_header_p
    
    dbg_printf("    Type id = %u; entry_count = %u\n", 
               static_cast<uint32_t>(type_spec_header_p->id),
               static_cast<uint32_t>(type_spec_header_p->entry_count));
    
    return;
  }
  
  /*
   * ParseNext() - Parse the contents of the resource table
   *
   * This function has the same structure as the one in class BinaryXml
   *
   * If we have reached the end then just return nullptr and that's it
   */ 
  CommonHeader *ParseNext(CommonHeader *next_header_p) {
    assert(next_header_p != nullptr);
    
    if(static_cast<size_t>((reinterpret_cast<unsigned char *>(next_header_p) - \
        raw_data_p)) >= length) {
      return nullptr;      
    }
    
    CommonHeader *ret_header_p = \
      TypeUtility::Advance(next_header_p, next_header_p->total_length);
    
    dbg_printf("Parsing header of type %u @ offset 0x%lX\n",
               (uint32_t)next_header_p->type,
               TypeUtility::GetPtrDiff(raw_data_p, next_header_p));
    
    switch(next_header_p->type) {
      case ChunkType::RESOURCE_TABLE: {
        ReportError(UNEXPECTED_START_OF_RESOURCE_TABLE);
        break;
      } 
      case ChunkType::STRING_POOL: {
        ParseStringPool(next_header_p);
        break;
      } 
      case ChunkType::PACKAGE: {
        ParsePackage(next_header_p);
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
