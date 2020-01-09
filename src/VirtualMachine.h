#ifndef RCWT_VIRTUALMACHINE_H
#define RCWT_VIRTUALMACHINE_H

#include "rcwt.h"
#include "Converter.h"

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
	}
	vector<instruction> execute(class_*);
    Converter* converter; // TODO
private:
    vector<uint32_t> call_stack;
    vector<uint32_t> registers;
    vector<function*> functions;
    uint32_t* stack_pointer;
    uint32_t* base_pointer;

    vector<class_> classes;
    vector<uint32_t> heap;
	void push(uint32_t);
	uint32_t pop();
};

#endif