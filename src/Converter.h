#ifndef RCWT_CONVERTER_H
#define RCWT_CONVERTER_H

#include <utility>

#include "rcwt.h"

class Converter {
private:
    /// placeholders are what certify spatial locality of
    /// methods, constructors, constant_pool, fields and their pointers
    vector<uint32_t> value_placeholder;
    vector<uint32_t> pointer_placeholder;
    vector<class_> classes;
    /// input is into what this converter read files
    vector<uint8_t> input;
    uint32_t position;
    /// read*() are utility methods to read files into memory
    uint8_t* read(uint32_t);
    uint32_t* read_int();
    function* read_function();
    function* read_methods(uint32_t);
    function* read_constructors();
    immediate* read_immediate();
    immediate* read_constant_pool();
public:
    Converter() {
        this->value_placeholder.clear();
        this->pointer_placeholder.clear();
        this->input.clear();
        this->position = 0u;
    }
    void IRtoWC(string const& file_name);
    class_* IRtoMemory(string const& file_name);
    class_* WCtoMemory(string const& file_name);
};


#endif
