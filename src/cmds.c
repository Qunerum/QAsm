#include <stdio.h>
#include "../include/main.h"
#include "../include/utility.h"
#include "../include/cmds.h"

static FILE* f;
void setF(FILE* file) { f = file; }
void split(char* data) { splitStart(data, ' ', ct[5], ct[6]); }

void startMain(char* args) { (void)args; fprintf(f, "_start:\n"); }
void endMain(char* args) { (void)args; fprintf(f, "\tmov rax, 60\n\tmov rdi, 0\n\tsyscall\n\tret\n"); }

void step(char* args) { fprintf(f, "%s: ; Step\n", args); }
void es(char* args) { (void)args; fprintf(f, "\tret ; End step\n"); }
void call(char* args) { (void)args; fprintf(f, "\tcall %s ; Call step\n", args); }

void prt(char* args) { fprintf(f, "\tlea rsi, %s\n\tmov rdx, len_%s\n\tcall prt ; Print text\n", args, args); }
void prtn(char* args) { fprintf(f, "\tmov eax, %s\n\tcall intToText\n\tcall prt ; Print number\n", args); }
void prti(char* args) { fprintf(f, "\tmov eax, [rel %s]\n\tcall intToText\n\tcall prt ; Print int\n", args); }
void prtln(char* args) { (void)args; fprintf(f, "\tcall prtln ; Print line\n"); }

void add(char* args) { split(args); fprintf(f, "\tadd dword [rel %s], %s ; Add\n", ct[5], ct[6]); }
void sub(char* args) { split(args); fprintf(f, "\tsub dword [rel %s], %s ; Sub\n", ct[5], ct[6]); }
void mul(char* args) { split(args); fprintf(f, "\tmov eax, [rel %s]\n\timul eax, %s\n\tmov [rel %s], eax ; Mul\n", ct[5], ct[6], ct[5]); }
void div(char* args) { split(args); fprintf(f, "\tmov eax, [rel %s]\n\tcdq\n\tmov ecx, %s\n\tidiv ecx\n\tmov [rel %s], eax ; Div\n", ct[5], ct[6], ct[5]); }

void insertTab(char* args) { fprintf(f, "\t%s\n", args); }
void insert(char* args) { fprintf(f, "%s\n", args); }

qaCmd cmds[] = {
    {"sm", startMain},
    {"em", endMain},

    {"step", step},
    {"es", es},
    {"call", call},

    {"prt", prt},
    {"prtn", prtn},
    {"prti", prti},
    {"prtln", prtln},

    {"add", add},
    {"sub", sub},
    {"mul", mul},
    {"div", div},

    {".t", insertTab},
    {".", insert},
};
int cmd_count = sizeof(cmds) / sizeof(qaCmd);
