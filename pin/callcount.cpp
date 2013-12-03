/*BEGIN_LEGAL 
Intel Open Source License 

Copyright (c) 2002-2013 Intel Corporation. All rights reserved.
 
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.  Redistributions
in binary form must reproduce the above copyright notice, this list of
conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.  Neither the name of
the Intel Corporation nor the names of its contributors may be used to
endorse or promote products derived from this software without
specific prior written permission.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE INTEL OR
ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
END_LEGAL */
#include <iostream>
#include <fstream>
#include "pin.H"
#include <stack>          // std::stack
#include <cstdlib>		  // std::exit

std::stack<ADDRINT> addrStack;
ofstream OutFile;

// The running count of instructions is kept here
// make it static to help the compiler optimize docount
static UINT64 ccount = 0;

// This function is called before every instruction is executed
VOID docount()
{
	ccount++;
}

VOID saveCall(ADDRINT nextip)
{
	addrStack.push(nextip);
	OutFile << "Return set: 0x" << hex << nextip << endl;
}

VOID checkRet(ADDRINT *rsp)
{
	ADDRINT retval;

	ADDRINT rspval = *rsp;
	ADDRINT *psp = (ADDRINT *)rspval;
	retval = *psp;
	ADDRINT originval = addrStack.top();
	if(!addrStack.empty()){
		addrStack.pop();
	}
	else{
		cerr << "ERROR: TOO MANY RETURNS DETECTED." << endl;
		std::exit(EXIT_FAILURE);
	}
	if(originval != retval){
		cerr << "ERROR: STACK SMASHING DETECTED: expected target 0x" << hex << originval << ", actual return target 0x" << retval << endl;
		std::exit(EXIT_FAILURE);
	}
	OutFile << "Return to: 0x" << hex << retval << endl;
}

// Pin calls this function every time a call instruction is encountered
VOID Instruction(INS ins, VOID *v)
{	
	// Insert a call to docount before every call instruction, no arguments are passed
	if (INS_IsCall(ins))
	{	
		INS_InsertCall(ins, IPOINT_BEFORE,
			(AFUNPTR)docount,
			IARG_END);
		INS_InsertCall(ins, IPOINT_BEFORE,
			AFUNPTR(saveCall),
			IARG_ADDRINT, INS_NextAddress(ins),
			IARG_END);
	}
	else if (INS_IsRet(ins))
	{
		INS_InsertCall(ins, IPOINT_BEFORE,
			AFUNPTR(checkRet),
			IARG_CALL_ORDER, CALL_ORDER_FIRST,
			IARG_REG_REFERENCE, REG_STACK_PTR,
			IARG_END);
	}
}

KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool",
    "o", "callcount.out", "specify output file name");

// This function is called when the application exits
VOID Fini(INT32 code, VOID *v)
{
    // Write to a file since cout and cerr maybe closed by the application
    OutFile.setf(ios::showbase);
    OutFile << "Count " << dec << ccount << endl;
    OutFile.close();
}

/* ===================================================================== */
/* Print Help Message                                                    */
/* ===================================================================== */

INT32 Usage()
{
    cerr << "This tool counts the number of dynamic instructions executed" << endl;
    cerr << endl << KNOB_BASE::StringKnobSummary() << endl;
    return -1;
}

/* ===================================================================== */
/* Main                                                                  */
/* ===================================================================== */
/*   argc, argv are the entire command line: pin -t <toolname> -- ...    */
/* ===================================================================== */

int main(int argc, char * argv[])
{
    // Initialize pin
    if (PIN_Init(argc, argv)) return Usage();

    OutFile.open(KnobOutputFile.Value().c_str());


    // Register Instruction to be called to instrument instructions
    INS_AddInstrumentFunction(Instruction, 0);

    // Register Fini to be called when the application exits
    PIN_AddFiniFunction(Fini, 0);
    
    // Start the program, never returns
    PIN_StartProgram();
    
    return 0;
}
