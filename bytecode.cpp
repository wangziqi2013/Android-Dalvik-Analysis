
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

/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////

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

void BytecodeSegment::OnNop()
{
	this->PrintLineNum();
	fprintf(this->out_file, "nop\n");
}

void BytecodeSegment::OnMove(unsigned char dest, unsigned char src)
{
	this->PrintLineNum();
	fprintf(this->out_file, "move v%u, v%u\n", dest, src);
}

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
		
		switch(byte1)
		{
			case 0x00:
				this->OnNop();
				break;
			case 0x01:
				byte2 = this->GetNextByte();
				// Dest is lower 4 bits, src is higher 4 bits
				this->OnMove(byte2 | 0x0F, byte2 >> 4);
				break;
			default:
				// If it could not been recognized by word-length
				// instruction then we do it in byte manner
				break;
		}
	}
	
	this->OnFinish();
	
	return;
}
