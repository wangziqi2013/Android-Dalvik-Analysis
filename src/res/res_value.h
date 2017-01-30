
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
  // Where the unit type information is.  This gives us 16 possible
  // types, as defined below.
  static constexpr uint32_t COMPLEX_UNIT_SHIFT = 0;
  static constexpr uint32_t COMPLEX_UNIT_MASK = 0xf;

  // TYPE_DIMENSION: Value is raw pixels.
  static constexpr uint32_t COMPLEX_UNIT_PX = 0;
  // TYPE_DIMENSION: Value is Device Independent Pixels.
  static constexpr uint32_t COMPLEX_UNIT_DIP = 1;
  // TYPE_DIMENSION: Value is a Scaled device independent Pixels.
  static constexpr uint32_t COMPLEX_UNIT_SP = 2;
  // TYPE_DIMENSION: Value is in points.
  static constexpr uint32_t COMPLEX_UNIT_PT = 3;
  // TYPE_DIMENSION: Value is in inches.
  static constexpr uint32_t COMPLEX_UNIT_IN = 4;
  // TYPE_DIMENSION: Value is in millimeters.
  static constexpr uint32_t COMPLEX_UNIT_MM = 5;

  // TYPE_FRACTION: A basic fraction of the overall size.
  static constexpr uint32_t COMPLEX_UNIT_FRACTION = 0;
  // TYPE_FRACTION: A fraction of the parent size.
  static constexpr uint32_t COMPLEX_UNIT_FRACTION_PARENT = 1;

  // Where the radix information is, telling where the decimal place
  // appears in the mantissa.  This give us 4 possible fixed point
  // representations as defined below.
  static constexpr uint32_t COMPLEX_RADIX_SHIFT = 4;
  static constexpr uint32_t COMPLEX_RADIX_MASK = 0x3;

  // The mantissa is an integral number -- i.e., 0xnnnnnn.0
  static constexpr uint32_t COMPLEX_RADIX_23p0 = 0;
  // The mantissa magnitude is 16 bits -- i.e, 0xnnnn.nn
  static constexpr uint32_t COMPLEX_RADIX_16p7 = 1;
  // The mantissa magnitude is 8 bits -- i.e, 0xnn.nnnn
  static constexpr uint32_t COMPLEX_RADIX_8p15 = 2;
  // The mantissa magnitude is 0 bits -- i.e, 0x0.nnnnnn
  static constexpr uint32_t COMPLEX_RADIX_0p23 = 3;

  // Where the actual value is.  This gives us 23 bits of
  // precision.  The top bit is the sign.
  static constexpr uint32_t COMPLEX_MANTISSA_SHIFT = 8;
  static constexpr uint32_t COMPLEX_MANTISSA_MASK = 0xffffff;
  
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
        buffer_p->Printf("#%08X", data);
        
        break; 
      } // ARGB8
      case DataType::INT_COLOR_RGB8: {
        // Just ignore the high bits and prints low 24 bit (6 hex digits)
        buffer_p->Printf("#%06X", data & 0x00FFFFFF);
        
        break; 
      } // RGB8
      case DataType::INT_COLOR_ARGB4: {
        buffer_p->Append('#');
        
        // Print 
        buffer_p->Printf("%01X", (data >> 12) & 0x0000000F);
        buffer_p->Printf("%01X", (data >> 8) & 0x0000000F);
        buffer_p->Printf("%01X", (data >> 4) & 0x0000000F);
        buffer_p->Printf("%01X", (data >> 0) & 0x0000000F);
        
        break; 
      } // ARGB4
      case DataType::INT_COLOR_RGB4: {
        buffer_p->Append('#');
        
        // Print lowest 3 bytes
        buffer_p->Printf("%01X", (data >> 8) & 0x0000000F);
        buffer_p->Printf("%01X", (data >> 4) & 0x0000000F);
        buffer_p->Printf("%01X", (data >> 0) & 0x0000000F);
        
        break; 
      } // ARGB4
      case DataType::DIMENSION: 
      case DataType::FRACTION: 
        // Logic is a little bit complicated so write another function 
        // to handle it
        AppendComplexValueToBuffer(buffer_p);  
        break; 
      default: {
        ReportError(UNSUPPORTED_RESOURCE_VALUE_TYPE, 
                    static_cast<uint32_t>(type),
                    static_cast<uint32_t>(data));
        break;
      } // default
    } // switch type
    
    return;
  }
  
 private: 
  
  /*
   * AppendComplexValueToBuffer() - Called for dimention or fraction value
   */
  inline void AppendComplexValueToBuffer(Buffer *buffer_p) {
    assert(type == DataType::DIMENSION || type == DataType::FRACTION);
    
    // First step is to extract the actual numeric part
    // and then print it
    
    // The following was copied from Android run time code with modification:
    // https://github.com/android/platform_frameworks_base/blob/master/libs/androidfw/ResourceTypes.cpp
    
    // These two could be folded by the compiler
    static const float MANTISSA_MULT = \
      1.0f / (1 << COMPLEX_MANTISSA_SHIFT);
    static const float RADIX_MULTS[] = {
      1.0f * MANTISSA_MULT, 1.0f / (1 << 7) * MANTISSA_MULT,
      1.0f / (1 << 15) * MANTISSA_MULT, 1.0f / (1 << 23) * MANTISSA_MULT,
    };

    float value = (data & (COMPLEX_MANTISSA_MASK << \
                   COMPLEX_MANTISSA_SHIFT)) * \
                  RADIX_MULTS[(data >> COMPLEX_RADIX_SHIFT) & \
                              COMPLEX_RADIX_MASK];
    
    buffer_p->Printf("%f", value);
    
    // Then extract the unit and print it
    
    // Extract the unit
    uint32_t unit = (data >> COMPLEX_UNIT_SHIFT) & COMPLEX_UNIT_MASK;
    if(type == DataType::DIMENSION) {
      switch(unit) {
        case COMPLEX_UNIT_PX: buffer_p->Append("px"); break;
        case COMPLEX_UNIT_DIP: buffer_p->Append("dp"); break;
        case COMPLEX_UNIT_SP: buffer_p->Append("sp"); break;
        case COMPLEX_UNIT_PT: buffer_p->Append("pt"); break;
        case COMPLEX_UNIT_IN: buffer_p->Append("in"); break;
        case COMPLEX_UNIT_MM: buffer_p->Append("mm"); break;
        default: {
          ReportError(UNKNOWN_DIMENSION_UNIT, unit);
        } // default
      } // switch
    } else {
      switch(unit) {
        case COMPLEX_UNIT_FRACTION: buffer_p->Append("%%"); break;
        case COMPLEX_UNIT_FRACTION_PARENT: buffer_p->Append("%%p"); break;
        default: {
          ReportError(UNKNOWN_FRACTION_UNIT, unit);
        } // default
      } // switch
    } // if dimension or fraction
    
    return; 
  }
} BYTE_ALIGNED;

} // namespace android_dalvik_analysis
} // namespace wangziqi2013

#endif
