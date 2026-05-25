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
#define FLOAT 2
#define STRING 3

// 0 - Line buffer
// 1 - Command
// 2 - Arguments
// 3 , 4 - For commands
#define CT_COUNT 5 // 0 1 2 3 4

extern char* ct[CT_COUNT];
void ctc(int i);


typedef struct {
    char* cmd;
    void (*handler)(char*);
} qcCmd;
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

extern char* block[MAX_BLOCK_SIZE + 1];

qcVar* getVar(char* name, int* out);
void addVar(char* name, int type, char* val);
void remVar(char* name);

qcList* getList(char* name, int* out);
void addList(char* name, int type);
void addToList(char* listName, char* value);
void setToList(char* name, int id, char* value);
void remList(char* name);
void remiList(char* name, int id);

void run_func(char* name);
int detectType(char* str);
void runLine(char* line);

char* typeToStr(int t);
void cmd_write_file(const char* filename, const char* content);

#endif
