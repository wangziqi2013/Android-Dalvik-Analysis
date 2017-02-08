
#pragma once

#ifndef _DEX_COMMON_H
#define _DEX_COMMON_H

#include "common.h"

namespace wangziqi2013 {
namespace android_dalvik_analysis { 
namespace dex {

// This is the ID of a run time type
using StringId = uint32_t;
using TypeId = uint32_t;
// In the type list, ID is short int
using ShortTypeId = uint16_t;
using ProtoId = uint32_t;
// In method id item the proto ID is short
using ShortProtoId = uint16_t;
using FieldId = uint32_t;
using MethodId = uint32_t;

/*
 * class FieldInfo - stores decoded field metadata
 */
class FieldInfo {
 public: 
  FieldId id;
  uint32_t access_flag;
  
  /*
   * Constructor
   */
  FieldInfo(FieldId p_id, uint32_t p_access_flag) :
    id{p_id},
    access_flag{p_access_flag}
  {}
};

/*
 * class MethodInfo - Stores metadata about methods
 */
class MethodInfo {
  
};

/*
 * class ClassDataInfo - Stored decoded filed information about a class
 */
class ClassDataInfo {
 public:
  
} BYTE_ALIGNED;

} // namespace dex
} // namespace android_dalvik_analysis
} // namespace wangziqi2013

#endif
