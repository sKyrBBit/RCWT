#include "VirtualMachine.h"

void VirtualMachine::push(uint32_t i) {
	call_stack.push_back(i);
	stack_pointer++;
}
uint32_t VirtualMachine::pop() {
	auto tmp = call_stack.back();
	call_stack.pop_back();
	stack_pointer--;
	return tmp;
}
void VirtualMachine::execute(class_* entry) {
// direct threading
#define INIT_DISPATCH JUMP;
#define CASE(op) L_ ## op:
#define NEXT i=*++pc; goto *table[i.type]
#define JUMP i=*pc; goto *table[i.type]
#define END_DISPATCH L_END:
    auto* pc = (instruction*) &entry->methods[entry->entry_point];
    instruction i = *new instruction;
    static void* table[] = {
        /* 00 */ &&L_NOP,   /* 01 */ &&L_EXIT,  /* 02 */ &&L_DEBUG,  /* 03 */ &&L_NOP,
        /* 04 */ &&L_NOP,   /* 05 */ &&L_NOP,   /* 06 */ &&L_NOP,    /* 07 */ &&L_NOP,
        /* 08 */ &&L_NOP,   /* 09 */ &&L_NOP,   /* 0a */ &&L_NOP,    /* 0b */ &&L_NOP,
        /* 0c */ &&L_NOP,   /* 0d */ &&L_NOP,   /* 0e */ &&L_NOP,    /* 0f */ &&L_NOP,

        /* 10 */ &&L_COPY,  /* 11 */ &&L_SWAP,  /* 12 */ &&L_CALL,   /* 13 */ &&L_RET,
        /* 14 */ &&L_PUSH,  /* 15 */ &&L_POP,   /* 16 */ &&L_SET,    /* 17 */ &&L_GET,
        /* 18 */ &&L_STORE, /* 19 */ &&L_LOAD,  /* 1a */ &&L_GOTO,   /* 1b */ &&L_NEW,
        /* 1c */ &&L_NOP,   /* 1d */ &&L_NOP,   /* 1e */ &&L_NOP,    /* 1f */ &&L_NOP,

        /* 20 */ &&L_ADD,   /* 21 */ &&L_SUB,   /* 22 */ &&L_MUL,    /* 23 */ &&L_DIV,
	    /* 24 */ &&L_GT,    /* 25 */ &&L_GE,    /* 26 */ &&L_LT,     /* 27 */ &&L_LE,
        /* 28 */ &&L_EQ,    /* 29 */ &&L_AND,   /* 2a */ &&L_OR,     /* 2b */ &&L_NOT,
        /* 2c */ &&L_CONST, /* 2d */ &&L_NOP,   /* 2e */ &&L_NOP,    /* 2f */ &&L_NOP,
    };
	INIT_DISPATCH {
        CASE(NOP) {
		} NEXT;
		CASE(EXIT) {
			goto L_END;
		} NEXT;
		CASE(DEBUG) {
            pc++;
		    printf("debug | type: %x operand0: %x operand1: %x operand2: %x\n",
		            i.type, i.operand0, i.operand1, i.operand2);
		} JUMP;

		CASE(COPY) {
		    registers[i.operand0] = registers[i.operand1];
		} NEXT;
		CASE(SWAP) {
            auto tmp = registers[i.operand0];
            registers[i.operand0] = registers[i.operand1];
            registers[i.operand1] = tmp;
		} NEXT;
        CASE(GOTO) {
            pc += i.operand0;
        } JUMP;
        CASE(CALL) {
            push((uint32_t) base_pointer);
            base_pointer = stack_pointer;
            for (uint32_t u = 0; u < i.operand2; u++) push(0u);
            push((uint32_t) pc);
            push((uint32_t) &registers[i.operand0]);
            pc = (instruction*) &functions[i.operand1];

            compiler->compile("test", functions[i.operand1]->body, 1, i.operand2); // TODO
        } JUMP;
        CASE(RET) {
            *(uint32_t*) pop() = i.operand0;
            pc = (instruction*) pop();
            stack_pointer = base_pointer;
            base_pointer = (uint32_t*) pop();
        } JUMP;
        CASE(PUSH) {
            push(registers[i.operand0]);
        } NEXT;
        CASE(POP) {
            registers[i.operand0] = pop();
        } NEXT;
        CASE(SET) {
            ((uint32_t*) registers[i.operand0])[i.operand1] = registers[i.operand2];
        } NEXT;
        CASE(GET) {
            registers[i.operand2] = ((uint32_t*) registers[i.operand0])[i.operand1];
        } NEXT;
        CASE(STORE) {
            base_pointer[i.operand0] = registers[i.operand1];
        } NEXT;
        CASE(LOAD) {
            registers[i.operand1] = base_pointer[i.operand0];
        } NEXT;
        CASE(NEW) {
            auto tmp = free_pointer;
            while (free_pointer + i.operand1 > limit_pointer) garbage_collect();
            free_pointer += i.operand1;
            registers[i.operand0] = (uint32_t) tmp;
        } NEXT;

		CASE(ADD) {
		    registers[i.operand0] = registers[i.operand1] + registers[i.operand2];
		} NEXT;
		CASE(SUB) {
            registers[i.operand0] = registers[i.operand1] - registers[i.operand2];
		} NEXT;
        CASE(MUL) {
            registers[i.operand0] = registers[i.operand1] * registers[i.operand2];
        } NEXT;
        CASE(DIV) {
            registers[i.operand0] = registers[i.operand1] / registers[i.operand2];
        } NEXT;
        CASE(GT) {
            registers[i.operand0] = registers[i.operand1] > registers[i.operand2];
        } NEXT;
        CASE(GE) {
            registers[i.operand0] = registers[i.operand1] >= registers[i.operand2];
        } NEXT;
        CASE(LT) {
            registers[i.operand0] = registers[i.operand1] < registers[i.operand2];
        } NEXT;
        CASE(LE) {
            registers[i.operand0] = registers[i.operand1] <= registers[i.operand2];
        } NEXT;
        CASE(EQ) {
            registers[i.operand0] = registers[i.operand1] == registers[i.operand2];
        } NEXT;
        CASE(AND) {
            registers[i.operand0] = registers[i.operand1] && registers[i.operand2];
        } NEXT;
        CASE(OR) {
            registers[i.operand0] = registers[i.operand1] || registers[i.operand2];
        } NEXT;
        CASE(NOT) {
            registers[i.operand0] = !registers[i.operand1];
        } NEXT;
        CASE(CONST) {
            registers[i.operand0] = i.operand1;
        } NEXT;
    } END_DISPATCH;
}
void VirtualMachine::garbage_collect() {
    // TODO
}