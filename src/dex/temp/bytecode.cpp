
#include "bytecode.h"

/*
 * FormatObjectTable Specifies instruction format
 *
 * It uses implicit conversion from integer (op length) to
 * InstructionFormat instance
 */
BytecodeFormat FormatObjectTable[] = 
{
    [_10x] = 1, 
    [_12x] = 1, 
    [_11n] = 1, 
    [_11x] = 1, 
    [_10t] = 1,
    [_20t] = 3,
    [_20bc] = 3,
    [_22x] = 3,
    [_21t] = 3,
    [_21s] = 3,
    [_21h] = 3,
    [_21c] = 3,
    [_23x] = 3,
    [_22b] = 3,
    [_22t] = 3,
    [_22s] = 3,
    [_22c] = 3,
    [_22cs] = 3,
    [_30t] = 5,
    [_32x] = 5,
    [_31i] = 5,
    [_31t] = 5,
    [_31c] = 5,
    [_35c] = 5,
    [_35ms] = 5,
    [_35mi] = 5,
    [_3rc] = 5,
    [_3rms] = 5,
    [_3rmi] = 5,
    [_51l] = 9,
};

/*
 * Maps byte code to its instruction format
 *
 * Generated using extract_bytecode_format.py
 */ 
int BytecodeFormatTable[256] = 
{
    [0x00] = _10x,
    [0x01] = _12x,
    [0x02] = _22x,
    [0x03] = _32x,
    [0x04] = _12x,
    [0x05] = _22x,
    [0x06] = _32x,
    [0x07] = _12x,
    [0x08] = _22x,
    [0x09] = _32x,
    [0x0a] = _11x,
    [0x0b] = _11x,
    [0x0c] = _11x,
    [0x0d] = _11x,
    [0x0e] = _10x,
    [0x0f] = _11x,
    [0x10] = _11x,
    [0x11] = _11x,
    [0x12] = _11n,
    [0x13] = _21s,
    [0x14] = _31i,
    [0x15] = _21h,
    [0x16] = _21s,
    [0x17] = _31i,
    [0x18] = _51l,
    [0x19] = _21h,
    [0x1a] = _21c,
    [0x1b] = _31c,
    [0x1c] = _21c,
    [0x1d] = _11x,
    [0x1e] = _11x,
    [0x1f] = _21c,
    [0x20] = _22c,
    [0x21] = _12x,
    [0x22] = _21c,
    [0x23] = _22c,
    [0x24] = _35c,
    [0x25] = _3rc,
    [0x26] = _31t,
    [0x27] = _11x,
    [0x28] = _10t,
    [0x29] = _20t,
    [0x2a] = _30t,
    [0x2b] = _31t,
    [0x2c] = _31t,
    [0x2d] = _23x,
    [0x2e] = _23x,
    [0x2f] = _23x,
    [0x30] = _23x,
    [0x31] = _23x,
    [0x32] = _22t,
    [0x33] = _22t,
    [0x34] = _22t,
    [0x35] = _22t,
    [0x36] = _22t,
    [0x37] = _22t,
    [0x38] = _21t,
    [0x39] = _21t,
    [0x3a] = _21t,
    [0x3b] = _21t,
    [0x3c] = _21t,
    [0x3d] = _21t,
    [0x3e] = _10x,
    [0x3f] = _10x,
    [0x40] = _10x,
    [0x41] = _10x,
    [0x42] = _10x,
    [0x43] = _10x,
    [0x44] = _23x,
    [0x45] = _23x,
    [0x46] = _23x,
    [0x47] = _23x,
    [0x48] = _23x,
    [0x49] = _23x,
    [0x4a] = _23x,
    [0x4b] = _23x,
    [0x4c] = _23x,
    [0x4d] = _23x,
    [0x4e] = _23x,
    [0x4f] = _23x,
    [0x50] = _23x,
    [0x51] = _23x,
    [0x52] = _22c,
    [0x53] = _22c,
    [0x54] = _22c,
    [0x55] = _22c,
    [0x56] = _22c,
    [0x57] = _22c,
    [0x58] = _22c,
    [0x59] = _22c,
    [0x5a] = _22c,
    [0x5b] = _22c,
    [0x5c] = _22c,
    [0x5d] = _22c,
    [0x5e] = _22c,
    [0x5f] = _22c,
    [0x60] = _21c,
    [0x61] = _21c,
    [0x62] = _21c,
    [0x63] = _21c,
    [0x64] = _21c,
    [0x65] = _21c,
    [0x66] = _21c,
    [0x67] = _21c,
    [0x68] = _21c,
    [0x69] = _21c,
    [0x6a] = _21c,
    [0x6b] = _21c,
    [0x6c] = _21c,
    [0x6d] = _21c,
    [0x6e] = _35c,
    [0x6f] = _35c,
    [0x70] = _35c,
    [0x71] = _35c,
    [0x72] = _35c,
    [0x73] = _10x,
    [0x74] = _3rc,
    [0x75] = _3rc,
    [0x76] = _3rc,
    [0x77] = _3rc,
    [0x78] = _3rc,
    [0x79] = _10x,
    [0x7a] = _10x,
    [0x7b] = _12x,
    [0x7c] = _12x,
    [0x7d] = _12x,
    [0x7e] = _12x,
    [0x7f] = _12x,
    [0x80] = _12x,
    [0x81] = _12x,
    [0x82] = _12x,
    [0x83] = _12x,
    [0x84] = _12x,
    [0x85] = _12x,
    [0x86] = _12x,
    [0x87] = _12x,
    [0x88] = _12x,
    [0x89] = _12x,
    [0x8a] = _12x,
    [0x8b] = _12x,
    [0x8c] = _12x,
    [0x8d] = _12x,
    [0x8e] = _12x,
    [0x8f] = _12x,
    [0x90] = _23x,
    [0x91] = _23x,
    [0x92] = _23x,
    [0x93] = _23x,
    [0x94] = _23x,
    [0x95] = _23x,
    [0x96] = _23x,
    [0x97] = _23x,
    [0x98] = _23x,
    [0x99] = _23x,
    [0x9a] = _23x,
    [0x9b] = _23x,
    [0x9c] = _23x,
    [0x9d] = _23x,
    [0x9e] = _23x,
    [0x9f] = _23x,
    [0xa0] = _23x,
    [0xa1] = _23x,
    [0xa2] = _23x,
    [0xa3] = _23x,
    [0xa4] = _23x,
    [0xa5] = _23x,
    [0xa6] = _23x,
    [0xa7] = _23x,
    [0xa8] = _23x,
    [0xa9] = _23x,
    [0xaa] = _23x,
    [0xab] = _23x,
    [0xac] = _23x,
    [0xad] = _23x,
    [0xae] = _23x,
    [0xaf] = _23x,
    [0xb0] = _12x,
    [0xb1] = _12x,
    [0xb2] = _12x,
    [0xb3] = _12x,
    [0xb4] = _12x,
    [0xb5] = _12x,
    [0xb6] = _12x,
    [0xb7] = _12x,
    [0xb8] = _12x,
    [0xb9] = _12x,
    [0xba] = _12x,
    [0xbb] = _12x,
    [0xbc] = _12x,
    [0xbd] = _12x,
    [0xbe] = _12x,
    [0xbf] = _12x,
    [0xc0] = _12x,
    [0xc1] = _12x,
    [0xc2] = _12x,
    [0xc3] = _12x,
    [0xc4] = _12x,
    [0xc5] = _12x,
    [0xc6] = _12x,
    [0xc7] = _12x,
    [0xc8] = _12x,
    [0xc9] = _12x,
    [0xca] = _12x,
    [0xcb] = _12x,
    [0xcc] = _12x,
    [0xcd] = _12x,
    [0xce] = _12x,
    [0xcf] = _12x,
    [0xd0] = _22s,
    [0xd1] = _22s,
    [0xd2] = _22s,
    [0xd3] = _22s,
    [0xd4] = _22s,
    [0xd5] = _22s,
    [0xd6] = _22s,
    [0xd7] = _22s,
    [0xd8] = _22b,
    [0xd9] = _22b,
    [0xda] = _22b,
    [0xdb] = _22b,
    [0xdc] = _22b,
    [0xdd] = _22b,
    [0xde] = _22b,
    [0xdf] = _22b,
    [0xe0] = _22b,
    [0xe1] = _22b,
    [0xe2] = _22b,
    [0xe3] = _10x,
    [0xe4] = _10x,
    [0xe5] = _10x,
    [0xe6] = _10x,
    [0xe7] = _10x,
    [0xe8] = _10x,
    [0xe9] = _10x,
    [0xea] = _10x,
    [0xeb] = _10x,
    [0xec] = _10x,
    [0xed] = _10x,
    [0xee] = _10x,
    [0xef] = _10x,
    [0xf0] = _10x,
    [0xf1] = _10x,
    [0xf2] = _10x,
    [0xf3] = _10x,
    [0xf4] = _10x,
    [0xf5] = _10x,
    [0xf6] = _10x,
    [0xf7] = _10x,
    [0xf8] = _10x,
    [0xf9] = _10x,
    [0xfa] = _10x,
    [0xfb] = _10x,
    [0xfc] = _10x,
    [0xfd] = _10x,
    [0xfe] = _10x,
    [0xff] = _10x,
};

/*
 * Constructor - Initialize members and execution environment
 */
BytecodeSegment::BytecodeSegment(unsigned char *pbytecode,
                                 unsigned int pcode_len,
                                 FILE *pout_file,
                                 FILE *perr_file)
{
    this->bytecode = pbytecode;
    this->code_len = pcode_len;
    this->out_file = pout_file;
    this->err_file = perr_file;
    
    this->current_ptr = this->bytecode;
    
    // We do not accept zero length code as valid input
    this->Assert(this->code_len != 0, __LINE__);
    
    return;    
}

/*
 * Destructor
 */
BytecodeSegment::~BytecodeSegment()
{
    return;    
}

/*
 * Assert() - Check condition and fails if it does not satisfy
 *
 * Used for enforcing invariants/restrictions when processing
 * byte code. 
 *
 * Failure recovery should be done elsewhere
 */
void BytecodeSegment::Assert(bool cond, int linenum)
{
    if(!cond)
    {
        fprintf(this->err_file, 
                "Assertion fails @ LINE %d FILE %s\n",
                linenum,
                __FILE__);
        exit(1);
    }
    
    return;
}

/*
 * GetNextByte() - Fetch next byte from input stream
 *
 * Default action is exit immediately with unexpected EOF.
 * Should be overloaded if a different behaviour is desired
 */
inline unsigned char BytecodeSegment::GetNextByte()
{
    if(!this->IsEof())
    {
        return *this->current_ptr++;
    }
    
    fprintf(this->err_file, "GetNextByte(): Unexpected EOF\n");
    this->Assert(false, __LINE__);
    
    return 0;
}

/*
 * GetNextShort() - Get next short from input
 *
 * Either bytes must be in valid range
 */
inline unsigned short BytecodeSegment::GetNextShort()
{
    unsigned char byte1 = this->GetNextByte();
    unsigned char byte2 = this->GetNextByte();
    
    return ((unsigned short)byte2 << 8)    | (unsigned short)byte1;
}

/*
 * GetNextInt() - Get next 32 bit value from input
 *
 * Each of the four bytes must be in valid range
 */
inline unsigned int BytecodeSegment::GetNextInt()
{
    unsigned short word1 = this->GetNextShort();
    unsigned short word2 = this->GetNextShort();
    
    return ((unsigned int)word2 << 16) | (unsigned int)word1;    
}

/*
 * GetNextNBytes() - Read next N bytes into a buffer
 *
 * This method checks for EOF on each byte, and if unexpected EOF
 * is seen assertion fails immediately
 */
inline void BytecodeSegment::GetNextNBytes(unsigned char *buffer, 
                                           int size)
{
    // This will check for EOF on each access which is mostly desired
    for(int i = 0;i < size;i++)
    {
        buffer[i] = this->GetNextByte();    
    }
    
    return;
}

/*
 * IsEof() - Whether there is anything to read
 *
 * Returns true if current pointer points to an invalid byte
 * false otherwise
 */
inline bool BytecodeSegment::IsEof()
{
    return (this->current_ptr - this->bytecode) >= this->code_len;
}

/*
 * PrintLineNum() - Print current instruction number
 *
 * Will print in left justified, 8 digit hex format plus a space
 */
inline void BytecodeSegment::PrintLineNum()
{
    fprintf(this->out_file, 
            "(0x%.8X) %-8d ", 
            this->current_opcode_offset,
            this->instruction_counter);
    return;    
}

/*
 * PrintBuffer() - Print a buffer in small endian
 *
 * We start from the end of the array and keep printing until
 * the first byte is printed
 */
void BytecodeSegment::PrintBuffer(unsigned char *buffer, 
                                  int size)
{
    // Get to the last byte
    unsigned char *ptr = buffer + size - 1;
    while(size > 0)
    {
        fprintf(this->out_file, "%.2X", *ptr);
        ptr--;
        size--;    
    }
    
    return;
}

/*
 * ResolveOffsetToCount() - Return instruction counter pointed to
 * by the offset
 *
 * If the offset is invalid (i.e. inside an instruction) then
 * assertion fail
 */
unsigned int BytecodeSegment::ResolveOffsetToCount(unsigned int base, 
                                                   int offset)
{
    unsigned int target = (unsigned int)((int)base + offset);
    map<unsigned int, unsigned int>::iterator it = \
        this->instruction_offset_map.find(target);
    if(it == this->instruction_offset_map.end())
    {
        fprintf(this->err_file, 
                "ResolveOffsetToCount(): Unable to resolve target: 0x%.8X\n",
                target);
    }
    
    return it->second;
}

////////////////////////////////////////////////////////////////
// Call back function block start //////////////////////////////
////////////////////////////////////////////////////////////////

// The following code does not have comment header
// Please refer to the header file for information

/*
 * OnStart() - Called when dispatching starts
 *
 * Set environmental variables such as starting counter, starting
 * offset and offset-counter map
 */
void BytecodeSegment::OnStart()
{
    // Line number starts at 1, but we initialize it to 0
    // since it will be increased by 1 before printing
    this->instruction_counter = 0;
    this->current_opcode_offset = 0;
    this->instruction_counter_map.clear();
    this->instruction_offset_map.clear();
    
    this->PrintLineNum();
    fprintf(this->out_file, "Dispatch starts\n");
    
    return;
}

/*
 * OnFinish() - Called when diapatch finishes
 *
 * The current pointer now points to the last byte + 1 position
 */
void BytecodeSegment::OnFinish()
{
    this->PrintLineNum();
    fprintf(this->out_file, "Dispatch finishes\n");
    
    return;
}

/*
 * OnSkip() - Called when an instruction is skipped
 */
void BytecodeSegment::OnSkip()
{
    this->PrintLineNum();
    fprintf(this->out_file, "[Skipped]\n");
    
    return;    
}

/*
 * OnNextInstruction() - Called when a new instruction is retrieved
 *
 * Used to update instruction offset and program count
 * This function will be called after OnStart() and byte code
 * retrival, but before any further action is taken
 */
void BytecodeSegment::OnNextInstruction(unsigned char opcode)
{
    // This counter is initialized to 0, so in order to
    // start at instruction 1 we need to increase first
    this->instruction_counter++;
    
    // This is the offset of current opcode offset even if there
    // are operands it always points to opcode
    // This is useful as the base address of goto and branching instruction
    this->current_opcode_offset = this->current_ptr - this->bytecode - 1; 
    // Maps current instruction byte offset in input stream to
    // integral instruction count, starting from 1
    // Need to -1 on pointer difference since we have already fetched the
    // bytecode
    this->instruction_offset_map[this->current_opcode_offset] = \
        this->instruction_counter;
        
    // Also maps current instruction counter to offset
    this->instruction_counter_map[this->instruction_counter] = \
        this->current_opcode_offset;
        
    return;
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

void BytecodeSegment::OnNop(unsigned char null)
{
    // For nop instruction we require its high byte being 0x00
    this->Assert(null == 0x00, __LINE__);
    
    this->PrintLineNum();
    fprintf(this->out_file, "nop\n");
}

/*
 * OnPackedSwitchPayload() - Parse a switch jump table
 *
 * ushort: item size
 * int[item_size]: values
 * int[item_size]: target offset relative to the opcode
 */
void BytecodeSegment::OnPackedSwitchPayload(unsigned short size)
{
    this->PrintLineNum();
    fprintf(this->out_file, "packed-switch-payload, size = %u\n", size);
    int first_key = (int)this->GetNextInt();
    
    for(unsigned int i = 0;i < size;i++)
    {
        this->PrintLineNum();
        int offset = (int)this->GetNextInt();
        fprintf(this->out_file, 
                "%d: 0x%.8X\n", 
                first_key + i,
                (int)this->current_opcode_offset + offset * 2);    
    }
    
    return;
}

/*
 * OnSparseSwitchPayload() - Parse jump table for sparse valued switch
 */
void BytecodeSegment::OnSparseSwitchPayload(unsigned short size)
{
    this->PrintLineNum();
    fprintf(this->out_file, "sparse-switch-payload, size = %u\n", size);
    int *buffer = new(int [size]);
    
    // This is values array
    this->GetNextNBytes((unsigned char *)buffer, size * sizeof(int));    
    
    for(unsigned int i = 0;i < size;i++)
    {
        this->PrintLineNum();
        int offset = (int)this->GetNextInt();
        
        fprintf(this->out_file,
                "%d: 0x%.8X\n",
                buffer[i],
                (int)this->current_opcode_offset + offset * 2);
    }
    
    delete[] buffer;
    
    return;
}

void BytecodeSegment::OnMove(unsigned char dest, unsigned char src)
{
    this->PrintLineNum();
    fprintf(this->out_file, "move v%u, v%u\n", dest, src);
}

void BytecodeSegment::OnMoveFrom16(unsigned char dest, unsigned short src)
{
    this->PrintLineNum();
    fprintf(this->out_file, "move/from16 v%u, v%u\n", dest, src);
}

void BytecodeSegment::OnMove16(unsigned short dest, unsigned short src)
{
    this->PrintLineNum();
    fprintf(this->out_file, "move/16 v%u, v%u\n", dest, src);    
}

void BytecodeSegment::OnMoveWide(unsigned char dest, unsigned char src)
{
    this->PrintLineNum();
    fprintf(this->out_file, "move-wide v%u, v%u\n", dest, src);    
}

void BytecodeSegment::OnMoveWideFrom16(unsigned char dest, unsigned short src)
{
    this->PrintLineNum();
    fprintf(this->out_file, "move-wide/from16 v%u, v%u\n", dest, src);
}

void BytecodeSegment::OnMoveWide16(unsigned short dest, unsigned short src)
{
    this->PrintLineNum();
    fprintf(this->out_file, "move-wide/16 v%u, v%u\n", dest, src);    
}

void BytecodeSegment::OnMoveObject(unsigned char dest, unsigned char src)
{
    this->PrintLineNum();
    fprintf(this->out_file, "move-object v%u, v%u\n", dest, src);
}

void BytecodeSegment::OnMoveObjectFrom16(unsigned char dest, unsigned short src)
{
    this->PrintLineNum();
    fprintf(this->out_file, "move-object/from16 v%u, v%u\n", dest, src);
}

void BytecodeSegment::OnMoveObject16(unsigned short dest, unsigned short src)
{
    this->PrintLineNum();
    fprintf(this->out_file, "move-object/16 v%u, v%u\n", dest, src);    
}

void BytecodeSegment::OnMoveResult(unsigned char dest)
{
    this->PrintLineNum();
    fprintf(this->out_file, "move-result v%u\n", dest);    
}

void BytecodeSegment::OnMoveResultWide(unsigned char dest)
{
    this->PrintLineNum();
    fprintf(this->out_file, "move-result-wide v%u\n", dest);    
}

void BytecodeSegment::OnMoveResultObject(unsigned char dest)
{
    this->PrintLineNum();
    fprintf(this->out_file, "move-result-object v%u\n", dest);    
}

void BytecodeSegment::OnMoveException(unsigned char dest)
{
    this->PrintLineNum();
    fprintf(this->out_file, "move-exception v%u\n", dest);
}

void BytecodeSegment::OnReturnVoid(unsigned char null)
{
    this->Assert(null == 0x00, __LINE__);
    this->PrintLineNum();
    fprintf(this->out_file, "return-void\n");    
}

void BytecodeSegment::OnReturn(unsigned char dest)
{
    this->PrintLineNum();
    fprintf(this->out_file, "return v%u\n", dest);    
}

void BytecodeSegment::OnReturnWide(unsigned char dest)
{
    this->PrintLineNum();
    fprintf(this->out_file, "return-wide v%u\n", dest);    
}

void BytecodeSegment::OnReturnObject(unsigned char dest)
{
    this->PrintLineNum();
    fprintf(this->out_file, "return-object v%u\n", dest);    
}

void BytecodeSegment::OnConst4(unsigned char dest, int value)
{
    this->PrintLineNum();
    fprintf(this->out_file, "const/4 v%u, %d\n", dest, value);    
}

void BytecodeSegment::OnConst16(unsigned char dest, int value)
{
    this->PrintLineNum();
    fprintf(this->out_file, "const/16 v%u, %d\n", dest, value);    
}

void BytecodeSegment::OnConst(unsigned char dest, unsigned int value)
{
    this->PrintLineNum();
    fprintf(this->out_file, "const v%u, 0x%.8X\n", dest, value);
}

void BytecodeSegment::OnConstHigh16(unsigned char dest, unsigned int value)
{
    this->PrintLineNum();
    fprintf(this->out_file, "const/high16 v%u, 0x%.8X\n", dest, value);    
}

void BytecodeSegment::OnConstWide16(unsigned char dest, long long value)
{
    this->PrintLineNum();
    fprintf(this->out_file, "const-wide/16 v%u, %lld\n", dest, value);    
}

void BytecodeSegment::OnConstWide32(unsigned char dest, long long value)
{
    this->PrintLineNum();
    fprintf(this->out_file, "const-wide/32 v%u, %lld\n", dest, value);    
}

void BytecodeSegment::OnConstWide(unsigned char dest, unsigned long long value)
{
    this->PrintLineNum();
    fprintf(this->out_file, "const-wide v%u, 0x%.8X\n", dest, value);    
}

void BytecodeSegment::OnConstWideHigh16(unsigned char dest, 
                                        unsigned long long value)
{
    this->PrintLineNum();
    fprintf(this->out_file, "const-wide/high16 v%u, 0x%.8X\n", dest, value);
}

void BytecodeSegment::OnConstString(unsigned char dest, unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file, "const-string v%u, string@%u\n", dest, index);    
}

void BytecodeSegment::OnConstStringJumbo(unsigned char dest, unsigned int index)
{
    this->PrintLineNum();
    fprintf(this->out_file, "const-string/jumbo v%u, string@%u\n", dest, index);    
}

void BytecodeSegment::OnConstClass(unsigned char dest, unsigned short index)
{
    this->PrintLineNum();
    // That's actually an index into class table
    fprintf(this->out_file, "const-class v%u, type@%u\n", dest, index);    
}

void BytecodeSegment::OnMonitorEnter(unsigned char reg)
{
    this->PrintLineNum();
    fprintf(this->out_file, "monitor-enter v%u\n", reg);
}

void BytecodeSegment::OnMonitorExit(unsigned char reg)
{
    this->PrintLineNum();
    fprintf(this->out_file, "monitor-exit v%u\n", reg);    
}

void BytecodeSegment::OnCheckCast(unsigned char reg, unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file, "check-cast v%u, type@%u\n", reg, index);    
}

void BytecodeSegment::OnInstanceOf(unsigned char dest, 
                                   unsigned char reg, 
                                   unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file, "instance-of v%u, v%u, type@%u\n", dest, reg, index);    
}

void BytecodeSegment::OnArrayLength(unsigned char dest, unsigned char reg)
{
    this->PrintLineNum();
    fprintf(this->out_file, "array-length v%u, v%u\n", dest, reg);    
}

void BytecodeSegment::OnNewInstance(unsigned char dest, unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file, "new-instance v%u, type@%u\n", dest, index);    
}

void BytecodeSegment::OnNewArray(unsigned char dest, 
                                 unsigned char size, 
                                 unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file, "new-array v%u, v%u, type@%u\n", dest, size, index);
}

void BytecodeSegment::OnInvokeVirtualRange(unsigned short index, 
                                             unsigned short start,
                                             unsigned char count)
{
    this->PrintLineNum();
    fprintf(this->out_file, 
            "invoke-virtual/range{v%.4X..v%.4X}, method@%u\n",
            start,
            start + count - 1,
            index);    
}

void BytecodeSegment::OnInvokeSuperRange(unsigned short index, 
                                         unsigned short start,
                                         unsigned char count)
{
    this->PrintLineNum();
    fprintf(this->out_file, 
            "invoke-super/range{v%.4X..v%.4X}, method@%u\n",
            start,
            start + count - 1,
            index);    
}

void BytecodeSegment::OnInvokeDirectRange(unsigned short index, 
                                           unsigned short start,
                                           unsigned char count)
{
    this->PrintLineNum();
    fprintf(this->out_file, 
            "invoke-direct/range{v%.4X..v%.4X}, method@%u\n",
            start,
            start + count - 1,
            index);    
}

void BytecodeSegment::OnInvokeStaticRange(unsigned short index, 
                                           unsigned short start,
                                           unsigned char count)
{
    this->PrintLineNum();
    fprintf(this->out_file, 
            "invoke-static/range{v%.4X..v%.4X}, method@%u\n",
            start,
            start + count - 1,
            index);    
}

void BytecodeSegment::OnInvokeInterfaceRange(unsigned short index, 
                                             unsigned short start,
                                             unsigned char count)
{
    this->PrintLineNum();
    fprintf(this->out_file, 
            "invoke-interface/range{v%.4X..v%.4X}, method@%u\n",
            start,
            start + count - 1,
            index);    
}

void BytecodeSegment::OnFilledNewArray(unsigned char size, 
                                        unsigned char c, 
                                         unsigned char d,
                                         unsigned char e,
                                         unsigned char f,
                                         unsigned char g,
                                         unsigned short type)
{
    this->PrintLineNum();
    unsigned char reg_array[5] = {c, d, e, f, g};
    const char *sep = "";
    
    fprintf(this->out_file, "filled-new-array {");
    for(int i = 0;i < size;i++)
    {
        fprintf(this->out_file, "%sv%u", sep, reg_array[i]);
        sep = ", ";    
    }
    
    fprintf(this->out_file, "}, type@%u\n", type);
}

void BytecodeSegment::OnFilledNewArrayRange(unsigned char size,
                                            unsigned short type,
                                            unsigned short start)
{
    this->PrintLineNum();
    fprintf(this->out_file, 
            "filled-new-array/range {v%u..v%u}, type@%u",
            start,
            start + size - 1,
            type);
}

void BytecodeSegment::OnFillArrayData(unsigned char reg,
                                      int offset)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "fill-array-data v%u, %d\n", 
            reg, 
            offset * 2);
}

/*
 * OnFillArrayDataPayload() - Read data embedded in instruction
 *
 * This is not a callback for opcode, but rather for embedded data
 * The length of this array is aligned to 16-bit words which
 * implies that if width * size is odd then there is a 1 byte
 * padding after this structure
 */
void BytecodeSegment::OnFillArrayDataPayload(unsigned short width, 
                                             unsigned int size)
{
    this->PrintLineNum();
    fprintf(this->out_file, 
            "fill-array-data-payload, width = %u, size = %u\n",
            width,
            size);
    
    unsigned char *buffer = new(unsigned char [width]);
    for(unsigned int i = 0;i < size;i++)
    {
        // This would make sure they are aligned in output file
        this->PrintLineNum();
        this->GetNextNBytes(buffer, width);
        this->PrintBuffer(buffer, width);
        fprintf(this->out_file, "\n");
    }
    
    delete[] buffer;
    
    // If the total length is odd we should also read the extra 1 byte
    if((width * size) % 2 == 1)
    {
        this->PrintLineNum();
        fprintf(this->out_file, "padding: %.2X\n", this->GetNextByte());
    }
}

void BytecodeSegment::OnThrow(unsigned char reg)
{
    this->PrintLineNum();
    fprintf(this->out_file, "throw v%u\n", reg);    
}

void BytecodeSegment::OnGoto(char offset)
{
    this->PrintLineNum();
    fprintf(this->out_file, 
            "goto 0x%.8X\n", 
            (int)this->current_opcode_offset + (int)offset * 2);
}

void BytecodeSegment::OnGoto16(short offset, 
                               unsigned char null)
{
    this->Assert(null == 0, __LINE__);
    
    this->PrintLineNum();
    fprintf(this->out_file, 
            "goto/16 0x%.8X\n", 
            (int)this->current_opcode_offset + (int)offset * 2);
}

void BytecodeSegment::OnGoto32(int offset, unsigned char null)
{
    this->Assert(null == 0x00, __LINE__);
    
    this->PrintLineNum();
    fprintf(this->out_file,
            "goto/32 0x%.8X\n",
            (int)this->current_opcode_offset + offset * 2);    
}

void BytecodeSegment::OnPackedSwitch(unsigned char reg, int offset)
{
    this->PrintLineNum();
    fprintf(this->out_file, 
            "packed-switch v%u, 0x%.8X\n",
            reg, 
            (int)this->current_opcode_offset + offset * 2);
}

void BytecodeSegment::OnSparseSwitch(unsigned char reg, int offset)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "sparse-switch v%u, 0x%.8X\n",
            reg,
            (int)this->current_opcode_offset + offset * 2);    
}

void BytecodeSegment::OnCmplFloat(unsigned char dest, 
                                   unsigned char reg1, 
                                   unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "cmpl-float v%u, v%u, v%u\n",
            dest,
            reg1,
            reg2);
}

void BytecodeSegment::OnCmpgFloat(unsigned char dest, 
                                   unsigned char reg1, 
                                   unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "cmpg-float v%u, v%u, v%u\n",
            dest,
            reg1,
            reg2);
}

void BytecodeSegment::OnCmplDouble(unsigned char dest, 
                                    unsigned char reg1, 
                                    unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "cmpl-double v%u, v%u, v%u\n",
            dest,
            reg1,
            reg2);
}

void BytecodeSegment::OnCmpgDouble(unsigned char dest, 
                                    unsigned char reg1, 
                                    unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "cmpg-double v%u, v%u, v%u\n",
            dest,
            reg1,
            reg2);
}

void BytecodeSegment::OnCmpLong(unsigned char dest, 
                                 unsigned char reg1, 
                                 unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "cmp-long v%u, v%u, v%u\n",
            dest,
            reg1,
            reg2);
}

void BytecodeSegment::OnIfEq(unsigned char reg1, 
                             unsigned char reg2,
                             int offset)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "if-eq v%u, v%u, 0x%.8X\n",
            reg1, 
            reg2,
            (int)this->current_opcode_offset + offset * 2);
}

void BytecodeSegment::OnIfNe(unsigned char reg1, 
                             unsigned char reg2,
                             int offset)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "if-ne v%u, v%u, 0x%.8X\n",
            reg1, 
            reg2,
            (int)this->current_opcode_offset + offset * 2);
}

void BytecodeSegment::OnIfLt(unsigned char reg1, 
                             unsigned char reg2,
                             int offset)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "if-lt v%u, v%u, 0x%.8X\n",
            reg1, 
            reg2,
            (int)this->current_opcode_offset + offset * 2);
}

void BytecodeSegment::OnIfGe(unsigned char reg1, 
                             unsigned char reg2,
                             int offset)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "if-ge v%u, v%u, 0x%.8X\n",
            reg1, 
            reg2,
            (int)this->current_opcode_offset + offset * 2);
}

void BytecodeSegment::OnIfGt(unsigned char reg1, 
                             unsigned char reg2,
                             int offset)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "if-gt v%u, v%u, 0x%.8X\n",
            reg1, 
            reg2,
            (int)this->current_opcode_offset + offset * 2);
}

void BytecodeSegment::OnIfLe(unsigned char reg1, 
                             unsigned char reg2,
                             int offset)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "if-le v%u, v%u, 0x%.8X\n",
            reg1, 
            reg2,
            (int)this->current_opcode_offset + offset * 2);
}

void BytecodeSegment::OnIfNeZ(unsigned char reg, 
                               int offset)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "if-nez v%u, 0x%.8X\n",
            reg,
            (int)this->current_opcode_offset + offset * 2);    
}

void BytecodeSegment::OnIfLtZ(unsigned char reg, 
                               int offset)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "if-ltz v%u, 0x%.8X\n",
            reg,
            (int)this->current_opcode_offset + offset * 2);    
}

void BytecodeSegment::OnIfGeZ(unsigned char reg, 
                               int offset)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "if-gez v%u, 0x%.8X\n",
            reg,
            (int)this->current_opcode_offset + offset * 2);    
}

void BytecodeSegment::OnIfGtZ(unsigned char reg, 
                               int offset)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "if-gtz v%u, 0x%.8X\n",
            reg,
            (int)this->current_opcode_offset + offset * 2);    
}

void BytecodeSegment::OnIfLeZ(unsigned char reg, 
                               int offset)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "if-lez v%u, 0x%.8X\n",
            reg,
            (int)this->current_opcode_offset + offset * 2);    
}

void BytecodeSegment::OnIfEqZ(unsigned char reg, 
                               int offset)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "if-eqz v%u, 0x%.8X\n",
            reg,
            (int)this->current_opcode_offset + offset * 2);    
}

void BytecodeSegment::OnUnused(unsigned char null)
{
    this->PrintLineNum();
    fprintf(this->out_file, 
            "*unused*\n");
}

void BytecodeSegment::OnAGet(unsigned char reg,
                             unsigned char array,
                             unsigned char index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "aget v%u, v%u, v%u\n",
            reg,
            array,
            index);    
}

void BytecodeSegment::OnAGetWide(unsigned char reg,
                                  unsigned char array,
                                  unsigned char index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "aget-wide v%u, v%u, v%u\n",
            reg,
            array,
            index);    
}

void BytecodeSegment::OnAGetObject(unsigned char reg,
                                    unsigned char array,
                                    unsigned char index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "aget-object v%u, v%u, v%u\n",
            reg,
            array,
            index);    
}

void BytecodeSegment::OnAGetBoolean(unsigned char reg,
                                     unsigned char array,
                                     unsigned char index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "aget-boolean v%u, v%u, v%u\n",
            reg,
            array,
            index);    
}

void BytecodeSegment::OnAGetByte(unsigned char reg,
                                  unsigned char array,
                                  unsigned char index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "aget-byte v%u, v%u, v%u\n",
            reg,
            array,
            index);    
}

void BytecodeSegment::OnAGetChar(unsigned char reg,
                                  unsigned char array,
                                  unsigned char index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "aget-char v%u, v%u, v%u\n",
            reg,
            array,
            index);    
}

void BytecodeSegment::OnAGetShort(unsigned char reg,
                                   unsigned char array,
                                   unsigned char index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "aget-short v%u, v%u, v%u\n",
            reg,
            array,
            index);    
}

void BytecodeSegment::OnAPut(unsigned char reg,
                             unsigned char array,
                             unsigned char index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "aput v%u, v%u, v%u\n",
            reg,
            array,
            index);    
}

void BytecodeSegment::OnAPutWide(unsigned char reg,
                                  unsigned char array,
                                  unsigned char index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "aput-wide v%u, v%u, v%u\n",
            reg,
            array,
            index);    
}

void BytecodeSegment::OnAPutObject(unsigned char reg,
                                    unsigned char array,
                                    unsigned char index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "aput-object v%u, v%u, v%u\n",
            reg,
            array,
            index);    
}

void BytecodeSegment::OnAPutBoolean(unsigned char reg,
                                     unsigned char array,
                                     unsigned char index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "aput-boolean v%u, v%u, v%u\n",
            reg,
            array,
            index);    
}

void BytecodeSegment::OnAPutByte(unsigned char reg,
                                  unsigned char array,
                                  unsigned char index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "aput-byte v%u, v%u, v%u\n",
            reg,
            array,
            index);    
}

void BytecodeSegment::OnAPutChar(unsigned char reg,
                                  unsigned char array,
                                  unsigned char index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "aput-char v%u, v%u, v%u\n",
            reg,
            array,
            index);    
}

void BytecodeSegment::OnAPutShort(unsigned char reg,
                                   unsigned char array,
                                   unsigned char index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "aput-short v%u, v%u, v%u\n",
            reg,
            array,
            index);    
}

void BytecodeSegment::OnIGet(unsigned char reg,
                             unsigned char obj,
                             unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "iget v%u, v%u, field@%u\n",
            reg,
            obj,
            index);    
}

void BytecodeSegment::OnIGetWide(unsigned char reg,
                                  unsigned char obj,
                                  unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "iget-wide v%u, v%u, field@%u\n",
            reg,
            obj,
            index);    
}

void BytecodeSegment::OnIGetObject(unsigned char reg,
                                    unsigned char obj,
                                    unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "iget-object v%u, v%u, field@%u\n",
            reg,
            obj,
            index);    
}

void BytecodeSegment::OnIGetBoolean(unsigned char reg,
                                     unsigned char obj,
                                     unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "iget-boolean v%u, v%u, field@%u\n",
            reg,
            obj,
            index);    
}

void BytecodeSegment::OnIGetByte(unsigned char reg,
                                  unsigned char obj,
                                  unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "iget-byte v%u, v%u, field@%u\n",
            reg,
            obj,
            index);    
}

void BytecodeSegment::OnIGetChar(unsigned char reg,
                                  unsigned char obj,
                                  unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "iget-char v%u, v%u, field@%u\n",
            reg,
            obj,
            index);    
}

void BytecodeSegment::OnIGetShort(unsigned char reg,
                                   unsigned char obj,
                                   unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "iget-short v%u, v%u, field@%u\n",
            reg,
            obj,
            index);    
}

void BytecodeSegment::OnIPut(unsigned char reg,
                             unsigned char obj,
                             unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "iput v%u, v%u, field@%u\n",
            reg,
            obj,
            index);    
}

void BytecodeSegment::OnIPutWide(unsigned char reg,
                                  unsigned char obj,
                                  unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "iput-wide v%u, v%u, field@%u\n",
            reg,
            obj,
            index);    
}

void BytecodeSegment::OnIPutObject(unsigned char reg,
                                    unsigned char obj,
                                    unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "iput-object v%u, v%u, field@%u\n",
            reg,
            obj,
            index);    
}

void BytecodeSegment::OnIPutBoolean(unsigned char reg,
                                     unsigned char obj,
                                     unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "iput-boolean v%u, v%u, field@%u\n",
            reg,
            obj,
            index);    
}

void BytecodeSegment::OnIPutByte(unsigned char reg,
                                  unsigned char obj,
                                  unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "iput-byte v%u, v%u, field@%u\n",
            reg,
            obj,
            index);    
}

void BytecodeSegment::OnIPutChar(unsigned char reg,
                                  unsigned char obj,
                                  unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "iput-char v%u, v%u, field@%u\n",
            reg,
            obj,
            index);    
}

void BytecodeSegment::OnIPutShort(unsigned char reg,
                                   unsigned char obj,
                                   unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "iput-short v%u, v%u, field@%u\n",
            reg,
            obj,
            index);    
}

void BytecodeSegment::OnSGet(unsigned char reg,
                             unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "sget v%u, field@%u\n",
            reg,
            index);    
}

void BytecodeSegment::OnSGetWide(unsigned char reg,
                                  unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "sget-wide v%u, field@%u\n",
            reg,
            index);    
}

void BytecodeSegment::OnSGetObject(unsigned char reg,
                                    unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "sget-object v%u, field@%u\n",
            reg,
            index);    
}

void BytecodeSegment::OnSGetBoolean(unsigned char reg,
                                     unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "sget-boolean v%u, field@%u\n",
            reg,
            index);    
}

void BytecodeSegment::OnSGetByte(unsigned char reg,
                                  unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "sget-byte v%u, field@%u\n",
            reg,
            index);    
}

void BytecodeSegment::OnSGetChar(unsigned char reg,
                                  unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "sget-char v%u, field@%u\n",
            reg,
            index);    
}

void BytecodeSegment::OnSGetShort(unsigned char reg,
                                   unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "sget-short v%u, field@%u\n",
            reg,
            index);    
}

void BytecodeSegment::OnSPut(unsigned char reg,
                             unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "sput v%u, field@%u\n",
            reg,
            index);    
}

void BytecodeSegment::OnSPutWide(unsigned char reg,
                                  unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "sput-wide v%u, field@%u\n",
            reg,
            index);    
}

void BytecodeSegment::OnSPutObject(unsigned char reg,
                                    unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "sput-object v%u, field@%u\n",
            reg,
            index);    
}

void BytecodeSegment::OnSPutBoolean(unsigned char reg,
                                     unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "sput-boolean v%u, field@%u\n",
            reg,
            index);    
}

void BytecodeSegment::OnSPutByte(unsigned char reg,
                                  unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "sput-byte v%u, field@%u\n",
            reg,
            index);    
}

void BytecodeSegment::OnSPutChar(unsigned char reg,
                                  unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "sput-char v%u, field@%u\n",
            reg,
            index);    
}

void BytecodeSegment::OnSPutShort(unsigned char reg,
                                   unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "sput-short v%u, field@%u\n",
            reg,
            index);    
}

void BytecodeSegment::OnInvokeVirtual(unsigned char size,
                                      unsigned char c,
                                       unsigned char d,
                                       unsigned char e,
                                       unsigned char f,
                                       unsigned char g,
                                       unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "invoke-virtual {");
    unsigned char buffer[5] = {c, d, e, f, g};
    const char *sep = "";
    for(int i = 0;i < size;i++)
    {
        fprintf(this->out_file, "%sv%u", sep, buffer[i]);
        sep = ", ";
    }
    
    fprintf(this->out_file, "}, meth@%u\n", index);
}

void BytecodeSegment::OnInvokeSuper(unsigned char size,
                                    unsigned char c,
                                     unsigned char d,
                                     unsigned char e,
                                     unsigned char f,
                                     unsigned char g,
                                     unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "invoke-super {");
    unsigned char buffer[5] = {c, d, e, f, g};
    const char *sep = "";
    for(int i = 0;i < size;i++)
    {
        fprintf(this->out_file, "%sv%u", sep, buffer[i]);
        sep = ", ";
    }
    
    fprintf(this->out_file, "}, meth@%u\n", index);
}

void BytecodeSegment::OnInvokeDirect(unsigned char size,
                                     unsigned char c,
                                      unsigned char d,
                                      unsigned char e,
                                      unsigned char f,
                                      unsigned char g,
                                      unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "invoke-direct {");
    unsigned char buffer[5] = {c, d, e, f, g};
    const char *sep = "";
    for(int i = 0;i < size;i++)
    {
        fprintf(this->out_file, "%sv%u", sep, buffer[i]);
        sep = ", ";
    }
    
    fprintf(this->out_file, "}, meth@%u\n", index);
}

void BytecodeSegment::OnInvokeStatic(unsigned char size,
                                     unsigned char c,
                                      unsigned char d,
                                      unsigned char e,
                                      unsigned char f,
                                      unsigned char g,
                                      unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "invoke-static {");
    unsigned char buffer[5] = {c, d, e, f, g};
    const char *sep = "";
    for(int i = 0;i < size;i++)
    {
        fprintf(this->out_file, "%sv%u", sep, buffer[i]);
        sep = ", ";
    }
    
    fprintf(this->out_file, "}, meth@%u\n", index);
}

void BytecodeSegment::OnInvokeInterface(unsigned char size,
                                        unsigned char c,
                                         unsigned char d,
                                         unsigned char e,
                                         unsigned char f,
                                         unsigned char g,
                                         unsigned short index)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "invoke-interface {");
    unsigned char buffer[5] = {c, d, e, f, g};
    const char *sep = "";
    for(int i = 0;i < size;i++)
    {
        fprintf(this->out_file, "%sv%u", sep, buffer[i]);
        sep = ", ";
    }
    
    fprintf(this->out_file, "}, meth@%u\n", index);
}

void BytecodeSegment::OnNegInt(unsigned char reg1,
                               unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "neg-int v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnNotInt(unsigned char reg1,
                               unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "not-int v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnNegLong(unsigned char reg1,
                                unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "neg-long v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnNotLong(unsigned char reg1,
                                unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "not-long v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnNegFloat(unsigned char reg1,
                                 unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "neg-float v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnNegDouble(unsigned char reg1,
                                  unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "neg-double v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnIntToLong(unsigned char reg1,
                                  unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "int-to-long v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnIntToFloat(unsigned char reg1,
                                   unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "int-to-float v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnIntToDouble(unsigned char reg1,
                                    unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "int-to-double v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnLongToInt(unsigned char reg1,
                                  unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "long-to-int v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnLongToFloat(unsigned char reg1,
                                    unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "long-to-float v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnLongToDouble(unsigned char reg1,
                                     unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "long-to-double v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnFloatToInt(unsigned char reg1,
                                   unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "float-to-int v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnFloatToLong(unsigned char reg1,
                                    unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "float-to-long v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnFloatToDouble(unsigned char reg1,
                                      unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "float-to-double v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnDoubleToInt(unsigned char reg1,
                                    unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "double-to-int v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnDoubleToLong(unsigned char reg1,
                                     unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "double-to-long v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnDoubleToFloat(unsigned char reg1,
                                      unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "double-to-float v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnIntToByte(unsigned char reg1,
                                  unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "int-to-byte v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnIntToChar(unsigned char reg1,
                                  unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "int-to-char v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnIntToShort(unsigned char reg1,
                                   unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "int-to-short v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnAddInt(unsigned char dest,
                               unsigned char reg1,
                               unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "add-int v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnSubInt(unsigned char dest,
                               unsigned char reg1,
                               unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "sub-int v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnMulInt(unsigned char dest,
                               unsigned char reg1,
                               unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "mul-int v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnDivInt(unsigned char dest,
                               unsigned char reg1,
                               unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "div-int v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnRemInt(unsigned char dest,
                               unsigned char reg1,
                               unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "rem-int v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnAndInt(unsigned char dest,
                               unsigned char reg1,
                               unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "and-int v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnOrInt(unsigned char dest,
                              unsigned char reg1,
                              unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "or-int v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnXorInt(unsigned char dest,
                               unsigned char reg1,
                               unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "xor-int v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnShlInt(unsigned char dest,
                               unsigned char reg1,
                               unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "shl-int v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnShrInt(unsigned char dest,
                               unsigned char reg1,
                               unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "shr-int v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnUshrInt(unsigned char dest,
                                unsigned char reg1,
                                unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "ushr-int v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnAddLong(unsigned char dest,
                                unsigned char reg1,
                                unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "add-long v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnSubLong(unsigned char dest,
                                unsigned char reg1,
                                unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "sub-long v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnMulLong(unsigned char dest,
                                unsigned char reg1,
                                unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "mul-long v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnDivLong(unsigned char dest,
                                unsigned char reg1,
                                unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "div-long v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnRemLong(unsigned char dest,
                                unsigned char reg1,
                                unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "rem-long v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnAndLong(unsigned char dest,
                                unsigned char reg1,
                                unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "and-long v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnOrLong(unsigned char dest,
                               unsigned char reg1,
                               unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "or-long v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnXorLong(unsigned char dest,
                                unsigned char reg1,
                                unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "xor-long v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnShlLong(unsigned char dest,
                                unsigned char reg1,
                                unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "shl-long v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnShrLong(unsigned char dest,
                                unsigned char reg1,
                                unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "shr-long v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnUshrLong(unsigned char dest,
                                 unsigned char reg1,
                                 unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "ushr-long v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnAddFloat(unsigned char dest,
                                 unsigned char reg1,
                                 unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "add-float v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnSubFloat(unsigned char dest,
                                 unsigned char reg1,
                                 unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "sub-float v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnMulFloat(unsigned char dest,
                                 unsigned char reg1,
                                 unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "mul-float v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnDivFloat(unsigned char dest,
                                 unsigned char reg1,
                                 unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "div-float v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnRemFloat(unsigned char dest,
                                 unsigned char reg1,
                                 unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "rem-float v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnAddDouble(unsigned char dest,
                                  unsigned char reg1,
                                  unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "add-double v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnSubDouble(unsigned char dest,
                                  unsigned char reg1,
                                  unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "sub-double v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnMulDouble(unsigned char dest,
                                  unsigned char reg1,
                                  unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "mul-double v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnDivDouble(unsigned char dest,
                                  unsigned char reg1,
                                  unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "div-double v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnRemDouble(unsigned char dest,
                                  unsigned char reg1,
                                  unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "rem-double v%u, v%u, v%u\n",
            dest, 
            reg1, 
            reg2); 
}

void BytecodeSegment::OnAddInt2addr(unsigned char reg1,
                                    unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "add-int/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnSubInt2addr(unsigned char reg1,
                                    unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "sub-int/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnMulInt2addr(unsigned char reg1,
                                    unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "mul-int/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnDivInt2addr(unsigned char reg1,
                                    unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "div-int/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnRemInt2addr(unsigned char reg1,
                                    unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "rem-int/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnAndInt2addr(unsigned char reg1,
                                    unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "and-int/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnOrInt2addr(unsigned char reg1,
                                   unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "or-int/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnXorInt2addr(unsigned char reg1,
                                    unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "xor-int/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnShlInt2addr(unsigned char reg1,
                                    unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "shl-int/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnShrInt2addr(unsigned char reg1,
                                    unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "shr-int/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnUshrInt2addr(unsigned char reg1,
                                     unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "ushr-int/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnAddLong2addr(unsigned char reg1,
                                     unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "add-long/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnSubLong2addr(unsigned char reg1,
                                     unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "sub-long/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnMulLong2addr(unsigned char reg1,
                                     unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "mul-long/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnDivLong2addr(unsigned char reg1,
                                     unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "div-long/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnRemLong2addr(unsigned char reg1,
                                     unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "rem-long/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnAndLong2addr(unsigned char reg1,
                                     unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "and-long/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnOrLong2addr(unsigned char reg1,
                                    unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "or-long/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnXorLong2addr(unsigned char reg1,
                                     unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "xor-long/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnShlLong2addr(unsigned char reg1,
                                     unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "shl-long/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnShrLong2addr(unsigned char reg1,
                                     unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "shr-long/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnUshrLong2addr(unsigned char reg1,
                                      unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "ushr-long/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnAddFloat2addr(unsigned char reg1,
                                      unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "add-float/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnSubFloat2addr(unsigned char reg1,
                                      unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "sub-float/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnMulFloat2addr(unsigned char reg1,
                                      unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "mul-float/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnDivFloat2addr(unsigned char reg1,
                                      unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "div-float/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnRemFloat2addr(unsigned char reg1,
                                      unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "rem-float/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnAddDouble2addr(unsigned char reg1,
                                       unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "add-double/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnSubDouble2addr(unsigned char reg1,
                                       unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "sub-double/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnMulDouble2addr(unsigned char reg1,
                                       unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "mul-double/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnDivDouble2addr(unsigned char reg1,
                                       unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "div-double/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnRemDouble2addr(unsigned char reg1,
                                       unsigned char reg2)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "rem-double/2addr v%u, v%u\n",
            reg1, 
            reg2); 
}

void BytecodeSegment::OnAddIntlit16(unsigned char reg1,
                                    unsigned char reg2,
                                    int value)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "add-int/lit16 v%u, v%u, %d\n",
            reg1, 
            reg2, 
            value); 
}

void BytecodeSegment::OnRsubInt(unsigned char reg1,
                                unsigned char reg2,
                                int value)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "rsub-int v%u, v%u, %d\n",
            reg1, 
            reg2, 
            value); 
}

void BytecodeSegment::OnMulIntlit16(unsigned char reg1,
                                    unsigned char reg2,
                                    int value)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "mul-int/lit16 v%u, v%u, %d\n",
            reg1, 
            reg2, 
            value); 
}

void BytecodeSegment::OnDivIntlit16(unsigned char reg1,
                                    unsigned char reg2,
                                    int value)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "div-int/lit16 v%u, v%u, %d\n",
            reg1, 
            reg2, 
            value); 
}

void BytecodeSegment::OnRemIntlit16(unsigned char reg1,
                                    unsigned char reg2,
                                    int value)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "rem-int/lit16 v%u, v%u, %d\n",
            reg1, 
            reg2, 
            value); 
}

void BytecodeSegment::OnAndIntlit16(unsigned char reg1,
                                    unsigned char reg2,
                                    int value)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "and-int/lit16 v%u, v%u, %d\n",
            reg1, 
            reg2, 
            value); 
}

void BytecodeSegment::OnOrIntlit16(unsigned char reg1,
                                   unsigned char reg2,
                                   int value)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "or-int/lit16 v%u, v%u, %d\n",
            reg1, 
            reg2, 
            value); 
}

void BytecodeSegment::OnXorIntlit16(unsigned char reg1,
                                    unsigned char reg2,
                                    int value)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "xor-int/lit16 v%u, v%u, %d\n",
            reg1, 
            reg2, 
            value); 
}

void BytecodeSegment::OnAddIntlit8(unsigned char dest,
                                   unsigned char src,
                                   int value)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "add-int/lit8 v%u, v%u, %d\n",
            dest, 
            src, 
            value); 
}

void BytecodeSegment::OnRsubIntlit8(unsigned char dest,
                                    unsigned char src,
                                    int value)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "rsub-int/lit8 v%u, v%u, %d\n",
            dest, 
            src, 
            value); 
}

void BytecodeSegment::OnMulIntlit8(unsigned char dest,
                                   unsigned char src,
                                   int value)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "mul-int/lit8 v%u, v%u, %d\n",
            dest, 
            src, 
            value); 
}

void BytecodeSegment::OnDivIntlit8(unsigned char dest,
                                   unsigned char src,
                                   int value)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "div-int/lit8 v%u, v%u, %d\n",
            dest, 
            src, 
            value); 
}

void BytecodeSegment::OnRemIntlit8(unsigned char dest,
                                   unsigned char src,
                                   int value)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "rem-int/lit8 v%u, v%u, %d\n",
            dest, 
            src, 
            value); 
}

void BytecodeSegment::OnAndIntlit8(unsigned char dest,
                                   unsigned char src,
                                   int value)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "and-int/lit8 v%u, v%u, %d\n",
            dest, 
            src, 
            value); 
}

void BytecodeSegment::OnOrIntlit8(unsigned char dest,
                                  unsigned char src,
                                  int value)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "or-int/lit8 v%u, v%u, %d\n",
            dest, 
            src, 
            value); 
}

void BytecodeSegment::OnXorIntlit8(unsigned char dest,
                                   unsigned char src,
                                   int value)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "xor-int/lit8 v%u, v%u, %d\n",
            dest, 
            src, 
            value); 
}

void BytecodeSegment::OnShlIntlit8(unsigned char dest,
                                   unsigned char src,
                                   int value)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "shl-int/lit8 v%u, v%u, %d\n",
            dest, 
            src, 
            value); 
}

void BytecodeSegment::OnShrIntlit8(unsigned char dest,
                                   unsigned char src,
                                   int value)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "shr-int/lit8 v%u, v%u, %d\n",
            dest, 
            src, 
            value); 
}

void BytecodeSegment::OnUshrIntlit8(unsigned char dest,
                                    unsigned char src,
                                    int value)
{
    this->PrintLineNum();
    fprintf(this->out_file,
            "ushr-int/lit8 v%u, v%u, %d\n",
            dest, 
            src, 
            value); 
}

////////////////////////////////////////////////////////////////
// Call back function block end ////////////////////////////////
////////////////////////////////////////////////////////////////

/*
 * Dispatch() - Bytecode decoder kernel
 *
 * Reads opcode, parse its operands, and call corresponding callbacks
 * If you want to change the default behaviour which is just printing
 * disassembly code on output file, then you should override all OnXXX()
 * methods, since this function will call them with arguments when an
 * opcode is found
 */
void BytecodeSegment::Dispatch()
{
    this->OnStart();
    
    // When == is satisfied, ptr points to position after the last valid byte
    while(!this->IsEof())
    {
        unsigned char byte1 = this->GetNextByte();
        unsigned char byte2, byte3, byte4;
        // Used by filled arrays
        unsigned char c, d, e, f, g;
        unsigned short word1, word2;
        unsigned int uint1, uint2;
        int int1;
        long long lld1;
        unsigned long long llu1;
        
        this->OnNextInstruction(byte1);
        //this->PrintLineNum();
        //fprintf(out_file, "opcode = %.2X\n", byte1);
        switch(byte1)
        {
            case 0x00:
                byte2 = this->GetNextByte();
                if(byte2 == 0x00) 
                {
                    this->OnNop(byte2);
                }
                else if(byte2 == 0x01)
                {
                    word1 = this->GetNextShort();
                    this->OnPackedSwitchPayload(word1);
                }
                else if(byte2 == 0x02)
                {
                    word1 = this->GetNextShort();
                    this->OnSparseSwitchPayload(word1);
                }
                else if(byte2 == 0x03)
                {
                    word1 = this->GetNextShort();
                    uint1 = this->GetNextInt();
                    this->OnFillArrayDataPayload(word1, uint1);
                }
                else
                {
                    fprintf(this->out_file, "MSB = %.2X, error!\n", byte2);
                    this->Assert(false, __LINE__);    
                }
                
                break;
            case 0x01:
                byte2 = this->GetNextByte();
                // Dest is lower 4 bits, src is higher 4 bits
                this->OnMove(byte2 | 0x0F, byte2 >> 4);
                break;
            case 0x02:
                byte2 = this->GetNextByte();
                word1 = this->GetNextShort();
                // Dest is 8 bits, src is 16 bits
                this->OnMoveFrom16(byte2, word1);
                break;
            case 0x03:
                byte2 = this->GetNextByte();
                word1 = this->GetNextShort();
                word2 = this->GetNextShort();
                // First word is dest
                this->OnMove16(word1, word2);
                break;
            case 0x04:
                byte2 = this->GetNextByte();
                this->OnMoveWide(byte2 | 0x0F, byte2 >> 4);
                break;
            case 0x05:
                byte2 = this->GetNextByte();
                word1 = this->GetNextShort();
                // Dest is 8 bits, src is 16 bits
                this->OnMoveWideFrom16(byte2, word1);
                break;
            case 0x06:
                byte2 = this->GetNextByte();
                word1 = this->GetNextShort();
                word2 = this->GetNextShort();
                // First word is dest
                this->OnMoveWide16(word1, word2);
                break;
            case 0x07:
                byte2 = this->GetNextByte();
                // Dest is lower 4 bits, src is higher 4 bits
                this->OnMoveObject(byte2 | 0x0F, byte2 >> 4);
                break;
            case 0x08:
                byte2 = this->GetNextByte();
                word1 = this->GetNextShort();
                // Dest is 8 bits, src is 16 bits
                this->OnMoveObjectFrom16(byte2, word1);
                break;
            case 0x09:
                byte2 = this->GetNextByte();
                word1 = this->GetNextShort();
                word2 = this->GetNextShort();
                // First word is dest
                this->OnMoveObject16(word1, word2);
                break;
            case 0x0A:
                byte2 = this->GetNextByte();
                this->OnMoveResult(byte2);
                break;
            case 0x0B:
                byte2 = this->GetNextByte();
                this->OnMoveResultWide(byte2);
                break;
            case 0x0C:
                byte2 = this->GetNextByte();
                this->OnMoveResultObject(byte2);
                break;
            case 0x0D:
                byte2 = this->GetNextByte();
                this->OnMoveException(byte2);
                break;
            case 0x0E:
                byte2 = this->GetNextByte();
                this->OnReturnVoid(byte2);
                break;
            case 0x0F:
                byte2 = this->GetNextByte();
                this->OnReturn(byte2);
                break;
            case 0x10:
                byte2 = this->GetNextByte();
                this->OnReturnWide(byte2);
                break;
            case 0x11:
                byte2 = this->GetNextByte();
                this->OnReturnObject(byte2);
                break;
            case 0x12:
                byte2 = this->GetNextByte();
                // Shift to front as uint and then shift back as int
                // This way we make use of arithmetic shift for
                // signed integer defined by C/C++
                int1 = ((int)((((unsigned int)byte2) >> 4) << 28)) >> 28;
                byte2 &= 0x0F;
                this->OnConst4(byte2, int1);
                break;
            case 0x13:
                byte2 = this->GetNextByte();
                word2 = this->GetNextShort();
                int1 = ((int)(((unsigned int)word2) << 16)) >> 16;
                this->OnConst16(byte2, int1);
                break;
            case 0x14:
                byte2 = this->GetNextByte();
                uint1 = this->GetNextInt();
                this->OnConst(byte2, uint1);
                break;
            case 0x15:
                byte2 = this->GetNextByte();
                word1 = this->GetNextShort();
                uint1 = (unsigned int)word1 << 16;
                this->OnConstHigh16(byte2, uint1);
                break;
            case 0x16:
                byte2 = this->GetNextByte();
                word1 = this->GetNextShort();
                
                lld1 = \
                    (
                        (long long) \
                            (
                                (
                                    (unsigned long long) \
                                        word1 \
                                ) \
                                     << 48
                            )
                    ) >> 48;
                this->OnConstWide16(byte2, lld1);
                break;
            case 0x17:
                byte2 = this->GetNextByte();
                uint1 = this->GetNextInt();
                
                lld1 = ((long long)(((unsigned long long)uint1) << 32)) >> 32;
                this->OnConstWide32(byte2, lld1);
                break;
            case 0x18:
                byte2 = this->GetNextByte();
                uint1 = this->GetNextInt();
                uint2 = this->GetNextInt();
                llu1 = ((unsigned long long)uint2 << 32) | \
                      ((unsigned long long)uint1);
                this->OnConstWide(byte2, llu1);
                break;
            case 0x19:
                byte2 = this->GetNextByte();
                word1 = this->GetNextShort();
                llu1 = (unsigned long long)word1 << 48;
                
                this->OnConstWideHigh16(byte2, llu1);
                break;
            case 0x1A:
                byte2 = this->GetNextByte();
                word1 = this->GetNextShort();
                
                this->OnConstString(byte2, word1);
                break;
            case 0x1B:
                byte2 = this->GetNextByte();
                uint1 = this->GetNextInt();
                this->OnConstStringJumbo(byte2, uint1);
                break;
            case 0x1C:
                byte2 = this->GetNextByte();
                word1 = this->GetNextShort();
                this->OnConstClass(byte2, word1);
                break;
            case 0x1D:
                byte2 = this->GetNextByte();
                this->OnMonitorEnter(byte2);
                break;
            case 0x1E:
                byte2 = this->GetNextByte();
                this->OnMonitorExit(byte2);
                break;
            case 0x1F:
                byte2 = this->GetNextByte();
                word1 = this->GetNextShort();
                this->OnCheckCast(byte2, word1);
                break;
            case 0x20:
                byte2 = this->GetNextByte();
                word1 = this->GetNextShort();
                this->OnInstanceOf(byte2 | 0x0F, byte2 >> 4, word1);
                break;
            case 0x21:
                byte2 = this->GetNextByte();
                this->OnArrayLength(byte2 | 0x0F, byte2 > 4);
                break;
            case 0x22:
                byte2 = this->GetNextByte();
                word1 = this->GetNextShort();
                this->OnNewInstance(byte2, word1);
                break;
            case 0x23:
                byte2 = this->GetNextByte();
                word1 = this->GetNextShort();
                this->OnNewArray(byte2 & 0x0F, byte2 >> 4, word1);
                break;
            case 0x24:
                // This is used as size
                byte2 = this->GetNextByte();
                g = byte2 & 0x0F;
                byte2 >>= 4;
                // This is the type
                word1 = this->GetNextShort();
                
                c = this->GetNextByte();
                d = c >> 4;
                c &= 0x0F;
                
                e = this->GetNextByte();
                f = e >> 4;
                e &= 0x0F;
                
                this->OnFilledNewArray(byte2, c, d, e, f, g, word1);
                
                break;
            case 0x25:
                // size
                byte2 = this->GetNextByte();
                // Type
                word1 = this->GetNextShort();
                // Start register
                word2 = this->GetNextShort();
                
                this->OnFilledNewArrayRange(byte2, word1, word2);
                break;
            case 0x26:
                byte2 = this->GetNextByte();
                int1 = (int)this->GetNextInt();
                this->OnFillArrayData(byte2, int1);
                break;
            case 0x27:
                byte2 = this->GetNextByte();
                this->OnThrow(byte2);
                break;
            case 0x28:
                byte2 = this->GetNextByte();
                this->OnGoto(byte2);
                break;
            case 0x29:
                byte2 = this->GetNextByte();
                word1 = this->GetNextShort();
                this->OnGoto16((short)word1, byte2);
                break;
            case 0x2A:
                byte2 = this->GetNextByte();
                int1 = (int)this->GetNextInt();
                this->OnGoto32(int1, byte2);
                break;
            case 0x2B:
                byte2 = this->GetNextByte();
                int1 = (int)this->GetNextInt();
                this->OnPackedSwitch(byte2, int1);
                break;
            case 0x2C:
                byte2 = this->GetNextByte();
                int1 = (int)this->GetNextInt();
                this->OnSparseSwitch(byte2, int1);
                break;
            case 0x2D:
            case 0x2E:
            case 0x2F:
            case 0x30:
            case 0x31:
                byte2 = this->GetNextByte();
                byte3 = this->GetNextByte();
                byte4 = this->GetNextByte();
                switch(byte1)
                {
                    case 0x2D:
                        this->OnCmplFloat(byte2, byte3, byte4);
                        break;
                    case 0x2E:
                        this->OnCmpgFloat(byte2, byte3, byte4);
                        break;
                    case 0x2F:
                        this->OnCmplDouble(byte2, byte3, byte4);
                        break;
                    case 0x30:
                        this->OnCmpgDouble(byte2, byte3, byte4);
                        break;
                    case 0x31:
                        this->OnCmpLong(byte2, byte3, byte4);
                        break;
                    default:
                        this->Assert(false, __LINE__);
                        break;
                }
                
                break;
            case 0x32:
            case 0x33:
            case 0x34:
            case 0x35:
            case 0x36:
            case 0x37:
                byte2 = this->GetNextByte();
                word1 = this->GetNextShort();
                // second reg
                byte3 = byte2 >> 4;
                // first reg
                byte2 &= 0x0F;
                // Signed extended offset
                int1 = ((int)(((unsigned int)word1) << 16)) >> 16;
                
                switch(byte1)
                {
                    case 0x32:
                        this->OnIfEq(byte2, byte3, int1);
                        break;
                    case 0x33:
                        this->OnIfNe(byte2, byte3, int1);
                        break;
                    case 0x34:
                        this->OnIfLt(byte2, byte3, int1);
                        break;
                    case 0x35:
                        this->OnIfGe(byte2, byte3, int1);
                        break;
                    case 0x36:
                        this->OnIfGt(byte2, byte3, int1);
                        break;
                    case 0x37:
                        this->OnIfLe(byte2, byte3, int1);
                        break;
                    default:
                        this->Assert(false, __LINE__);
                        break;
                }
                
                break;
            
            case 0x38:
            case 0x39:
            case 0x3A:
            case 0x3B:
            case 0x3C:
            case 0x3D:
                byte2 = this->GetNextByte();
                word1 = this->GetNextShort();
                int1 = ((int)(((unsigned int)word1) << 16)) >> 16;
                
                switch(byte1)
                {
                    case 0x38:
                        this->OnIfEqZ(byte2, int1);
                        break;    
                    case 0x39:
                        this->OnIfNeZ(byte2, int1);
                        break;
                    case 0x3A:
                        this->OnIfLtZ(byte2, int1);
                        break;
                    case 0x3B:
                        this->OnIfGeZ(byte2, int1);
                        break;
                    case 0x3C:
                        this->OnIfGtZ(byte2, int1);
                        break;
                    case 0x3D:
                        this->OnIfLeZ(byte2, int1);
                        break;
                    default:
                        this->Assert(false, __LINE__);
                        break;
                }
                
                break;
            case 0x3E:
            case 0x3F:
            case 0x40:
            case 0x41:
            case 0x42:
            case 0x43:
                byte2 = this->GetNextByte();
                this->OnUnused(byte2);
                break;
            
            case 0x44:
            case 0x45:
            case 0x46:
            case 0x47:
            case 0x48:
            case 0x49:
            case 0x4A:
            case 0x4B:
            case 0x4C:
            case 0x4D:
            case 0x4E:
            case 0x4F:
            case 0x50:
            case 0x51:
                byte2 = this->GetNextByte();
                byte3 = this->GetNextByte();
                byte4 = this->GetNextByte();
                switch(byte1)
                {
                    case 0x44:
                        this->OnAGet(byte2, byte3, byte4);
                        break;    
                    case 0x45:
                        this->OnAGetWide(byte2, byte3, byte4);
                        break;    
                    case 0x46:
                        this->OnAGetObject(byte2, byte3, byte4);
                        break;    
                    case 0x47:
                        this->OnAGetBoolean(byte2, byte3, byte4);
                        break;    
                    case 0x48:
                        this->OnAGetByte(byte2, byte3, byte4);
                        break;    
                    case 0x49:
                        this->OnAGetChar(byte2, byte3, byte4);
                        break;    
                    case 0x4A:
                        this->OnAGetShort(byte2, byte3, byte4);
                        break;    
                    case 0x4B:
                        this->OnAPut(byte2, byte3, byte4);
                        break;    
                    case 0x4C:
                        this->OnAPutWide(byte2, byte3, byte4);
                        break;    
                    case 0x4D:
                        this->OnAPutObject(byte2, byte3, byte4);
                        break;    
                    case 0x4E:
                        this->OnAPutBoolean(byte2, byte3, byte4);
                        break;    
                    case 0x4F:
                        this->OnAPutByte(byte2, byte3, byte4);
                        break;    
                    case 0x50:
                        this->OnAPutChar(byte2, byte3, byte4);
                        break;    
                    case 0x51:
                        this->OnAPutShort(byte2, byte3, byte4);
                        break;
                    default:
                        this->Assert(false, __LINE__);
                        break;
                }
                
                break;
                
            case 0x52:
            case 0x53:
            case 0x54:
            case 0x55:
            case 0x56:
            case 0x57:
            case 0x58:
            case 0x59:
            case 0x5A:
            case 0x5B:
            case 0x5C:
            case 0x5D:
            case 0x5E:
            case 0x5F:
                byte2 = this->GetNextByte();
                byte3 = byte2 >> 4;
                byte2 &= 0x0F;
                word1 = this->GetNextShort();
                
                switch(byte1)
                {
                    case 0x52:
                        this->OnIGet(byte2, byte3, word1);
                        break;    
                    case 0x53:
                        this->OnIGetWide(byte2, byte3, word1);
                        break;    
                    case 0x54:
                        this->OnIGetObject(byte2, byte3, word1);
                        break;    
                    case 0x55:
                        this->OnIGetBoolean(byte2, byte3, word1);
                        break;    
                    case 0x56:
                        this->OnIGetByte(byte2, byte3, word1);
                        break;    
                    case 0x57:
                        this->OnIGetChar(byte2, byte3, word1);
                        break;    
                    case 0x58:
                        this->OnIGetShort(byte2, byte3, word1);
                        break;    
                    case 0x59:
                        this->OnIPut(byte2, byte3, word1);
                        break;    
                    case 0x5A:
                        this->OnIPutWide(byte2, byte3, word1);
                        break;    
                    case 0x5B:
                        this->OnIPutObject(byte2, byte3, word1);
                        break;    
                    case 0x5C:
                        this->OnIPutBoolean(byte2, byte3, word1);
                        break;    
                    case 0x5D:
                        this->OnIPutByte(byte2, byte3, word1);
                        break;    
                    case 0x5E:
                        this->OnIPutChar(byte2, byte3, word1);
                        break;    
                    case 0x5F:
                        this->OnIPutShort(byte2, byte3, word1);
                        break;    
                }
                
                break;
            case 0x60:
            case 0x61:
            case 0x62:
            case 0x63:
            case 0x64:
            case 0x65:
            case 0x66:
            case 0x67:
            case 0x68:
            case 0x69:
            case 0x6A:
            case 0x6B:
            case 0x6C:
            case 0x6D:
                byte2 = this->GetNextByte();
                word1 = this->GetNextShort();
            
                switch(byte1)
                {
                    case 0x60:
                        this->OnSGet(byte2, word1);
                        break;    
                    case 0x61:
                        this->OnSGetWide(byte2, word1);
                        break;    
                    case 0x62:
                        this->OnSGetObject(byte2, word1);
                        break;    
                    case 0x63:
                        this->OnSGetBoolean(byte2, word1);
                        break;    
                    case 0x64:
                        this->OnSGetByte(byte2, word1);
                        break;    
                    case 0x65:
                        this->OnSGetChar(byte2, word1);
                        break;    
                    case 0x66:
                        this->OnSGetShort(byte2, word1);
                        break;    
                    case 0x67:
                        this->OnSPut(byte2, word1);
                        break;    
                    case 0x68:
                        this->OnSPutWide(byte2, word1);
                        break;    
                    case 0x69:
                        this->OnSPutObject(byte2, word1);
                        break;    
                    case 0x6A:
                        this->OnSPutBoolean(byte2, word1);
                        break;    
                    case 0x6B:
                        this->OnSPutByte(byte2, word1);
                        break;    
                    case 0x6C:
                        this->OnSPutChar(byte2, word1);
                        break;    
                    case 0x6D:
                        this->OnSPutShort(byte2, word1);
                        break;
                    default:
                        this->Assert(false, __LINE__);
                        break;
                }
                
                break;
            case 0x6E:
            case 0x6F:
            case 0x70:
            case 0x71:
            case 0x72:
                byte2 = this->GetNextByte();
                g = byte2 & 0x0F;
                // Number of registers
                byte2 >>= 4;
                // Method index
                word1 = this->GetNextShort();
                c = this->GetNextByte();
                d = c >> 4;
                c &= 0x0F;
                e = this->GetNextByte();
                f = e >> 4;
                e &= 0x0F;
                
                switch(byte1)
                {
                    case 0x6E:
                        this->OnInvokeVirtual(byte2, c, d, e, f, g, word1);
                        break;
                    case 0x6F:
                        this->OnInvokeSuper(byte2, c, d, e, f, g, word1);
                        break;    
                    case 0x70:
                        this->OnInvokeDirect(byte2, c, d, e, f, g, word1);
                        break;    
                    case 0x71:
                        this->OnInvokeStatic(byte2, c, d, e, f, g, word1);
                        break;    
                    case 0x72:
                        this->OnInvokeInterface(byte2, c, d, e, f, g, word1);
                        break;    
                    default:
                        this->Assert(false, __LINE__);
                        break;
                }
                
                break;
            case 0x73:
                byte2 = this->GetNextByte();
                this->OnUnused(byte2);
                break;
            case 0x74:
            case 0x75:
            case 0x76:
            case 0x77:
            case 0x78:
                byte2 = this->GetNextByte();
                word1 = this->GetNextShort();
                word2 = this->GetNextShort();
                
                switch(byte1)
                {
                    case 0x74:
                        this->OnInvokeVirtualRange(word1, word2, byte2);
                        break;
                    case 0x75:
                        this->OnInvokeSuperRange(word1, word2, byte2);
                        break;
                    case 0x76:    
                        this->OnInvokeDirectRange(word1, word2, byte2);
                        break;
                    case 0x77:
                        this->OnInvokeStaticRange(word1, word2, byte2);
                        break;
                    case 0x78:
                        this->OnInvokeInterfaceRange(word1, word2, byte2);
                        break;
                    default:
                        this->Assert(false, __LINE__);
                        break;
                }
                
                break;
            case 0x79:
            case 0x7A:
                byte2 = this->GetNextByte();
                this->OnUnused(byte2);
                break;
            
            case 0x7B:
            case 0x7C:
            case 0x7D:
            case 0x7E:
            case 0x7F:
            case 0x80:
            case 0x81:
            case 0x82:
            case 0x83:
            case 0x84:
            case 0x85:
            case 0x86:
            case 0x87:
            case 0x88:
            case 0x89:
            case 0x8A:
            case 0x8B:
            case 0x8C:
            case 0x8D:
            case 0x8E:
            case 0x8F:
                byte2 = this->GetNextByte();
                byte3 = byte2 >> 4;
                byte2 &= 0x0F;
                
                switch(byte1)
                {
                    case 0x7B:
                        this->OnNegInt(byte2, byte3);
                        break;
                    case 0x7C:
                        this->OnNotInt(byte2, byte3);
                        break;
                    case 0x7D:
                        this->OnNegLong(byte2, byte3);
                        break;
                    case 0x7E:
                        this->OnNotLong(byte2, byte3);
                        break;
                    case 0x7F:
                        this->OnNegFloat(byte2, byte3);
                        break;
                    case 0x80:
                        this->OnNegDouble(byte2, byte3);
                        break;
                    case 0x81:
                        this->OnIntToLong(byte2, byte3);
                        break;
                    case 0x82:
                        this->OnIntToFloat(byte2, byte3);
                        break;
                    case 0x83:
                        this->OnIntToDouble(byte2, byte3);
                        break;
                    case 0x84:
                        this->OnLongToInt(byte2, byte3);
                        break;
                    case 0x85:
                        this->OnLongToFloat(byte2, byte3);
                        break;
                    case 0x86:
                        this->OnLongToDouble(byte2, byte3);
                        break;
                    case 0x87:
                        this->OnFloatToInt(byte2, byte3);
                        break;
                    case 0x88:
                        this->OnFloatToLong(byte2, byte3);
                        break;
                    case 0x89:
                        this->OnFloatToDouble(byte2, byte3);
                        break;
                    case 0x8A:
                        this->OnDoubleToInt(byte2, byte3);
                        break;
                    case 0x8B:
                        this->OnDoubleToLong(byte2, byte3);
                        break;
                    case 0x8C:
                        this->OnDoubleToFloat(byte2, byte3);
                        break;
                    case 0x8D:
                        this->OnIntToByte(byte2, byte3);
                        break;
                    case 0x8E:
                        this->OnIntToChar(byte2, byte3);
                        break;
                    case 0x8F:
                        this->OnIntToShort(byte2, byte3);
                        break;
                    default:
                        this->Assert(false, __LINE__);
                        break;
                }
                
                break;
            
            case 0x90:
            case 0x91:
            case 0x92:
            case 0x93:
            case 0x94:
            case 0x95:
            case 0x96:
            case 0x97:
            case 0x98:
            case 0x99:
            case 0x9A:
            case 0x9B:
            case 0x9C:
            case 0x9D:
            case 0x9E:
            case 0x9F:
            case 0xA0:
            case 0xA1:
            case 0xA2:
            case 0xA3:
            case 0xA4:
            case 0xA5:
            case 0xA6:
            case 0xA7:
            case 0xA8:
            case 0xA9:
            case 0xAA:
            case 0xAB:
            case 0xAC:
            case 0xAD:
            case 0xAE:
            case 0xAF:
                byte2 = this->GetNextByte();
                byte3 = this->GetNextByte();
                byte4 = this->GetNextByte();
                
                switch(byte1)
                {
                    case 0x90:
                        this->OnAddInt(byte2, byte3, byte4);
                        break;
                    case 0x91:
                        this->OnSubInt(byte2, byte3, byte4);
                        break;
                    case 0x92:
                        this->OnMulInt(byte2, byte3, byte4);
                        break;
                    case 0x93:
                        this->OnDivInt(byte2, byte3, byte4);
                        break;
                    case 0x94:
                        this->OnRemInt(byte2, byte3, byte4);
                        break;
                    case 0x95:
                        this->OnAndInt(byte2, byte3, byte4);
                        break;
                    case 0x96:
                        this->OnOrInt(byte2, byte3, byte4);
                        break;
                    case 0x97:
                        this->OnXorInt(byte2, byte3, byte4);
                        break;
                    case 0x98:
                        this->OnShlInt(byte2, byte3, byte4);
                        break;
                    case 0x99:
                        this->OnShrInt(byte2, byte3, byte4);
                        break;
                    case 0x9A:
                        this->OnUshrInt(byte2, byte3, byte4);
                        break;
                    case 0x9B:
                        this->OnAddLong(byte2, byte3, byte4);
                        break;
                    case 0x9C:
                        this->OnSubLong(byte2, byte3, byte4);
                        break;
                    case 0x9D:
                        this->OnMulLong(byte2, byte3, byte4);
                        break;
                    case 0x9E:
                        this->OnDivLong(byte2, byte3, byte4);
                        break;
                    case 0x9F:
                        this->OnRemLong(byte2, byte3, byte4);
                        break;
                    case 0xA0:
                        this->OnAndLong(byte2, byte3, byte4);
                        break;
                    case 0xA1:
                        this->OnOrLong(byte2, byte3, byte4);
                        break;
                    case 0xA2:
                        this->OnXorLong(byte2, byte3, byte4);
                        break;
                    case 0xA3:
                        this->OnShlLong(byte2, byte3, byte4);
                        break;
                    case 0xA4:
                        this->OnShrLong(byte2, byte3, byte4);
                        break;
                    case 0xA5:
                        this->OnUshrLong(byte2, byte3, byte4);
                        break;
                    case 0xA6:
                        this->OnAddFloat(byte2, byte3, byte4);
                        break;
                    case 0xA7:
                        this->OnSubFloat(byte2, byte3, byte4);
                        break;
                    case 0xA8:
                        this->OnMulFloat(byte2, byte3, byte4);
                        break;
                    case 0xA9:
                        this->OnDivFloat(byte2, byte3, byte4);
                        break;
                    case 0xAA:
                        this->OnRemFloat(byte2, byte3, byte4);
                        break;
                    case 0xAB:
                        this->OnAddDouble(byte2, byte3, byte4);
                        break;
                    case 0xAC:
                        this->OnSubDouble(byte2, byte3, byte4);
                        break;
                    case 0xAD:
                        this->OnMulDouble(byte2, byte3, byte4);
                        break;
                    case 0xAE:
                        this->OnDivDouble(byte2, byte3, byte4);
                        break;
                    case 0xAF:
                        this->OnRemDouble(byte2, byte3, byte4);
                        break;
                    default:
                        this->Assert(false, __LINE__);
                        break;
                }
                
                break;
            case 0xB0:
            case 0xB1:
            case 0xB2:
            case 0xB3:
            case 0xB4:
            case 0xB5:
            case 0xB6:
            case 0xB7:
            case 0xB8:
            case 0xB9:
            case 0xBA:
            case 0xBB:
            case 0xBC:
            case 0xBD:
            case 0xBE:
            case 0xBF:
            case 0xC0:
            case 0xC1:
            case 0xC2:
            case 0xC3:
            case 0xC4:
            case 0xC5:
            case 0xC6:
            case 0xC7:
            case 0xC8:
            case 0xC9:
            case 0xCA:
            case 0xCB:
            case 0xCC:
            case 0xCD:
            case 0xCE:
            case 0xCF:
                byte2 = this->GetNextByte();
                byte3 = byte2 >> 4;
                byte2 &= 0x0F;
                
                switch(byte1)
                {
                    case 0xB0:
                        this->OnAddInt2addr(byte2, byte3);
                        break;
                    case 0xB1:
                        this->OnSubInt2addr(byte2, byte3);
                        break;
                    case 0xB2:
                        this->OnMulInt2addr(byte2, byte3);
                        break;
                    case 0xB3:
                        this->OnDivInt2addr(byte2, byte3);
                        break;
                    case 0xB4:
                        this->OnRemInt2addr(byte2, byte3);
                        break;
                    case 0xB5:
                        this->OnAndInt2addr(byte2, byte3);
                        break;
                    case 0xB6:
                        this->OnOrInt2addr(byte2, byte3);
                        break;
                    case 0xB7:
                        this->OnXorInt2addr(byte2, byte3);
                        break;
                    case 0xB8:
                        this->OnShlInt2addr(byte2, byte3);
                        break;
                    case 0xB9:
                        this->OnShrInt2addr(byte2, byte3);
                        break;
                    case 0xBA:
                        this->OnUshrInt2addr(byte2, byte3);
                        break;
                    case 0xBB:
                        this->OnAddLong2addr(byte2, byte3);
                        break;
                    case 0xBC:
                        this->OnSubLong2addr(byte2, byte3);
                        break;
                    case 0xBD:
                        this->OnMulLong2addr(byte2, byte3);
                        break;
                    case 0xBE:
                        this->OnDivLong2addr(byte2, byte3);
                        break;
                    case 0xBF:
                        this->OnRemLong2addr(byte2, byte3);
                        break;
                    case 0xC0:
                        this->OnAndLong2addr(byte2, byte3);
                        break;
                    case 0xC1:
                        this->OnOrLong2addr(byte2, byte3);
                        break;
                    case 0xC2:
                        this->OnXorLong2addr(byte2, byte3);
                        break;
                    case 0xC3:
                        this->OnShlLong2addr(byte2, byte3);
                        break;
                    case 0xC4:
                        this->OnShrLong2addr(byte2, byte3);
                        break;
                    case 0xC5:
                        this->OnUshrLong2addr(byte2, byte3);
                        break;
                    case 0xC6:
                        this->OnAddFloat2addr(byte2, byte3);
                        break;
                    case 0xC7:
                        this->OnSubFloat2addr(byte2, byte3);
                        break;
                    case 0xC8:
                        this->OnMulFloat2addr(byte2, byte3);
                        break;
                    case 0xC9:
                        this->OnDivFloat2addr(byte2, byte3);
                        break;
                    case 0xCA:
                        this->OnRemFloat2addr(byte2, byte3);
                        break;
                    case 0xCB:
                        this->OnAddDouble2addr(byte2, byte3);
                        break;
                    case 0xCC:
                        this->OnSubDouble2addr(byte2, byte3);
                        break;
                    case 0xCD:
                        this->OnMulDouble2addr(byte2, byte3);
                        break;
                    case 0xCE:
                        this->OnDivDouble2addr(byte2, byte3);
                        break;
                    case 0xCF:
                        this->OnRemDouble2addr(byte2, byte3);
                        break;
                    default:
                        this->Assert(false, __LINE__);
                        break;
                }
                
                break;
            case 0xD0:
            case 0xD1:
            case 0xD2:
            case 0xD3:
            case 0xD4:
            case 0xD5:
            case 0xD6:
            case 0xD7:
                byte2 = this->GetNextByte();
                byte3 = byte2 >> 4;
                byte2 &= 0x0F;
                word1 = this->GetNextShort();
                int1 = ((int)(((unsigned int)word1) << 16)) >> 16;
                
                switch(byte1)
                {
                    case 0xD0:
                        this->OnAddIntlit16(byte2, byte3, int1);
                        break;
                    case 0xD1:
                        this->OnRsubInt(byte2, byte3, int1);
                        break;
                    case 0xD2:
                        this->OnMulIntlit16(byte2, byte3, int1);
                        break;
                    case 0xD3:
                        this->OnDivIntlit16(byte2, byte3, int1);
                        break;
                    case 0xD4:
                        this->OnRemIntlit16(byte2, byte3, int1);
                        break;
                    case 0xD5:
                        this->OnAndIntlit16(byte2, byte3, int1);
                        break;
                    case 0xD6:
                        this->OnOrIntlit16(byte2, byte3, int1);
                        break;
                    case 0xD7:
                        this->OnXorIntlit16(byte2, byte3, int1);
                        break;
                    default:
                        this->Assert(false, __LINE__);
                        break;
                }
                
                break;
            case 0xD8:
            case 0xD9:
            case 0xDA:
            case 0xDB:
            case 0xDC:
            case 0xDD:
            case 0xDE:
            case 0xDF:
            case 0xE0:
            case 0xE1:
            case 0xE2:
                byte2 = this->GetNextByte();
                byte3 = this->GetNextByte();
                byte4 = this->GetNextByte();
                int1 = ((int)(((unsigned int)byte4) << 24)) >> 24;
                
                switch(byte1)
                {
                    case 0xD8:
                        this->OnAddIntlit8(byte2, byte3, int1);
                        break;
                    case 0xD9:
                        this->OnRsubIntlit8(byte2, byte3, int1);
                        break;
                    case 0xDA:
                        this->OnMulIntlit8(byte2, byte3, int1);
                        break;
                    case 0xDB:
                        this->OnDivIntlit8(byte2, byte3, int1);
                        break;
                    case 0xDC:
                        this->OnRemIntlit8(byte2, byte3, int1);
                        break;
                    case 0xDD:
                        this->OnAndIntlit8(byte2, byte3, int1);
                        break;
                    case 0xDE:
                        this->OnOrIntlit8(byte2, byte3, int1);
                        break;
                    case 0xDF:
                        this->OnXorIntlit8(byte2, byte3, int1);
                        break;
                    case 0xE0:
                        this->OnShlIntlit8(byte2, byte3, int1);
                        break;
                    case 0xE1:
                        this->OnShrIntlit8(byte2, byte3, int1);
                        break;
                    case 0xE2:
                        this->OnUshrIntlit8(byte2, byte3, int1);
                        break;
                }
                
                break;
                
                case 0xE3:
                case 0xE4:
                case 0xE5:
                case 0xE6:
                case 0xE7:
                case 0xE8:
                case 0xE9:
                case 0xEA:
                case 0xEB:
                case 0xEC:
                case 0xED:
                case 0xEE:
                case 0xEF:
                case 0xF0:
                case 0xF1:
                case 0xF2:
                case 0xF3:
                case 0xF4:
                case 0xF5:
                case 0xF6:
                case 0xF7:
                case 0xF8:
                case 0xF9:
                case 0xFA:
                case 0xFB:
                case 0xFC:
                case 0xFD:
                case 0xFE:
                case 0xFF:
                    byte2 = this->GetNextByte();
                    this->OnUnused(byte2);
                    break;
            default:
                this->Assert(false, __LINE__);
                break;
        } // switch
        
        // 0x00 carries extra data, so we do not check its operand size
        if(byte1 != 0x00)
        {
            // Check consistency
            this->Assert(this->GetOperandSize(byte1) == \
                         (this->current_ptr - \
                          this->bytecode - \
                          this->current_opcode_offset - \
                          1),
                         __LINE__);
        }
    }
    
    this->OnFinish();
    
    return;
}

/*
 * GetOperandSize() - Return the size of opernad, not including opcode itself
 *
 * This is usually used to compute the starting offset of an operand
 * when we have already read its operands
 *
 * Or used when skipping instructions
 */
int BytecodeSegment::GetOperandSize(unsigned char opcode)
{
    return FormatObjectTable[BytecodeFormatTable[opcode]].operand_size;
}    

/*
 * Skip() - Skip current instruction
 *
 * Since we have already read in the opcode, we just more forward the
 * buffer pointer by operand size and that is all
 *
 * This method does not raise any error even if the current instruction
 * is a malformed one. The exception would be raised on next retrival
 */
void BytecodeSegment::Skip(unsigned char opcode)
{
    // Called before pointer is moved
    this->OnSkip();
    // Move forward R/W pointer
    this->current_ptr += this->GetOperandSize(opcode);
    //fprintf(this->out_file, "%.2X, %d\n", opcode, this->GetOperandSize(opcode)); 
    
    return;
}
