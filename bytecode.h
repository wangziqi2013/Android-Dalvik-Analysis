
#ifndef _BYTECODE_H
#define _BYTECODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <vector>
#include "dex.h"

using namespace std;

struct BytecodeSegment
{
	// Input buffer
	// We do not support file input, since file I/O is slow
	// compared with memory buffered analysis
	unsigned char *bytecode;
	unsigned char *current_ptr;
	
	unsigned int code_len;
	// The following two must be already opened (e.g. stdout, stderr)
	// Where output should go, if any
	FILE *out_file;
	// Where errors should go, if any
	FILE *err_file;
	
	BytecodeSegment(unsigned char *pbytecode, 
					unsigned int pcode_len,
				    FILE *pout_file, 
					FILE *perr_file);
	~BytecodeSegment();
	
	void Dispatch();

private:
	inline void Assert(bool cond);
	inline unsigned char GetNextByte();
	inline unsigned short GetNextShort();
	inline unsigned int GetNextInt();
	inline bool IsEof();
	inline void PrintLineNum();

public:
	// This is defined for callbacks to refer
	// and will be incremented everytime after a new instruction is issued
	unsigned int instruction_counter;
	
	void OnStart();
	void OnFinish();
	
	void OnNop();
	void OnMove(unsigned char dest, unsigned char src);
};

#endif
