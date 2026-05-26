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
    if (f == NULL) { printf("Error: Cannot create file 'main.asm' !\n"); return 1; }
    while (fgets(ct[0], MAX_LINE_SIZE, file) != NULL) { if (ct[0][0] == '\n' || ct[0][0] == '\0') continue; runLine(ct[0]); }
    fprintf(f, "    mov rax, 60\n");
    fprintf(f, "    mov rdi, 0\n");
    fprintf(f, "    syscall");
    fclose(file);
    fclose(f);
    for (int i = 0; i < CT_COUNT; i++) { kfree(ct[i]); }
    return 0;
}
static int dataLoaded = 0, dataCollecting = 0;
// fprintf(f, "    \n");
void runLine(char* line) {
    ctc(3); ctc(4); ctc(5); ctc(6); ctc(7);
    trimStart(line, ' ', ct[3]);
    if (ct[3][0] == ']') { return; }
    splitStart(ct[3], ' ', ct[4], ct[7]);
    if (dataLoaded) {
        // 4 - Command
        // 5 - ;a;
        // 6 - ;b;
        // 7 - ARGS ALL
        // 8 - tmp
        for (int i = 0; i < cmd_count; i++) {
            if (is(cmds[i].cmd, ct[4])) {
                if (contains(cmds[i].data, ";a;") || contains(cmds[i].data, ";b;")) { splitStart(ct[7], ' ', ct[5], ct[6]); }

                while (contains(cmds[i].data, ";a;")) { replace(cmds[i].data, ";a;", ct[5], ct[8]); copyStr(cmds[i].data, ct[8]); }
                while (contains(cmds[i].data, ";b;")) { replace(cmds[i].data, ";b;", ct[6], ct[8]); copyStr(cmds[i].data, ct[8]); }

                fprintf(f, "    %s\n", cmds[i].data);
            }
        }
    } else {
        if (startWith(ct[3], "data")) {
            dataCollecting = 1;
            fprintf(f, "default rel\n");
            fprintf(f, "section .data\n");
            fprintf(f, "    nl db 10\n");
            fprintf(f, "    nll equ $ - nl\n");
            return;
        }
        if (dataCollecting && startWith(ct[3], "end")) {
            dataLoaded = 1;
            dataCollecting = 0;
            fprintf(f, "section .bss\n");
            fprintf(f, "    bufor resb 12\n");
            fprintf(f, "section .text\n");
            fprintf(f, "    global _start\n");
            fprintf(f, "intToText:\n");
            fprintf(f, "    mov rcx, 0\n");
            fprintf(f, "    mov ebx, 10\n");
            fprintf(f, ".ittLoop:\n");
            fprintf(f, "    mov edx, 0\n");
            fprintf(f, "    div ebx\n");
            fprintf(f, "    add edx, 48\n");
            fprintf(f, "    push rdx\n");
            fprintf(f, "    inc rcx\n");
            fprintf(f, "    cmp eax, 0\n");
            fprintf(f, "    jne .ittLoop\n");
            fprintf(f, "    lea rdi, [rel bufor]\n");
            fprintf(f, "    mov rdx, rcx\n");
            fprintf(f, "    lea rsi, [rel bufor]\n");
            fprintf(f, ".ittLoopWrite\n");
            fprintf(f, "    pop rax\n");
            fprintf(f, "    mov [rdi], al\n");
            fprintf(f, "    inc rdi\n");
            fprintf(f, "    loop .ittLoopWrite\n");
            fprintf(f, "    inc rdx\n");
            fprintf(f, "    ret\n");
            // if Linux
            fprintf(f, "prt:\n");
            fprintf(f, "    mov rax, 1\n");
            fprintf(f, "    mov rdi, 1\n");
            fprintf(f, "    syscall\n");
            fprintf(f, "    ret\n");
            fprintf(f, "_start:\n");
            return;
        }
        if (dataCollecting && !dataLoaded) {
            splitStart(ct[7], ' ', ct[5], ct[6]);
            if (is(ct[4], "int")) { fprintf(f, "    var_%s dd %s\n", ct[5], ct[6]); return; }
            if (is(ct[4], "string")) { fprintf(f, "    var_%s db %s\n    len_%s equ $ - var_%s\n", ct[5], ct[6], ct[5], ct[5]); return; }
        }
    }
}
