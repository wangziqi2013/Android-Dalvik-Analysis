
#include "bytecode.h"

/*
 * Constructor - Initialize members and execution environment
 */
BytecodeSegment::BytecodeSegment(unsigned char *pbytecode,
								 FILE *pout_file,
								 FILE *perr_file)
{
	this->bytecode = pbytecode;
	this->out_file = pout_file;
	this->err_file = perr_file;
	
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
