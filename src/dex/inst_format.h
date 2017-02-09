
//
// This file is auto-generated using Python script inst_format.py and
// HTML documentation ./html/inst_format.html, in order to automate coding process.
//
// Target file: ./inst_format.h
//
    
#pragma once

#ifndef _INST_FORMAT_H
#define _INST_FORMAT_H

#include "common.h"

namespace wangziqi2013 {
namespace android_dalvik_analysis {
namespace dex {

/*
 * enum class InstFormat - Human readable form of instruction format
 */
enum InstFormat {
  i00x = 0, /* N/A */
  i10x = 1, /* ?? op */
  i12x = 2, /* B A op */
  i11n = 3, /* B A op */
  i11x = 4, /* AA op */
  i10t = 5, /* AA op */
  i20t = 6, /* ?? op AAAA */
  i20bc = 7, /* AA op BBBB */
  i22x = 8, /* AA op BBBB */
  i21t = 9, /* AA op BBBB */
  i21s = 10, /* AA op BBBB */
  i21h = 11, /* AA op BBBB */
  i21c = 12, /* AA op BBBB */
  i23x = 13, /* AA op CC BB */
  i22b = 14, /* AA op CC BB */
  i22t = 15, /* B A op CCCC */
  i22s = 16, /* B A op CCCC */
  i22c = 17, /* B A op CCCC */
  i22cs = 18, /* B A op CCCC */
  i30t = 19, /* ?? op AAAAAAAA */
  i32x = 20, /* ?? op AAAA BBBB */
  i31i = 21, /* AA op BBBBBBBB */
  i31t = 22, /* AA op BBBBBBBB */
  i31c = 23, /* AA op BBBBBBBB */
  i35c = 24, /* A G op BBBB F E D C */
  i35ms = 25, /* A G op BBBB F E D C */
  i35mi = 26, /* A G op BBBB F E D C */
  i3rc = 27, /* AA op BBBB CCCC */
  i3rms = 28, /* AA op BBBB CCCC */
  i3rmi = 29, /* AA op BBBB CCCC */
  i51l = 30, /* AA op BBBBBBBBBBBBBBBB */
};

/*
 * class ArgumentPack - This is used to parse the byte code arguments
 *                      according to different bytecode types
 *
 * All arguments remain in the same format as they were in the compact form
 * of the byte code. In the case where the member is larger than the actual
 * number, only zero-extension is performed
 */
class ArgumentPack {
 public:
  uint64_t B;
  uint32_t A;
  uint16_t C;

  // This is the opcode
  uint8_t op;
  // This is the high order bit of opcode word
  // In some cases this is used as a type identifier for
  // data bearing instruction
  uint8_t xx;

  uint8_t D;
  uint8_t E;
  uint8_t F;
  uint8_t G;
};
    // Jump table declaration
extern uint8_t *(*op_jump_table[31])(ArgumentPack *, uint8_t *);
extern InstFormat bytecode_type_list[256];

} // namespace dex
} // namespace android_dalvik_analysis
} // namespace wangziqi2013

#endif
    