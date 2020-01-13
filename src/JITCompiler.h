#ifndef RCWT_JITCOMPILER_H
#define RCWT_JITCOMPILER_H

#include "rcwt.h"

class JITCompiler {
public:
    JITCompiler() {
        this->names = new char*[3];
        this->names[0] = "main";
        this->names[1] = "f";
        this->names[2] = "g";
        this->general_registers = new char*[8];
        this->general_registers[0] = "r8";
        this->general_registers[1] = "r9";
        this->general_registers[2] = "r10";
        this->general_registers[3] = "r11";
        this->general_registers[4] = "r12";
        this->general_registers[5] = "r13";
        this->general_registers[6] = "r14";
        this->general_registers[7] = "r15";
    }
    void compile(string const& ,instruction*, uint32_t, uint32_t);
    char** names;
private:
    char** general_registers;
};


#endif //RCWT_JITCOMPILER_H
