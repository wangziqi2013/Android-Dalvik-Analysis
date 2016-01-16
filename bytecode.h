
#ifndef _BYTECODE_H
#define _BYTECODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <vector>
#include "dex.h"

using namespace std;

/*
 * Defines symbolic names for instruction types
 */
enum
{
	_10x,
	_12x,
	_11n,
	_11x,
	_10t,
	_20t,
	_20bc,
	_22x,
	_21t,
	_21s,
	_21h,
	_21c,
	_23x,
	_22b,
	_22t,
	_22s,
	_22c,
	_22cs,
	_30t,
	_32x,
	_31i,
	_31t,
	_31c,
	_35c,
	_35ms,
	_35mi,
	_3rc,
	_3rms,
	_3rmi,
	_51l,
};

struct BytecodeFormat
{
	int operand_size;
	int operand_num;
	
	BytecodeFormat(int osize)
	{
		this->operand_size = osize;
		return;	
	}
};

struct BytecodeSegment
{
	// Input buffer
	// We do not support file input, since file I/O is slow
	// compared with memory buffered analysis
	unsigned char *bytecode;
	unsigned char *current_ptr;
	// Current byte offset from the start of bytecode
	// This always points to the opcode (i.e. the offset of opcode)
	// even if there are operands
	unsigned int current_opcode_offset;
	
	// Maps instruction offset to instruction index (starting from 1)
	map<unsigned int, unsigned int> instruction_offset_map;
	// Maps instruction counter to offset
	// Just a reverse mapping of instruction_offset_map
	map<unsigned int, unsigned int> instruction_counter_map;
	
	
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
	
	int GetOperandSize(unsigned char opcode);
	void Skip(unsigned char opcode);

private:
	inline void Assert(bool cond, int linenum);
	// Make byte and short fast, other cases slower by not inlining them
	inline unsigned char GetNextByte();
	inline unsigned short GetNextShort();
	unsigned int GetNextInt();
	void GetNextNBytes(unsigned char *buffer, int size);
	inline bool IsEof();
	inline void PrintLineNum();
	void PrintBuffer(unsigned char *buffer, int size);
	unsigned int ResolveOffsetToCount(unsigned int base, int offset);

public:
	// This is defined for callbacks to refer
	// and will be incremented everytime after a new instruction is issued
	unsigned int instruction_counter;
	
	// Control callbacks 
	void OnStart();
	void OnFinish();
	void OnSkip();
	void OnNextInstruction(unsigned char opcode);
	
	void OnNop(unsigned char null);
	void OnPackedSwitchPayload(unsigned short size);
	void OnSparseSwitchPayload(unsigned short size);
	
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
	
	// Array creation and initialization
	void OnArrayLength(unsigned char dest, unsigned char reg);
	void OnNewInstance(unsigned char dest, unsigned short index);
	void OnNewArray(unsigned char dest, 
					unsigned char size, 
					unsigned short index);
	void OnFilledNewArray(unsigned char size, 
						  unsigned char c, 
						  unsigned char d,
						  unsigned char e,
						  unsigned char f,
						  unsigned char g,
						  unsigned short type);
	void OnFilledNewArrayRange(unsigned char size,
							   unsigned short type,
							   unsigned short start);
	void OnFillArrayData(unsigned char reg,
						 int offset);
	void OnFillArrayDataPayload(unsigned short width, unsigned int size);
	
	// Control flow
	void OnThrow(unsigned char reg);
	void OnGoto(char offset);
	void OnGoto16(short offset, unsigned char null);
	void OnGoto32(int offset, unsigned char null);
	
	// Switch statement jump
	void OnPackedSwitch(unsigned char reg, int offset);
	void OnSparseSwitch(unsigned char reg, int offset);
	
	// Comparison
	void OnCmplFloat(unsigned char dest, 
					 unsigned char reg1, 
					 unsigned char reg2);
	void OnCmpgFloat(unsigned char dest, 
					 unsigned char reg1, 
					 unsigned char reg2);
	void OnCmplDouble(unsigned char dest, 
					  unsigned char reg1, 
					  unsigned char reg2);
	void OnCmpgDouble(unsigned char dest, 
					  unsigned char reg1, 
					  unsigned char reg2);
	void OnCmpLong(unsigned char dest, 
				   unsigned char reg1, 
				   unsigned char reg2);
	
	// If testing
	void OnIfEq(unsigned char reg1, 
				unsigned char reg2,
				int offset);
	void OnIfNe(unsigned char reg1, 
				unsigned char reg2,
				int offset);
	void OnIfLt(unsigned char reg1, 
				unsigned char reg2,
				int offset);
	void OnIfGe(unsigned char reg1, 
				unsigned char reg2,
				int offset);
	void OnIfGt(unsigned char reg1, 
				unsigned char reg2,
				int offset);
	void OnIfLe(unsigned char reg1, 
				unsigned char reg2,
				int offset);
	
	// If zero testing
	void OnIfEqZ(unsigned char reg, 
				 int offset);
	void OnIfNeZ(unsigned char reg, 
				 int offset);
	void OnIfLtZ(unsigned char reg, 
				 int offset);
	void OnIfGeZ(unsigned char reg, 
				 int offset);
	void OnIfGtZ(unsigned char reg, 
				 int offset);
	void OnIfLeZ(unsigned char reg, 
				 int offset);
	
	// Unused opcode
	void OnUnused(unsigned char null);
	
	// Get from array
	void OnAGet(unsigned char reg,
				unsigned char array,
				unsigned char index);
	void OnAGetWide(unsigned char reg,
					unsigned char array,
					unsigned char index);
	void OnAGetObject(unsigned char reg,
					  unsigned char array,
					  unsigned char index);
	void OnAGetBoolean(unsigned char reg,
					   unsigned char array,
					   unsigned char index);
	void OnAGetByte(unsigned char reg,
				 	unsigned char array,
				 	unsigned char index);
	void OnAGetChar(unsigned char reg,
				 	unsigned char array,
				 	unsigned char index);
	void OnAGetShort(unsigned char reg,
					 unsigned char array,
					 unsigned char index);
	
	// Write into array
	void OnAPut(unsigned char reg,
				unsigned char array,
				unsigned char index);
	void OnAPutWide(unsigned char reg,
					unsigned char array,
					unsigned char index);
	void OnAPutObject(unsigned char reg,
					  unsigned char array,
					  unsigned char index);
	void OnAPutBoolean(unsigned char reg,
					   unsigned char array,
					   unsigned char index);
	void OnAPutByte(unsigned char reg,
				 	unsigned char array,
				 	unsigned char index);
	void OnAPutChar(unsigned char reg,
				 	unsigned char array,
				 	unsigned char index);
	void OnAPutShort(unsigned char reg,
					 unsigned char array,
					 unsigned char index);
	
	// Method invokation
	void OnInvokeVirtual(unsigned short index, 
						 unsigned short start, 
						 unsigned char count);
	void OnInvokeSuper(unsigned short index, 
					   unsigned short start, 
					   unsigned char count);
	void OnInvokeDirect(unsigned short index, 
						unsigned short start, 
						unsigned char count);
	void OnInvokeStatic(unsigned short index, 
						unsigned short start, 
						unsigned char count);
	void OnInvokeInterface(unsigned short index, 
						   unsigned short start, 
						   unsigned char count);
};

#endif
