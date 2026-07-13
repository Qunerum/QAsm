#include <stdio.h>
#include "../include/main.h"
#include "../include/memory.h"
#include "../include/utility.h"
#include "../include/cmds.h"

#define RST      "\033[0m"
#define RED      "\033[1;38;5;160m"
#define COMPLETE "\033[1;38;5;2m"
#define ONGOING  "\033[0;38;5;220m"
#define VERSION  "0.0.21"
void cmd_write_file(const char* filename, const char* content) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Cannot open file %s to write.\n", filename);
        return;
    }
    fprintf(file, "%s", content);
    fclose(file);
}
int count_lines(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) return -1;
    int count = 0, ch;
    while ((ch = fgetc(fp)) != EOF) { if (ch == '\n') count++; }
    fclose(fp);
    return count;
}
char* ct[CT_COUNT];
void ctc(int i) { ct[i][0] = '\0'; }
FILE* f;
static void add(int t, char* text) { fprintf(f, "%s%s\n", t ? "\t" : "", text); }
int main(int argc, char* argv[]) {
    init_memory();
    char* targetFile;
    char* outFile;
    if (argc <= 1) { targetFile = "program.qa"; outFile = "outQAsm.asm"; } else if (argc == 2) { targetFile = argv[1]; outFile = "outQAsm.asm"; } else { targetFile = argv[1]; outFile = argv[2]; }
    FILE* file = fopen(targetFile, "r");
    if (file == NULL && is(targetFile, "program.qa")) {
        cmd_write_file("program.qa", "data\n\ttext text \"Hello, World!\", 10\nend\n\nsm\n\tprt text\nem");
        file = fopen("program.qa", "r");
    }
    if (file == NULL) { printf(RED"Error: Cannot open or create file '%s'!\n"RST, targetFile); return 1; }
    for (int i = 0; i < CT_COUNT; i++) { ct[i] = (char*)kmalloc(MAX_LINE_SIZE); }
    f = fopen(outFile, "w");
    setF(f);
    if (f == NULL) { printf(RED"Error: Cannot create file '%s' !\n"RST, outFile); return 1; }
    int cnt = 0, maxCnt = count_lines(targetFile);
    add(0, "; = > QAsm v. "VERSION);
    while (fgets(ct[0], MAX_LINE_SIZE, file) != NULL) {
        cnt++;
        printf(ONGOING"Compiling...  (%d/%d)\r"RST, cnt, maxCnt);
        if (ct[0][0] == '\n' || ct[0][0] == '\0') continue;
        runLine(ct[0]);
    }
    printf(COMPLETE"Compiled!     (%d/%d)\e[0m\n"RST, cnt, maxCnt);
    fclose(file);
    fclose(f);
    for (int i = 0; i < CT_COUNT; i++) { kfree(ct[i]); }
    return 0;
}
static int dataLoaded = 0, dataCollecting = 0, lb = 0;
void runLine(char* line) {
    ctc(3); ctc(4); ctc(5); ctc(6); ctc(7);
    trimStart(line, ' ', ct[4]);
    trimStart(ct[4], '\t', ct[3]);
    if (ct[3][0] == ']') { return; }
    int l = len(ct[3]) - 1;
    if (l >= 0 && ct[3][l] == '\n') { ct[3][l] = '\0'; }
    if (ct[3][0] == '\0' || ct[3][0] == '\r') { return; }
    ctc(4);
    splitStart(ct[3], ' ', ct[4], ct[7]);

    if (dataLoaded) { for (int i = 0; i < cmd_count; i++) { if (is(cmds[i].cmd, ct[4])) { cmds[i].data(ct[7]); return; } } } else {
        if (ct[4][0] == '.') { fprintf(f, "%s%s\n", ct[4][1] == 't' ? "\t" : "", ct[7]); lb = 1; return; }
        if (startWith(ct[3], "data")) {
            dataCollecting = 1;
            if (lb) add(0, "; = > QAsm v. "VERSION);
            add(0, "default rel");
            add(0, "; = > MACROS");
            add(0, "%macro RT 2");
            add(1, "mov eax, %2");
            add(1, "mov [rel qai%1], eax");
            add(0, "%endmacro");
            add(0, "; = > END MACROS");
            add(0, "section .data");
            add(1, "; QAsm data:");
            add(1, "qaia dd 0 ; QAsm int A");
            add(1, "qaib dd 0 ; QAsm int B");
            add(1, "; User data:");
            return;
        }
        if (dataCollecting && startWith(ct[3], "end")) {
            dataLoaded = 1;
            dataCollecting = 0;
            add(0, "section .bss");
            add(1, "itt_bfr resb 21");
            add(1, "prtbh_bfr resb 64");
            add(0, "section .text");
            add(1, "global _start");
            add(0, "; = = = INT TO TEXT = = =");
            add(0, "; RT a, [value]");
            add(0, "; call intToText");
            add(0, "; call prt");
            add(0, "intToText:");
            add(1, "mov rcx, 0");
            add(1, "mov ebx, 10");
            add(1, "xor rdx, rdx");
            add(0, ".ittLoop:");
            add(1, "mov edx, 0");
            add(1, "div ebx");
            add(1, "add edx, 48");
            add(1, "push rdx");
            add(1, "inc rcx");
            add(1, "cmp eax, 0");
            add(1, "jne .ittLoop");
            add(1, "lea rdi, [rel itt_bfr]");
            add(1, "mov rdx, rcx");
            add(1, "lea rsi, [rel itt_bfr]");
            add(0, ".ittLoopWrite:");
            add(1, "pop rax");
            add(1, "mov [rdi], al");
            add(1, "inc rdi");
            add(1, "loop .ittLoopWrite");
            add(1, "inc rdx");
            add(1, "ret");
            add(0, "; = = = PRINT = = =");
            add(0, "; mov rsi, [text]");
            add(0, "; mov rdx, [text length]");
            add(0, "; call prt");
            add(0, "prt:");
            add(1, "mov rax, 1");
            add(1, "mov rdi, 1");
            add(1, "syscall");
            add(1, "ret");
            add(0, "prtln:");
            add(1, "mov rsi, 10");
            add(1, "mov rdx, 1");
            add(1, "call prt");
            add(1, "ret");
            add(0, "; = = = PRINT BINARY = = =");
            add(1, "; RT a, [value]");
            add(1, "; RT b, [length]");
            add(1, "; call prtB");
            add(0, "prtB:");
            add(1, "mov eax, [rel qaia]");
            add(1, "lea rdi, [rel prtbh_bfr]");
            add(1, "mov rcx, 32");
            add(1, "sub rcx, [rel qaib]");
            add(1, "shl eax, cl");
            add(1, "mov rcx, [rel qaib]");
            add(1, "lea rdi, [rel prtbh_bfr]");
            add(0, ".prtBloop:");
            add(1, "mov edx, eax");
            add(1, "shr edx, 31");
            add(1, "add dl, '0'");
            add(1, "mov [rdi], dl");
            add(1, "inc rdi");
            add(1, "shl eax, 1");
            add(1, "loop .prtBloop");
            add(1, "mov rsi, prtbh_bfr");
            add(1, "mov rdx, [rel qaib]");
            add(1, "call prt");
            add(1, "ret");
            add(0, "; = = = PRINT HEX = = =");
            add(1, "; RT a, [value]");
            add(1, "; RT b, [length]");
            add(1, "; call prtH");
            add(0, "prtH:");
            add(1, "mov eax, [rel qaia]");
            add(1, "lea rdi, [rel prtbh_bfr]");
            add(1, "mov rcx, 8");
            add(1, "sub rcx, [rel qaib]");
            add(1, "shl rcx, 2");
            add(1, "shl eax, cl");
            add(1, "mov rcx, [rel qaib]");
            add(0, ".prtHloop:");
            add(1, "rol eax, 4");
            add(1, "mov edx, eax");
            add(1, "and edx, 0xF");
            add(1, "cmp dl, 9");
            add(1, "jbe .prtHdigit");
            add(1, "add dl, 7");
            add(0, ".prtHdigit:");
            add(1, "add dl, '0'");
            add(1, "mov [rdi], dl");
            add(1, "inc rdi");
            add(1, "loop .prtHloop");
            add(1, "mov rsi, [rel prtbh_bfr]");
            add(1, "mov rdx, [rel qaib]");
            add(1, "call prt");
            add(1, "ret");
            return;
        }
        if (dataCollecting && !dataLoaded) {
            splitStart(ct[7], ' ', ct[5], ct[6]);
            if (is(ct[4], "int")) { fprintf(f, "\t%s dd %s\n", ct[5], ct[6]); return; }
            if (is(ct[4], "text")) { fprintf(f, "\t%s db %s\n\tlen_%s equ $ - %s\n", ct[5], ct[6], ct[5], ct[5]); return; }
        }
    }
}
