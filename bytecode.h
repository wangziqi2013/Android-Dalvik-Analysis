
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
	// The following two must be already opened (e.g. stdout, stderr)
	// Where output should go, if any
	FILE *out_file;
	// Where errors should go, if any
	FILE *err_file;
	
	BytecodeSegment(unsigned char *pbytecode, 
				    FILE *pout_file, 
					FILE *perr_file);
	~BytecodeSegment();

private:
	inline void Assert(bool cond);
};

#endif
