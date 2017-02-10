
#include "inst.h"

namespace wangziqi2013 {
namespace android_dalvik_analysis {
namespace dex {

/*
 * BuildInst() - Fills the opcode and the address of the instruction
 *
 * The return value is the new data pointer. Offset could be computed using
 * the starting address of the code segment
 */
uint8_t *BuildInst(uint32_t addr, uint8_t *data_p, InstInfo *info_p) {
  info_p->op = *data_p;
  info_p->addr = addr;
  
  // We use this to receive arguments
  ArgumentPack arg_pack;
  
  InstFormat format = bytecode_type_list[info_p->op];
  data_p = op_jump_table[format](&arg_pack, data_p);
  
  // For 0x00 we need to do some special processing because
  // it may be a data bearing instruction, and if it is the case
  // we need to jump over the data either
  // 
  // Also note that since data_p already contains the first two bytes
  // of the opcode we do not need to advance data_p to jump over them
  // here, so there is a "-2" term for every switch
  if(info_p->op == 0x00) {
    dbg_printf("opcode = 0x00 - Either nop or data bearing instruction\n");
    
    switch(arg_pack.xx) {
      case 0x01: {
        uint16_t size = EncodingUtility::Get16Bit(data_p);
        data_p += (size * 4 + 8 - 2);
        break;
      }
      case 0x02: {
        uint16_t size = EncodingUtility::Get16Bit(data_p);
        data_p += (size * 8 + 4 - 2);
        break;
      }
      case 0x03: {
        uint16_t width = EncodingUtility::Get16Bit(data_p);
        uint32_t size = EncodingUtility::Get32Bit(data_p + 2);
        
        // If the data payload is of odd size then we must round it up
        // to an even number
        data_p += ((((width * size + 1) >> 1) << 1) + 8 - 2);
        break;
      }
      default: {
        break;
      } // default
    } // switch
  } // if
  
  return data_p;
}

}
}
}
