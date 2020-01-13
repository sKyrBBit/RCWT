#ifndef RCWT_VIRTUALMACHINE_H
#define RCWT_VIRTUALMACHINE_H

#include "rcwt.h"
#include "Converter.h"
#include "JITCompiler.h"

class VirtualMachine {
public:
	VirtualMachine() {
        this->call_stack.clear();
        this->registers.clear();
        this->functions.clear();
        this->stack_pointer = &call_stack.front();
        this->base_pointer = nullptr;
	    this->classes = vector<class_>();
	    this->converter = new Converter;
	    this->compiler = new JITCompiler;
	    this->free_pointer = (uint32_t*) malloc(1024);
	    this->limit_pointer = free_pointer + 1024;
	}
	void execute(class_*);
	void garbage_collect();
    Converter* converter; // TODO
    JITCompiler* compiler;
private:
    vector<uint32_t> call_stack;
    vector<uint32_t> registers;
    vector<function*> functions;
    /// a register which points the top of call stack
    uint32_t* stack_pointer;
    /// a register which points the begining of arguments and locals
    uint32_t* base_pointer;
    /// a register which points the begining of free heap memory
    uint32_t* free_pointer;
    /// a register which points the limit of heap memory
    uint32_t* limit_pointer;

    vector<class_> classes;
    /// push data to call stack
	void push(uint32_t);
    /// pop data from call stack
	uint32_t pop();
};

#endif