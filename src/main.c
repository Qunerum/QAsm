#include <stdio.h>
#include "../include/main.h"
#include "../include/memory.h"
#include "../include/utility.h"

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
    while (fgets(ct[0], MAX_LINE_SIZE, file) != NULL) { if (ct[0][0] == '\n' || ct[0][0] == '\0') continue; /* run */ }
    fclose(file);
    for (int i = 0; i < CT_COUNT; i++) { kfree(ct[i]); }
    return 0;
}
void runLine(char* line) {

}

int detectType(char* str) {
    int i = 0;
    int dotCount = 0;
    int digitCount = 0;
    if (str[0] == '-') i++;
    while (str[i] != '\0') {
        if (str[i] == '.') { dotCount++; }
        else if (str[i] >= '0' && str[i] <= '9') { digitCount++; }
        else { return STRING; }
        i++;
    }
    if (digitCount > 0 && dotCount == 0) return INT;
    if (digitCount > 0 && dotCount == 1) return FLOAT;
    return STRING;
}
