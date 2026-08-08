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
void prtln(char* args) { (void)args; fprintf(f, "\tcall prtln ; Print line\n"); }

void prtn(char* args) { fprintf(f, "\tRT a, %s\n\tcall intToText\n\tcall prt ; Print number\n", args); }
void prtnb(char* args) { split(args); fprintf(f, "\tRT a, %s\n\tRT b, %s\n\tcall prtB ; Print binary number\n", ct[5], ct[6]); }
void prtnh(char* args) { split(args); fprintf(f, "\tRT a, %s\n\tRT b, %s\n\tcall prtH ; Print HEX number\n", ct[5], ct[6]); }

void prti(char* args) { fprintf(f, "\tRT a, [rel %s]\n\tcall intToText\n\tcall prt ; Print int\n", args); }
void prtib(char* args) { split(args); fprintf(f, "\tRT a, [rel %s]\n\tRT b, %s\n\tcall prtB ; Print binary int\n", ct[5], ct[6]); }
void prtih(char* args) { split(args); fprintf(f, "\tRT a, [rel %s]\n\tRT b, %s\n\tcall prtH ; Print HEX int\n", ct[5], ct[6]); }

void add(char* args) { split(args); fprintf(f, "\tadd dword [rel %s], %s ; Add\n", ct[5], ct[6]); }
void addi(char* args) { split(args); fprintf(f, "\tadd dword [rel %s], [rel %s] ; Add\n", ct[5], ct[6]); }

void sub(char* args) { split(args); fprintf(f, "\tsub dword [rel %s], %s ; Sub\n", ct[5], ct[6]); }
void subi(char* args) { split(args); fprintf(f, "\tsub dword [rel %s], [rel %s] ; Sub\n", ct[5], ct[6]); }

void mul(char* args) { split(args); fprintf(f, "\tmov eax, [rel %s]\n\timul eax, %s\n\tmov [rel %s], eax ; Mul\n", ct[5], ct[6], ct[5]); }
void muli(char* args) { split(args); fprintf(f, "\tmov eax, [rel %s]\n\timul eax, [rel %s]\n\tmov [rel %s], eax ; Mul\n", ct[5], ct[6], ct[5]); }

void div(char* args) { split(args); fprintf(f, "\tmov eax, [rel %s]\n\tcdq\n\tmov ecx, %s\n\tidiv ecx\n\tmov [rel %s], eax ; Div\n", ct[5], ct[6], ct[5]); }
void divi(char* args) { split(args); fprintf(f, "\tmov eax, [rel %s]\n\tcdq\n\tmov ecx, [rel %s]\n\tidiv ecx\n\tmov [rel %s], eax ; Div\n", ct[5], ct[6], ct[5]); }

void sam(char* args) { fprintf(f, "\tsub rsp, 8\n\tmov dword [rsp], %s\n", args); }
void eam(char* args) { fprintf(f, "\tadd rsp, 8\n"); }

void insertTab(char* args) { fprintf(f, "\t%s\n", args); }
void insert(char* args) { fprintf(f, "%s\n", args); }

qaCmd cmds[] = {
    {"sm", startMain},
    {"em", endMain},

    {"step", step},
    {"es", es},
    {"call", call},

    {"prt", prt},
    {"prtln", prtln},

    {"prtn", prtn},
    {"prtnb", prtnb},
    {"prtnh", prtnh},

    {"prti", prti},
    {"prtib", prtib},
    {"prtih", prtih},

    {"add", add},
    {"addi", addi},
    {"sub", sub},
    {"subi", subi},
    {"mul", mul},
    {"muli", muli},
    {"div", div},
    {"divi", divi},

    {"sam", sam},
    {"eam", eam},

    {".t", insertTab},
    {".", insert},
};
int cmd_count = sizeof(cmds) / sizeof(qaCmd);
