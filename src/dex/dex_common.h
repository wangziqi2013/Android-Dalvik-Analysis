
#pragma once

#ifndef _DEX_COMMON_H
#define _DEX_COMMON_H

#include "common.h"
#include "inst.h"

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

// For offset values if they are 0x0 then it means None
static constexpr uint32_t NO_OFFSET = 0x00000000;
// This denotes the case where an index is not available
static constexpr uint32_t NO_INDEX = 0xFFFFFFFF;

/*
 * class FieldInfo - stores decoded field metadata
 */
class FieldInfo {
 public: 
  FieldId id;
  uint32_t access_flags;
};

/*
 * class TryInfo - Try block metadata
 */
class TryInfo {
 public:
  // TO BE ADDED 
};

/*
 * class CodeInfo - The metadata about code for a function
 */
class CodeInfo {
 public:
  uint16_t register_count;
  uint16_t input_word_count;
  uint16_t output_word_count;
  // Add a list of try items and thus its number could be 
  // implicitly expressed
  std::vector<TryInfo> try_list;
  
  // A list of instructions
  std::vector<InstInfo> inst_list;
};

/*
 * class MethodInfo - Stores metadata about methods
 */
class MethodInfo {
 public:
  MethodId id;
  uint32_t access_flags;
  
  // nullptr means there is no code section
  CodeInfo *code_p;
  
  /*
   * Destructor
   */
  ~MethodInfo() {
    if(code_p != nullptr) {
      delete code_p; 
    }
  }
};

/*
 * class ClassInfo - Decoded class metadata in a standalone module (i.e. no
 *                   extra dependencies)
 */
class ClassInfo {
 public:   
  TypeId id; 
  uint32_t access_flags;
  
  // Whether there is a super class defined
  bool has_parent;
  // This is undefined if there is no super class
  TypeId parent_id;
  
  // A list of interfaces this class uses
  std::vector<TypeId> interface_list;
  
  // Need an annotation list here
   
  std::vector<FieldInfo> static_field_list;
  std::vector<FieldInfo> instance_field_list;
  
  std::vector<MethodInfo> direct_method_list;
  std::vector<MethodInfo> virtual_method_list;
  
  // Need static data here
};


} // namespace dex
} // namespace android_dalvik_analysis
} // namespace wangziqi2013

#endif
