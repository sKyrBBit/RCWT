#ifndef RCWT_JITCOMPILER_H
#define RCWT_JITCOMPILER_H

#include "rcwt.h"

class JITCompiler {
public:
    JITCompiler(char** names) {
        this->names = names;
    }
    void compile(string const& ,instruction*, uint32_t);
    char** names;
};


#endif //RCWT_JITCOMPILER_H
