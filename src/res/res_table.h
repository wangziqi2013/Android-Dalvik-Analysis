
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
  using TableHeader = CommonHeader;

 // Data members  
 private:
  TableHeader *table_header_p;
  
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
    
    while(1) {
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
    
    if(table_header_p->type != ChunkType::RESOURCE_TABLE) {
      dbg_printf("Resource table type 0x%X is wrong (expecting 0x%X)\n", 
                 static_cast<uint32_t>(table_header_p->type), 
                 static_cast<uint32_t>(ChunkType::RESOURCE_TABLE));
      
      return nullptr; 
    } else if(table_header_p->header_length != sizeof(TableHeader)) {
      dbg_printf("Resource table length 0x%X is wrong (expecting 0x%lX)\n", 
                 table_header_p->header_length, 
                 sizeof(TableHeader));
      
      return nullptr;      
    } else if(table_header_p->total_length != length) {
      dbg_printf("XML total length 0x%X is wrong (expecting 0x%lX)\n", 
                 table_header_p->total_length, 
                 length);
      
      // We require that the entire document is part of the XML
      // Otherwise we are unable to decode the rest of it
      return nullptr; 
    } else {
      // Return the next byte and cast it as common header for later parsing
      return reinterpret_cast<CommonHeader *>(
               TypeUtility::Advance(table_header_p, sizeof(TableHeader))); 
    }
    
    assert(false);
    return nullptr;
  }
  
  /*
   * ParseNext() - Parse the contents of the resource table
   *
   * This function has the same structure as the one in class BinaryXml
   */ 
  CommonHeader *ParseNext(CommonHeader *next_header_p) {
    assert(next_header_p != nullptr);
    CommonHeader *ret_header_p = \
      TypeUtility::Advance(next_header_p, next_header_p->total_length);
    
    switch(next_header_p->type) {
      case ChunkType::RESOURCE_TABLE: {
        ReportError(UNEXPECTED_START_OF_RESOURCE_TABLE);
        break;
      } 
      case ChunkType::STRING_POOL: {
        ParseStringPool(next_header_p);
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
};

} // namespace android_dalvik_analysis
} // namespace wangziqi2013

#endif
