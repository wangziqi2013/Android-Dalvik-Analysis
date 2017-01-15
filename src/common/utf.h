
#pragma once

#ifndef _UTF_H
#define _UTF_H

#include "common.h"

namespace wangziqi2013 {
namespace android_dalvik_analysis { 

/*
 * class UtfString - Base class for UTF strings
 */
class UtfString {
 private:
  // The object does not own the memory
  unsigned char *data_p;
  
  // This stores the length of the string; if not supplied then will be 
  // decoded from the string
  size_t length;
   
 public: 
  /*
   * Constructor
   */
  UtfString(unsigned char *p_data_p, size_t p_length) :
    data_p{p_data_p},
    length{p_length}
  {}
  
  /*
   * Constructor - Leave length field unspecified
   */
  UtfString(unsigned char *p_data_p) :
    data_p{p_data_p},
    length{0UL}
  {}
  
  /*
   * Destructor
   */
  ~UtfString() {}
  
  // Return the length of the string depending on the type
  virtual size_t GetLength() = delete;
  
  // Serialize the string to a given buffer object
  virtual size_t PrintToBuffer(Buffer *buffer_p) = delete;
};

/*
 * Utf8String - UTF-8 encoded string
 */
class Utf8String : public UtfString {
 public:
   
  /*
   * Constructor
   */
  Utf8String(unsigned char *p_data_p, size_t p_length) :
    UtfString{p_data_p},
    UtfString{p_length}
  {}
  
  Utf8String(unsigned char *p_data_p)
  
  /*
   * Destructor
   */
  ~Utf8String() {}
  
  /*
   * GetLength() - Decode length field stored together with string
   *
   * This function targets at UTF8 string's length field, which could be 
   * either 1 byte or 2 bytes, depending on the highest bit on the low byte
   *
   * Return value is next unused byte
   */
  static size_t GetLength() {    
    // Zero extend it to be a size_t
    size_t length = static_cast<size_t>(data_p[0]);
  
    // If the high bit is set then use the first byte as bit 8 - 15
    // and the second byte as byte 0 - 7 to form a 16 bit string length field
    if(length >= 128UL) {
      length = ((length - 128UL) << 8) | static_cast<size_t>(data_p[1]);
    } 
    
    return length;
  }
  
}

} // namespace wangziqi2013
} // namespace android_dalvik_analysis

#endif
