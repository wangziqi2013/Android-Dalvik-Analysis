
#pragma once

#ifndef _RES_VALUE_H
#define _RES_VALUE_H

#include "common.h"

namespace wangziqi2013 {
namespace android_dalvik_analysis {

/*
 * class ResourceValue - Typed representation of resource values
 */
class ResourceValue {
 public:
  // The length of this struct
  uint16_t length;
  
  // It is an empty file and should always be 0
  uint8_t zero;
  
  /*
   * enum class DataType - Denotes the type of the data contained in this 
   *                       class instance
   */
  enum class DataType : uint8_t {
    // The 'data' is either 0 or 1, specifying this resource is either
    // undefined or empty, respectively.
    NULL_TYPE = 0x00,
    // The 'data' holds a ResTable_ref, a reference to another resource
    // table entry.
    REFERENCE = 0x01,
    // The 'data' holds an attribute resource identifier.
    ATTRIBUTE = 0x02,
    // The 'data' holds an index into the containing resource table's
    // global value string pool.
    STRING = 0x03,
    // The 'data' holds a single-precision floating point number.
    FLOAT = 0x04,
    // The 'data' holds a complex number encoding a dimension value,
    // such as "100in".
    DIMENSION = 0x05,
    // The 'data' holds a complex number encoding a fraction of a
    // container.
    FRACTION = 0x06,
    // The 'data' holds a dynamic ResTable_ref, which needs to be
    // resolved before it can be used like a TYPE_REFERENCE.
    DYNAMIC_REFERENCE = 0x07,
    // The 'data' holds an attribute resource identifier, which needs to be resolved
    // before it can be used like a TYPE_ATTRIBUTE.
    DYNAMIC_ATTRIBUTE = 0x08,

    // The 'data' is a raw integer value of the form n..n.
    INT_DEC = 0x10,
    // The 'data' is a raw integer value of the form 0xn..n.
    INT_HEX = 0x11,
    // The 'data' is either 0 or 1, for input "false" or "true" respectively.
    INT_BOOLEAN = 0x12,

    // The 'data' is a raw integer value of the form #aarrggbb.
    INT_COLOR_ARGB8 = 0x1c,
    // The 'data' is a raw integer value of the form #rrggbb.
    INT_COLOR_RGB8 = 0x1d,
    // The 'data' is a raw integer value of the form #argb.
    INT_COLOR_ARGB4 = 0x1e,
    // The 'data' is a raw integer value of the form #rgb.
    INT_COLOR_RGB4 = 0x1f,
  };
  
  // This is an enum type
  DataType type;
  
  // This holds actual data
  uint32_t data;
  
  // Possible data values for TYPE_NULL.
  enum {
    // The value is not defined.
    DATA_NULL_UNDEFINED = 0,
    // The value is explicitly defined as empty.
    DATA_NULL_EMPTY = 1
  };
  
  /*
   * AppendToBuffer() - Prints the content in the corresponding format and 
   *                    append it to the buffer
   *
   * Note that this function only processes simple values that do not require
   * extra reference. For resource values that need external reference, e.g.
   * string and reference, processing should be done outside of this function!
   */
  void AppendToBuffer(Buffer *buffer_p) {
    switch(type) {
      case DataType::NULL_TYPE: {
        if(data == DATA_NULL_UNDEFINED) {
          buffer_p->Append("undefined"); 
        } else if(data == DATA_NULL_EMPTY) {
          buffer_p->Append("null"); 
        } else {
          ReportError(UNKNOWN_VALUE_TYPE_NULL, data);
        }
        
        break;
      } // case NULL_TYPE
      case DataType::STRING: {
        // Because it takes an external reference and we could not process it
        // without knowing all the stuff
        assert(false && "Please process STRING type outside this function");
        
        break;
      } // case STRING
      case DataType::FLOAT: {
        // Must cast it as a float pointer and then push the float through
        // the pointer
        // Could not use static_cast because that will invoke FPU to convert
        // it to a logical float value from a logical integer value
        buffer_p->Printf("%f", *reinterpret_cast<float *>(&data));
        
        break;
      } // case FLOAT
      case DataType::INT_DEC: {
        buffer_p->Printf("%d", static_cast<int32_t>(data));
        
        break; 
      } // case DEC
      case DataType::INT_HEX: {
        buffer_p->Printf("0x%X", data);
        
        break; 
      } // case HEX
      case DataType::INT_BOOLEAN: {
        if(data == 0x00000000) {
          buffer_p->Append("false"); 
        } else if(data == 0xFFFFFFFF) {
          buffer_p->Append("true"); 
        } else {
          ReportError(UNKNOWN_BOOLEAN_VALUE, data); 
        }
        
        break; 
      } // case BOOLEAN
      case DataType::INT_COLOR_ARGB8: {
        // #AARRGGBB since Android uses small endian to represent
        // the ARGB seheme we could directly print them as HEX number
        buffer_p->Append("#%08X", data);
        
        break; 
      } // ARGB8
      case DataType::INT_COLOR_RGB8: {
        // Just ignore the high bits and prints low 24 bit (6 hex digits)
        buffer_p->Append("#%06X", data & 0x00FFFFFF);
        
        break; 
      } // RGB8
      case DataType::INT_COLOR_ARGB4: {
        buffer_p->Append('#');
        
        // Print 
        buffer_p->Append("%01X", (data >> 12) & 0x0000000F);
        buffer_p->Append("%01X", (data >> 8) & 0x0000000F);
        buffer_p->Append("%01X", (data >> 4) & 0x0000000F);
        buffer_p->Append("%01X", (data >> 0) & 0x0000000F);
        
        break; 
      } // ARGB4
      case DataType::INT_COLOR_RGB4: {
        buffer_p->Append('#');
        
        // Print lowest 3 bytes
        buffer_p->Append("%01X", (data >> 8) & 0x0000000F);
        buffer_p->Append("%01X", (data >> 4) & 0x0000000F);
        buffer_p->Append("%01X", (data >> 0) & 0x0000000F);
        
        break; 
      } // ARGB4
      default: {
        ReportError(UNSUPPORTED_RESOURCE_VALUE_TYPE, 
                    static_cast<uint32_t>(type),
                    static_cast<uint32_t>(data));
        break;
      } // default
    } // switch type
    
    return;
  }
} BYTE_ALIGNED;

} // namespace android_dalvik_analysis
} // namespace wangziqi2013

#endif
