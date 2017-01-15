
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
   
 public: 
  /*
   * Constructor
   */
  UtfString(unsigned char *p_data_p) :
    data_p{p_data_p} 
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

} // namespace wangziqi2013
} // namespace android_dalvik_analysis

#endif
