#ifndef RCWT_RCWT_H
#define RCWT_RCWT_H

#include <iomanip>
#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

union immediate;
struct instruction;
struct function;
struct class_;
struct object_;
struct frame;

union immediate {
    uint32_t i;
    bool b;
    uint8_t* s;
    immediate (*f)(immediate);
    object_* r;
    /*
     * void(*p)(immediate);
     * immediate (*c)();
     */
    immediate(uint32_t i)                { this->i = i; }
    immediate(bool b)                    { this->b = b; }
    immediate(uint8_t* s)                { this->s = s; }
    immediate(immediate (*f)(immediate)) { this->f = f; }
    immediate(object_* r)                { this->r = r; }
    /*
     * immediate(void (*p)(immediate))      { this->p = p; }
     * immediate(immediate (*c)())          { this->c = c; }
     */
};
struct instruction {
    uint8_t type;
    uint8_t operand0;
    uint8_t operand1;
    uint8_t operand2;
};
struct function {
    instruction* body;
};
struct class_ {
    immediate* constant_pool;
    uint32_t field_count;
    uint32_t method_count;
    function* methods;
    function* constructors;
    uint32_t entry_point;
};

struct object_ {
    immediate* fields;
    function* methods;
};
struct frame {
    immediate* locals;
    instruction* ret;
    object_* self;
};

#endif // LISP_LISP_H