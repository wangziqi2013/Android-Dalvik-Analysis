
#include "bytecode.h"

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
	this->Assert(this->code_len != 0);
	
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
void BytecodeSegment::Assert(bool cond)
{
	if(!cond)
	{
		fprintf(this->err_file, 
				"Assertion fails @ LINE %d FILE %s\n",
				__LINE__,
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
	this->Assert(false);
	while(true) ;
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
	return (this->current_ptr - this->bytecode + 1) >= this->code_len;
}

/*
 * PrintLineNum() - Print current instruction number
 *
 * Will print in left justified, 8 digit hex format plus a space
 */
inline void BytecodeSegment::PrintLineNum()
{
	fprintf(this->out_file, "%-8X ", this->instruction_counter);
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

void BytecodeSegment::OnNop(unsigned char null)
{
	// For nop instruction we require its high byte being 0x00
	this->Assert(null == 0x00);
	
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
	this->Assert(null == 0x00);
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
		unsigned int uint1;
		int int1;
		
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
			case 0x13:
				byte2 = this->GetNextByte();
				word2 = this->GetNextShort();
				int1 = ((int)(((unsigned int)word2) << 16)) >> 16;
				this->OnConst16(byte2, int1);
			case 0x14:
				byte2 = this->GetNextByte();
				uint1 = this->GetNextInt();
				this->OnConst(byte2, uint1);
			default:
				// If it could not been recognized by word-length
				// instruction then we do it in byte manner
				break;
		}
	}
	
	this->OnFinish();
	
	return;
}
