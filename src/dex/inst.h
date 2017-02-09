
#pragma once

#ifndef _INST_H
#define _INST_H

#include "common.h"
#include "inst_format.h"

namespace wangziqi2013 {
namespace android_dalvik_analysis { 
namespace dex {

/*
 * class InstructionInfo - Contains metadata about instructions
 */
class InstructionInfo {
  // The relative address of the instruction inside the current method
  uint32_t addr;
  uint8_t op;
};

} // namespace dex
} // namespace android_dalvik_analysis
} // namespace wangziqi2013

#endif
