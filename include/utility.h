#ifndef UTILITY_H
#define UTILITY_H

static inline int len(char* text) { char* start = text; while (*text) { text++; } return (int)(text - start); }
static inline int is(char* a, char* b) { while (*a != '\0' && *b != '\0') { if (*a != *b) return 0; a++; b++; } return (*a == *b); }

int isAny(char* a, char* b[]);
void splitStart(char* in, char delimiter, char* outA, char* outB);
void copyStr(char* target, char* source);
static inline int startWith(char* text, char* start) { while (*start != '\0') { if (*text == '\0' || *text != *start) return 0; text++; start++; } return 1; }
int startAny(char* a, char* b[], char* out);
void cutStart(char* t, int c, char* out);
void trimStart(char* t, char c, char* out);
int contains(char* str, char* search);
void replace(char* t, char* w, char* r, char* out);
void strToInt(char* str, int* out);
void intToStr(int n, char* out);
void strToFloat(char* str, float* out);
void floatToStr(float n, char* out);
char* addStr(char* a, char* b);

#endif
