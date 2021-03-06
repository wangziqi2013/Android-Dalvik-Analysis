
#pragma once

#ifndef _UTF_H
#define _UTF_H

#include "common.h"

// C++11 for UTF support
#include <codecvt>
#include <locale>

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

  /*
   * Constructor
   */
  UtfString(unsigned char *p_data_p, size_t p_length) :
    data_p{p_data_p},
    length{p_length}
  {}
  
  /*
   * Constructor - Leave length field unspecified
   *
   * This is used by derived classes to compute length later from the data
   */
  UtfString(unsigned char *p_data_p) :
    data_p{p_data_p},
    length{0UL}
  {}
  
  /*
   * Destructor
   */
  ~UtfString() {}
  
  // This scans the string and determines the length by the trailing 0x00
  // series (could be one or two bytes depending on the string type)
  // DO NOT MAKE IT VIRTUAL SINCE IT IS CALLED FROM CONSTRUCTOR
  void ScanForLength() { ReportError(BASE_CLASS_NOT_IMPLEMENTED); }
  
  // Return the length of the string depending on the type
  void DecodeLength() { ReportError(BASE_CLASS_NOT_IMPLEMENTED); }
  
 public:
  
  // Serialize the string to a given buffer object using only Ascii
  // If non-ascii appears then it just report error
  void PrintAscii(Buffer *buffer_p) { 
    (void)buffer_p; 
    ReportError(BASE_CLASS_NOT_IMPLEMENTED);
  } 
  
  // Print in a UTF-8 format
  void PrintUtf8(Buffer *buffer_p) { 
    (void)buffer_p;
    ReportError(BASE_CLASS_NOT_IMPLEMENTED); 
  }
  
  // Return the physical length of the string
  inline size_t GetLength() const {
    return length; 
  }
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
   * ScanForLength() - Scans the string to compute it length
   *
   * UTF-8 has a nice property that the starting byte of a multi-byte character
   * does not share value with continuing bytes. This makes counting characters
   * a lot more easier since we just scan for the pattern only valid for 
   * starting bytes, and ignore the rest.
   *
   * 0x00 is always used as a terminator for the string
   */
  void ScanForLength() {
    unsigned char *p = data_p;
    
    length = 0UL;
    
    while(*p != 0x00) {
      unsigned char ch = *p;
      if((ch & 0x80) == 0x80) {
        // Single byte character: 0xxx xxxx
        p++;
      } else if((ch & 0xE0) == 0xC0) {
        // Two byte starter: 110x xxxx
        p += 2; 
      } else if((ch & 0xF0) == 0xE0) {
        // Three byte starter: 1110 xxxx
        p += 3; 
      } else if((ch & 0xF8) == 0xF0) {
        p += 4; 
      } else {
        ReportError(INVALID_UTF8_CONTINUIATION_BYTE, 
                    p - data_p); 
      }
      
      length++;
    }
    
    return;
  } 
  
 public:
  
  /*
   * Constructor - This will also decode length encoded in either 1 or 2 bytes
   */
  Utf8String(unsigned char *p_data_p, bool length_prefixed=true) :
    UtfString{p_data_p} {
    if(length_prefixed == true) {
      DecodeLength();
    } else {
      ScanForLength(); 
    }
    
    return;
  }
  
  /*
   * Destructor
   */
  ~Utf8String() {}
  
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
  
  /*
   * PrintUtf8() - Prints the UTF-8 to buffer without any modification
   *
   * Since the string itself is UTF-8 what we need to do is just to get
   * it to the buffer using memcpy
   */
  inline void PrintUtf8(Buffer *buffer_p) {
    buffer_p->Append(data_p, length);
    
    return;
  }
}; // class Utf8String

///////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

/*
 * class MUtf8String - This class represents length and character encoding
 *                     of the MUTF-8 format
 *
 * Note that the length is decoded length. For ASCII characters it does not 
 * matter. But for UTF-16 surrogate pairs there will be problems
 */
class MUtf8String : public UtfString {
 public:
  MUtf8String(unsigned char *p_data_p) : 
    UtfString{p_data_p} {
    // Decode the length in ULEB128 form
    DecodeLength();
    
    return;
  }
   
  /*
   * DecodeLength() - Decode length field stored together with string
   *
   * This function targets at UTF8 string's length field, which could be 
   * either 1 byte or 2 bytes, depending on the highest bit on the low byte
   *
   * Return value is next unused byte
   */
  void DecodeLength() {
    // This will also move the data pointer to point to the 
    // actual character data
    length = EncodingUtility::ReadLEB128(&data_p);
    
    return;
  } 
  
  /*
   * ScanForLength() - Not implemented
   */
  void ScanForLength() {
    assert(false);
    
    return;
  }
  
  /*
   * PrintAscii() - Prints ASCII into a buffer
   *
   * This function ignores the length prefix and only uses terminating 0 
   * to print
   *
   * Note that this function works gracefully also with UTF-8 because the
   * internal representation is almost the same with UTF-8
   */
  void PrintAscii(Buffer *buffer_p) {
    int i = 0;
    while(data_p[i] != 0x00) {      
      buffer_p->Append(data_p[i]);
      i++; 
    }
    
    return;
  }
  
  /*
   * PrintUtf8() - Prints the UTF-8 to buffer without any modification
   *
   * Since the string itself is UTF-8 what we need to do is just to get
   * it to the buffer using memcpy
   */
  inline void PrintUtf8(Buffer *buffer_p) {
    // We assume it is already UTF-8
    buffer_p->Append(data_p, length);
    
    return;
  }
};

///////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

/*
 * Utf16String - UTF-16 encoded string
 */
class Utf16String : public UtfString {
 public:
   
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
   * ScanForLength() - Scans the string and find its length
   *
   * This is similar to UTF-8, but for UTF-16 we have a different set of masks
   * and byte sequences for determining the starting byte
   */
  void ScanForLength() {
    // Always read data in uint16_t units
    uint16_t *p = reinterpret_cast<uint16_t *>(data_p);
    
    // UTF-16 also uses 0x0000 as terminating sequence
    while(*p != 0x0000) {
      uint16_t ch = *p;
      if(ch < 0xD800 || ch > 0xDFFF) {
        // According to RFC 2781 this means it is a single unit character
        p++; 
      } else {
        p += 2; 
      }
    }
    
    return;
  }
  
 public:
  
  /*
   * Constructor
   */
  Utf16String(unsigned char *p_data_p, bool length_prefixed=true) :
    UtfString{p_data_p} {
    
    // If there is a prefixed length field then just
    // decode it. Otherwise we need to scan the entire string and 
    // use the trailing 0x00 0x00 to determine length
    if(length_prefixed == true) {
      DecodeLength();
    } else {
      ScanForLength(); 
    }
    
    return;
  }
  
  /*
   * Destructor
   */
  ~Utf16String() {}
  
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
  
  /*
   * PrintUtf8() - Prints the UTF-8 to buffer
   *
   * This function requires converting UTF-16 to UTF-8. Note that currently we
   * just use C++ std library for converting, so it is not in the fatest form
   * e.g. 
   */
  inline void PrintUtf8(Buffer *buffer_p) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> conversion;
    
    // Although a std::string is generated here hopefully it could be 
    // optimized out
    const std::string &utf_8_s = \
      conversion.to_bytes(reinterpret_cast<char16_t *>(data_p));
    
    // Append binary data to the buffer
    buffer_p->Append(utf_8_s.c_str(), utf_8_s.length()); 
    
    return;
  }
}; // class Utf16String

} // namespace android_dalvik_analysis
} // namespace wangziqi2013

#endif
