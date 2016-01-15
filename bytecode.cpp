
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
	[_51l] = 5,
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
	
	return ((unsigned short)byte2 << 8)	| (unsigned short)byte1;
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
	fprintf(this->out_file, "%-8X ", this->instruction_counter);
	this->instruction_counter++;
	return;	
}

////////////////////////////////////////////////////////////////
// Call back function block start //////////////////////////////
////////////////////////////////////////////////////////////////

// The following code does not have comment header
// Please refer to the header file for information

void BytecodeSegment::OnStart()
{
	fprintf(this->out_file, "Dispatch starts\n");
	
	// Line number starts at 1
	this->instruction_counter = 1;
}

void BytecodeSegment::OnFinish()
{
	fprintf(this->out_file, "Dispatch finishes\n");
}

void BytecodeSegment::OnSkip()
{
	this->PrintLineNum();
	fprintf(this->out_file, "[Skipped]\n");	
}

void BytecodeSegment::OnNop(unsigned char null)
{
	// For nop instruction we require its high byte being 0x00
	this->Assert(null == 0x00, __LINE__);
	
	this->PrintLineNum();
	fprintf(this->out_file, "nop\n");
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
	fprintf(this->out_file, "instance-of v%u, v%u, type@%u", dest, reg, index);	
}

void BytecodeSegment::OnArrayLength(unsigned char dest, unsigned char reg)
{
	this->PrintLineNum();
	fprintf(this->out_file, "array-length v%u, v%u", dest, reg);	
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
	fprintf(this->out_file, "new-array v%u, v%u, type@%u", dest, size, index);
}

void BytecodeSegment::OnInvokeVirtual(unsigned short index, 
									  unsigned short start,
									  unsigned char count)
{
	this->PrintLineNum();
	fprintf(this->out_file, 
			"invoke-virtual/range{v%.4X..v%.4X}, method@%u",
			start,
			start + count - 1,
			index);	
}

void BytecodeSegment::OnInvokeSuper(unsigned short index, 
									unsigned short start,
									unsigned char count)
{
	this->PrintLineNum();
	fprintf(this->out_file, 
			"invoke-super/range{v%.4X..v%.4X}, method@%u",
			start,
			start + count - 1,
			index);	
}

void BytecodeSegment::OnInvokeDirect(unsigned short index, 
									 unsigned short start,
									 unsigned char count)
{
	this->PrintLineNum();
	fprintf(this->out_file, 
			"invoke-direct/range{v%.4X..v%.4X}, method@%u",
			start,
			start + count - 1,
			index);	
}

void BytecodeSegment::OnInvokeStatic(unsigned short index, 
									 unsigned short start,
									 unsigned char count)
{
	this->PrintLineNum();
	fprintf(this->out_file, 
			"invoke-static/range{v%.4X..v%.4X}, method@%u",
			start,
			start + count - 1,
			index);	
}

void BytecodeSegment::OnInvokeInterface(unsigned short index, 
									    unsigned short start,
									    unsigned char count)
{
	this->PrintLineNum();
	fprintf(this->out_file, 
			"invoke-interface/range{v%.4X..v%.4X}, method@%u",
			start,
			start + count - 1,
			index);	
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
		unsigned char byte2;
		unsigned short word1, word2;
		unsigned int uint1, uint2;
		int int1;
		long long lld1;
		unsigned long long llu1;
		
		fprintf(out_file, "%.2X\n", byte1);
		switch(byte1)
		{
			case 0x00:
				byte2 = this->GetNextByte();
				this->OnNop(byte2);
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
						this->OnInvokeVirtual(word1, word2, byte2);
						break;
					case 0x75:
						this->OnInvokeSuper(word1, word2, byte2);
						break;
					case 0x76:	
						this->OnInvokeDirect(word1, word2, byte2);
						break;
					case 0x77:
						this->OnInvokeStatic(word1, word2, byte2);
						break;
					case 0x78:
						this->OnInvokeInterface(word1, word2, byte2);
						break;
					default:
						this->Assert(false, __LINE__);
						break;
				}
				
				break;
			default:
				this->Skip(byte1);
				break;
		}
	}
	
	this->OnFinish();
	
	return;
}

/*
 * GetOperandSize() - Return the size of opernad, not including opcode itself 
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
	fprintf(this->out_file, "%.2X, %d\n", opcode, this->GetOperandSize(opcode)); 
	
	return;
}
