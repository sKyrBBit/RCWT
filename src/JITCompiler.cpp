#include "JITCompiler.h"

using namespace std;

void JITCompiler::compile(string const& name, instruction* body, uint32_t size, uint32_t num_locals) {
    auto jit = string(name);
    jit += ".s";
    ofstream fout;
    fout.open(jit.c_str(), ios::out | ios::trunc);
    fout << ".intel_syntax noprefix" << endl
         << ".global " << name << endl
         << name << ':' << endl
         << "  push  rbp" << endl
         << "  mov   rbp, rsp" << endl
         << "  sub   rsp, " << +num_locals << endl;

#define INIT_DISPATCH JUMP;
#define CASE(op) L_ ## op:
#define NEXT if (u++ < size) { i=*++pc; goto *table[i.type]; } else { goto L_END; }
#define JUMP if (u++ < size) { i= *pc; goto *table[i.type]; } else { goto L_END; }
#define END_DISPATCH L_END:

    uint32_t u = 0u;
    auto* pc = body;
    instruction i = *new instruction;
    static void* table[] = {
            /* 00 */ &&L_NOP,   /* 01 */ &&L_EXIT,  /* 02 */ &&L_DEBUG,  /* 03 */ &&L_NOP,
            /* 04 */ &&L_NOP,   /* 05 */ &&L_NOP,   /* 06 */ &&L_NOP,    /* 07 */ &&L_NOP,
            /* 08 */ &&L_NOP,   /* 09 */ &&L_NOP,   /* 0a */ &&L_NOP,    /* 0b */ &&L_NOP,
            /* 0c */ &&L_NOP,   /* 0d */ &&L_NOP,   /* 0e */ &&L_NOP,    /* 0f */ &&L_NOP,

            /* 10 */ &&L_NOP,   /* 11 */ &&L_NOP,   /* 12 */ &&L_CALL,   /* 13 */ &&L_RET,
            /* 14 */ &&L_NOP,   /* 15 */ &&L_NOP,   /* 16 */ &&L_NOP,    /* 17 */ &&L_NOP,
            /* 18 */ &&L_NOP,   /* 19 */ &&L_NOP,   /* 1a */ &&L_NOP,    /* 1b */ &&L_NOP,
            /* 1c */ &&L_NOP,   /* 1d */ &&L_NOP,   /* 1e */ &&L_NOP,    /* 1f */ &&L_NOP,

            /* 20 */ &&L_ADD,   /* 21 */ &&L_SUB,   /* 22 */ &&L_MUL,    /* 23 */ &&L_DIV,
            /* 24 */ &&L_GT,    /* 25 */ &&L_GE,    /* 26 */ &&L_LT,     /* 27 */ &&L_LE,
            /* 28 */ &&L_EQ,    /* 29 */ &&L_AND,   /* 2a */ &&L_OR,     /* 2b */ &&L_NOT,
            /* 2c */ &&L_CONST, /* 2d */ &&L_NOP,   /* 2e */ &&L_NOP,    /* 2f */ &&L_NOP,
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
            fout << "  pop   rax" << endl
                 << "  mov   rsp, rbp" << endl
                 << "  pop   rbp" << endl
                 << "  ret" << endl;
        } JUMP;
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

}