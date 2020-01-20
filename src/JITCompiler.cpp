#include "JITCompiler.h"
#include <afxres.h>

void JITCompiler::compile(string const& name, instruction* body, uint32_t size, uint32_t num_locals) {
    auto jit = string(name);
    jit += ".s";
    ofstream fout;
    fout.open(jit.c_str(), ios::out | ios::trunc);
    fout << ".intel_syntax noprefix" << endl
         << ".global " << name << endl
         << name << ':' << endl;

#define INIT_DISPATCH JUMP;
#define CASE(op) L_ ## op:
#define NEXT if (u++ < size) { i=*++pc; goto *table[i.type]; } else { goto L_END; }
#define JUMP if (u++ < size) { i= *pc; goto *table[i.type]; } else { goto L_END; }
#define END_DISPATCH L_END:

    uint32_t u = 0u;
    auto* pc = body;
    instruction i = *new instruction;
    static void* table[] = {
            /* 00 */ &&L_NOP,      /* 01 */ &&L_EXIT,     /* 02 */ &&L_DEBUG,  /* 03 */ &&L_NOP,
            /* 04 */ &&L_NOP,      /* 05 */ &&L_NOP,      /* 06 */ &&L_NOP,    /* 07 */ &&L_NOP,
            /* 08 */ &&L_NOP,      /* 09 */ &&L_NOP,      /* 0a */ &&L_NOP,    /* 0b */ &&L_NOP,
            /* 0c */ &&L_NOP,      /* 0d */ &&L_NOP,      /* 0e */ &&L_NOP,    /* 0f */ &&L_NOP,

            /* 10 */ &&L_NOP,      /* 11 */ &&L_NOP,      /* 12 */ &&L_CALL,   /* 13 */ &&L_RET,
            /* 14 */ &&L_NOP,      /* 15 */ &&L_NOP,      /* 16 */ &&L_NOP,    /* 17 */ &&L_NOP,
            /* 18 */ &&L_NOP,      /* 19 */ &&L_NOP,      /* 1a */ &&L_NOP,    /* 1b */ &&L_NOP,
            /* 1c */ &&L_PROLOGUE, /* 1d */ &&L_EPILOGUE, /* 1e */ &&L_NOP,    /* 1f */ &&L_NOP,

            /* 20 */ &&L_ADD,      /* 21 */ &&L_SUB,      /* 22 */ &&L_MUL,    /* 23 */ &&L_DIV,
            /* 24 */ &&L_GT,       /* 25 */ &&L_GE,       /* 26 */ &&L_LT,     /* 27 */ &&L_LE,
            /* 28 */ &&L_EQ,       /* 29 */ &&L_AND,      /* 2a */ &&L_OR,     /* 2b */ &&L_NOT,
            /* 2c */ &&L_CONST,    /* 2d */ &&L_NOP,      /* 2e */ &&L_NOP,    /* 2f */ &&L_NOP,
    };

    INIT_DISPATCH {
        CASE(NOP) {
            fout << "  nop" << endl;
        } NEXT;
        CASE(EXIT) {
            goto L_END;
        } NEXT;
        CASE(DEBUG) {
        } NEXT;

        CASE(CALL) {
            fout << "  call  " << names[i.operand1] << endl;
        } JUMP;
        CASE(RET) {
            fout << "  ret" << endl;
        } JUMP;
        CASE(PROLOGUE) {
            fout << "  push  rbp" << endl
                 << "  mov   rbp, rsp" << endl
                 << "  sub   rsp, " << +i.operand0 << endl;
        } NEXT;
        CASE(EPILOGUE) {
            fout << "  pop   rax" << endl
                 << "  mov   rsp, rbp" << endl
                 << "  pop   rbp" << endl;
        } NEXT;

        CASE(ADD) {
            fout << "  pop   rdi" << endl
                 << "  pop   rax" << endl
                 << "  add   rax, rdi" << endl
                 << "  push  rax" << endl;
        } NEXT;
        CASE(SUB) {
            fout << "  pop   rdi" << endl
                 << "  pop   rax" << endl
                 << "  sub   rax, rdi" << endl
                 << "  push  rax" << endl;
        } NEXT;
        CASE(MUL) {
            fout << "  pop   rdi" << endl
                 << "  pop   rax" << endl
                 << "  mul   rax, rdi" << endl
                 << "  push  rax" << endl;
        } NEXT;
        CASE(DIV) {
            fout << "  pop   rdi" << endl
                 << "  pop   rax" << endl
                 << "  cqo" << endl
                 << "  idiv  rdi" << endl
                 << "  push  rax" << endl;
        } NEXT;
        CASE(GT) {
            fout << "  pop   rax" << endl
                 << "  pop   rdi" << endl
                 << "  cmp   rax, rdi" << endl
                 << "  setl   al" << endl
                 << "  movzb rax,  al" << endl;
        } NEXT;
        CASE(GE) {
            fout << "  pop   rax" << endl
                 << "  pop   rdi" << endl
                 << "  cmp   rax, rdi" << endl
                 << "  setle  al" << endl
                 << "  movzb rax,  al" << endl;
        } NEXT;
        CASE(LT) {
            fout << "  pop   rdi" << endl
                 << "  pop   rax" << endl
                 << "  cmp   rax, rdi" << endl
                 << "  setl   al" << endl
                 << "  movzb rax, al" << endl;
        } NEXT;
        CASE(LE) {
            fout << "  pop   rdi" << endl
                 << "  pop   rax" << endl
                 << "  cmp   rax, rdi" << endl
                 << "  setle  al" << endl
                 << "  movzb rax,  al" << endl;
        } NEXT;
        CASE(EQ) {
            fout << "  pop   rdi" << endl
                 << "  pop   rax" << endl
                 << "  cmp   rax, rdi" << endl
                 << "  sete  al" << endl
                 << "  movzb rax,  al" << endl;
        } NEXT;
        CASE(AND) {
            fout << "  pop   rdi" << endl
                 << "  pop   rax" << endl
                 << "  and   rax, rdi" << endl
                 << "  push  rax" << endl;
        } NEXT;
        CASE(OR) {
            fout << "  pop   rdi" << endl
                 << "  pop   rax" << endl
                 << "  or    rax, rdi" << endl
                 << "  push  rax" << endl;
        } NEXT;
        CASE(NOT) {
            fout << "  pop   rax" << endl
                 << "  not   rax" << endl
                 << "  push  rax" << endl;
        } NEXT;
        CASE(CONST) {
            fout << "  push  " << +i.operand0 << endl;
        } NEXT;
    } END_DISPATCH;

    string tmps;

    // assemble
    tmps = string(R"(gcc -shared )");
    tmps += name; // TODO absolute path
    tmps += R"(.c -o )";
    tmps += name;
    tmps += R"(.dll)";
    system(tmps.c_str());

// dependent-platform macros
#ifdef _WIN32

#include <windows.h>
#define dll_open(dll_file_name) LoadLibrary(dll_file_name)
#define dll_get(dll_handle, dll_procedure_name) GetProcAddress(dll_handle, dll_procedure_name)
#define dll_close(dll_handle) FreeLibrary(dll_handle)

#else

#include <dlfcn.h>
#define dll_open(dll_file_name) dlopen(dll_file_name, RTLD_LAZY)
#define dll_get(dll_handle, dll_procedure_name) dlsym(dll_handle, dll_procedure_name)
#define dll_close(dll_handle) dlclose(dll_handle)

#endif
    // execute
    typedef uint32_t (*native_function)(uint32_t, ...);
    tmps.clear();
    tmps += name; // TODO absolute path
    tmps += R"(.dll)";
    auto handle = dll_open(tmps.c_str());
    if (!handle) {
        cout << "did't open normally" << endl;
        exit(1);
    }
    auto procedure = dll_get(handle, name.c_str());
    if (!procedure) {
        cout << "did't get normally" << endl;
        exit(1);
    }
    cout << reinterpret_cast<native_function>(procedure)(0) << endl;
}