
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
 protected:
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
  
  /*
   * GetLength() - Returns the length of the string which is either specified
   *               oe decoded from the input stream
   */
  inline size_t GetLength() {
    return length; 
  }
  
  // Return the length of the string depending on the type
  virtual void DecodeLength() = 0;
  
  // Serialize the string to a given buffer object using only Ascii
  // If non-ascii appears then it just report error
  virtual void PrintAscii(Buffer *buffer_p) = 0;
};

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

/*
 * Utf8String - UTF-8 encoded string
 */
class Utf8String : public UtfString {
 public:
   
  /*
   * Constructor
   */
  Utf8String(unsigned char *p_data_p, size_t p_length) :
    UtfString{p_data_p, p_length}
  {}
  
  /*
   * Constructor - This will also decode length encoded in either 1 or 2 bytes
   */
  Utf8String(unsigned char *p_data_p) :
    UtfString{p_data_p} {
    DecodeLength();
    
    return;
  }
  
  /*
   * Destructor
   */
  ~Utf8String() {}
  
  /*
   * DecodeLength() - Decode length field stored together with string
   *
   * This function targets at UTF8 string's length field, which could be 
   * either 1 byte or 2 bytes, depending on the highest bit on the low byte
   *
   * Return value is next unused byte
   */
  void DecodeLength() {    
    // Zero extend it to be a size_t
    length = static_cast<size_t>(data_p[0]);
  
    // If the high bit is set then use the first byte as bit 8 - 15
    // and the second byte as byte 0 - 7 to form a 16 bit string length field
    if(length >= 128UL) {
      length = ((length - 128UL) << 8) | static_cast<size_t>(data_p[1]);
      
      data_p += 2;
    } else {
      data_p += 1;
    }
    
    return;
  }
  
  /*
   * PrintAscii() - Prints ASCII into a buffer
   *
   * If non-ASCII character is encoded inside the string we could detect it
   * and report error 
   */
  void PrintAscii(Buffer *buffer_p) {
    for(size_t i = 0;i < length;i++) {
      if(data_p[i] >= 128) {
        ReportError(ONLY_SUPPORT_ASCII, "8");
      } 
    }
    
    buffer_p->Append(data_p, length);
    
    return;
  }
}; // class Utf8String

///////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

/*
 * Utf16String - UTF-8 encoded string
 */
class Utf16String : public UtfString {
 public:
   
  /*
   * Constructor
   */
  Utf16String(unsigned char *p_data_p, size_t p_length) :
    UtfString{p_data_p, p_length}
  {}
  
  /*
   * Constructor
   */
  Utf16String(unsigned char *p_data_p) :
    UtfString{p_data_p} {
    DecodeLength();
    
    return;
  }
  
  /*
   * Destructor
   */
  ~Utf16String() {}
  
  /*
   * DecodeLength() - Decode length field stored together with string
   *
   * Similar to Utf8String's method. This function decodes 16 or 32 bite string
   */
  void DecodeLength() {    
    // Cast it to 16 bit integer first
    uint16_t *p = reinterpret_cast<uint16_t *>(data_p);
    
    // Zero extend it to be a size_t
    length = static_cast<size_t>(p[0]);
  
    // If the high bit is set then use the first byte as bit 16 - 31
    // and the second byte as byte 0 - 15 to form a 32 bit string length field
    if(length >= 0x8000UL) {
      length = ((length - 0x8000UL) << 16) | static_cast<size_t>(p[1]);
      
      data_p += 4;
    } else {
      data_p += 2;
    }
    
    return;
  }
  
  /*
   * PrintAscii() - Prints ASCII into a buffer
   *
   * This only works for ASCII sequence encoded in UTF-16. This function 
   * extracts the low byte inside a UTF16 unit and send it to the buffer
   */
  void PrintAscii(Buffer *buffer_p) {
    uint16_t *p = reinterpret_cast<uint16_t *>(data_p);
    for(size_t i = 0;i < length;i++) {
      if((p[i] & 0xFF00UL) != 0) {
        ReportError(ONLY_SUPPORT_ASCII, "16");
      } 
    }
    
    // Then append actual bytes into the buffer
    for(size_t i = 0;i < length;i++) {
      // This implicitly chops off higher bytes
      buffer_p->AppendByte(static_cast<unsigned char>(*p)); 
      
      // Advance by 2 bytes each time
      p++;
    }
    
    return;
  }
}; // class Utf16String

} // namespace android_dalvik_analysis
} // namespace wangziqi2013

#endif