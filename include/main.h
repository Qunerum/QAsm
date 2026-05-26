#ifndef MAIN_H
#define MAIN_H

#define MAX_LINE_SIZE 1024
#define MAX_BLOCK_SIZE 512
#define MAX_FUNCTIONS 256
#define MAX_VARIABLES 256
#define MAX_LISTS 256
#define MAX_LIST_LEN 512
#define UNKNOWN "NULL"

#define INT 1
#define STRING 2

// 0 - Line buffer
// 1 - Command
// 2 - Arguments
// 3 - runCmd line
// 4 - runCmd cmd
// 5 - runCmd name
// 6 - runCmd value
// 7 - tmp
// 8 - tmp2
#define CT_COUNT 9 // 0 1 2 3 4 5 6 7 8

extern char* ct[CT_COUNT];
void ctc(int i);

typedef struct {
    char* name;
    char* value;
    int type;
} qcVar;
typedef struct {
    char* name;
    int count;
    char* values[MAX_LIST_LEN];
    int type;
} qcList;
typedef struct {
    char* name;
    int currentLines;
    char* data[MAX_BLOCK_SIZE + 1];
} qcFunc;

void cmd_write_file(const char* filename, const char* content);
void runLine(char* line);

#endif
