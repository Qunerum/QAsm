#include <stdio.h>
#include "../include/main.h"
#include "../include/memory.h"
#include "../include/utility.h"
#include "../include/cmds.h"

void cmd_write_file(const char* filename, const char* content) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Cannot open file %s to write.\n", filename);
        return;
    }
    fprintf(file, "%s", content);
    fclose(file);
}

char* ct[CT_COUNT];
void ctc(int i) { ct[i][0] = '\0'; }
FILE* f;
int main(int argc, char* argv[]) {
    init_memory();
    char* targetFile;
    if (argc < 2) { targetFile = "program.qa"; } else { targetFile = argv[1]; }
    FILE* file = fopen(targetFile, "r");
    if (file == NULL && is(targetFile, "program.qa")) {
        cmd_write_file("program.qa", "data\n    string text Hello, World!;\nend\nprintln text");
        file = fopen("program.qa", "r");
    }
    if (file == NULL) { printf("Error: Cannot open or create file '%s'!\n", targetFile); return 1; }
    for (int i = 0; i < CT_COUNT; i++) { ct[i] = (char*)kmalloc(MAX_LINE_SIZE); }
    f = fopen("bin/main.asm", "w");
    setF(f);
    if (f == NULL) { printf("Error: Cannot create file 'main.asm' !\n"); return 1; }
    while (fgets(ct[0], MAX_LINE_SIZE, file) != NULL) { if (ct[0][0] == '\n' || ct[0][0] == '\0') continue; runLine(ct[0]); }
    fclose(file);
    fclose(f);
    for (int i = 0; i < CT_COUNT; i++) { kfree(ct[i]); }
    return 0;
}
static int dataLoaded = 0, dataCollecting = 0;
void runLine(char* line) {
    ctc(3); ctc(4); ctc(5); ctc(6); ctc(7);
    trimStart(line, ' ', ct[3]);
    if (ct[3][0] == ']') { return; }
    int l = len(ct[3]) - 1;
    if (l >= 0 && ct[3][l] == '\n') { ct[3][l] = '\0'; }
    if (ct[3][0] == '\0' || ct[3][0] == '\r') { return; }
    splitStart(ct[3], ' ', ct[4], ct[7]);

    if (dataLoaded) { for (int i = 0; i < cmd_count; i++) { if (is(cmds[i].cmd, ct[4])) { cmds[i].data(ct[7]); return; } } } else {
        if (startWith(ct[3], "data")) {
            dataCollecting = 1;
            fprintf(f, "default rel\n");
            fprintf(f, "section .data\n");
            fprintf(f, "\tnl db 10\n");
            fprintf(f, "\tnll equ $ - nl\n");
            return;
        }
        if (dataCollecting && startWith(ct[3], "end")) {
            dataLoaded = 1;
            dataCollecting = 0;
            fprintf(f, "section .bss\n");
            fprintf(f, "\tbufor resb 12\n");
            fprintf(f, "section .text\n");
            fprintf(f, "\tglobal _start\n");
            fprintf(f, "intToText:\n");
            fprintf(f, "\tmov rcx, 0\n");
            fprintf(f, "\tmov ebx, 10\n");
            fprintf(f, ".ittLoop:\n");
            fprintf(f, "\tmov edx, 0\n");
            fprintf(f, "\tdiv ebx\n");
            fprintf(f, "\tadd edx, 48\n");
            fprintf(f, "\tpush rdx\n");
            fprintf(f, "\tinc rcx\n");
            fprintf(f, "\tcmp eax, 0\n");
            fprintf(f, "\tjne .ittLoop\n");
            fprintf(f, "\tlea rdi, [rel bufor]\n");
            fprintf(f, "\tmov rdx, rcx\n");
            fprintf(f, "\tlea rsi, [rel bufor]\n");
            fprintf(f, ".ittLoopWrite:\n");
            fprintf(f, "\tpop rax\n");
            fprintf(f, "\tmov [rdi], al\n");
            fprintf(f, "\tinc rdi\n");
            fprintf(f, "\tloop .ittLoopWrite\n");
            fprintf(f, "\tinc rdx\n");
            fprintf(f, "\tret\n");
            // if Linux
            fprintf(f, "prt:\n");
            fprintf(f, "\tmov rax, 1\n");
            fprintf(f, "\tmov rdi, 1\n");
            fprintf(f, "\tsyscall\n");
            fprintf(f, "\tret\n");
            fprintf(f, "_start:\n");
            return;
        }
        if (dataCollecting && !dataLoaded) {
            splitStart(ct[7], ' ', ct[5], ct[6]);
            if (is(ct[4], "int")) { fprintf(f, "\tvint_%s dd %s\n", ct[5], ct[6]); return; }
            if (is(ct[4], "string")) { fprintf(f, "\tvstr_%s db %s\n    len_%s equ $ - vstr_%s\n", ct[5], ct[6], ct[5], ct[5]); return; }
        }
    }
}
