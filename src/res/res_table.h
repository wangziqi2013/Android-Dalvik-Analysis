
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
 private:
  
 public:
  
  /*
   * Constructor
   */
  ResourceTable(unsigned char *p_raw_data_p, 
                size_t p_length, 
                bool p_own_data=false) :
    ResourceBase{p_raw_data_p, p_length, p_own_data}
  {}
  
  /*
   * Destructor
   */
  ~ResourceTable() {
    return; 
  }
};

} // namespace android_dalvik_analysis
} // namespace wangziqi2013

#endif
