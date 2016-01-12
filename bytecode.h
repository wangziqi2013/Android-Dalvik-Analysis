
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
	
	void OnNop(unsigned char null);
	
	// Move word
	void OnMove(unsigned char dest, unsigned char src);
	void OnMoveFrom16(unsigned char dest, unsigned short src);
	void OnMove16(unsigned short dest, unsigned short src);
	
	// Move dword
	void OnMoveWide(unsigned char dest, unsigned char src);
	void OnMoveWideFrom16(unsigned char dest, unsigned short src);
	void OnMoveWide16(unsigned short dest, unsigned short src);
	
	// Move object
	void OnMoveObject(unsigned char dest, unsigned char src);
	void OnMoveObjectFrom16(unsigned char dest, unsigned short src);
	void OnMoveObject16(unsigned short dest, unsigned short src);
	
	// Move result
	void OnMoveResult(unsigned char dest);
	void OnMoveResultWide(unsigned char dest);
	void OnMoveResultObject(unsigned char dest);
	
	// Move exception - this is pnlu valid as the first instruction 
	// of the exception handler
	void OnMoveException(unsigned char dest);
	
	// Return
	void OnReturnVoid(unsigned char null);
	void OnReturn(unsigned char dest);
	void OnReturnWide(unsigned char dest);
	void OnReturnObject(unsigned char dest);
	
	// Load const into register
	void OnConst4(unsigned char dest, int value);
	void OnConst16(unsigned char dest, int value);
	// Since the 32 bit literal in already encoded this is 
	// likely to be an unsigned int
	void OnConst(unsigned char dest, unsigned int value);
	void OnConstHigh16(unsigned char dest, unsigned int value);
	// The value is extended to 64 bits
	void OnConstWide16(unsigned char dest, long long value); 
	void OnConstWide32(unsigned char dest, long long value);
	void OnConstWide(unsigned char dest, unsigned long long value);
	void OnConstWideHigh16(unsigned char dest, unsigned long long value);
	
	// Load object reference into register
	void OnConstString(unsigned char dest, unsigned short index);
	void OnConstStringJumbo(unsigned char dest, unsigned int index);
	void OnConstClass(unsigned char dest, unsigned short index);
	
	// Monitor
	void OnMonitorEnter(unsigned char reg);
	void OnMonitorExit(unsigned char reg);
	
	// Class hierarchy
	void OnCheckCast(unsigned char reg, unsigned short index);
	void OnInstanceOf(unsigned char dest, 
					  unsigned char reg, 
					  unsigned short index);
	
	void OnArrayLength(unsigned char dest, unsigned char reg);
	void OnNewInstance(unsigned char dest, unsigned short index);
	void OnNewArray(unsigned char dest, 
					unsigned char size, 
					unsigned short index);
};

#endif
