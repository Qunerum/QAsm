#include <stdio.h>
#include "../include/utility.h"

int isAny(char* a, char* b[]) {
    int i = 0;
    while (b[i] != NULL) { if (is(a, b[i])) return 1; i++; }
    return 0;
}
void splitStart(char* in, char delimiter, char* outA, char* outB) {
    int i = 0, ia = 0, ib = 0, found = 0;
    if (outA != NULL) outA[0] = '\0';
    if (outB != NULL) outB[0] = '\0';
    if (in == 0 || in[0] == '\0') return;
    while(in[i] != '\0') {
            if (in[i] == delimiter && !found) found = 1;
            else if (!found) { outA[ia] = in[i]; ia++; }
            else if (found) { outB[ib] = in[i]; ib++; }
            i++;
    }
    outA[ia] = '\0'; outB[ib] = '\0';
}
void copyStr(char* target, char* source) { int i = 0; while (source[i] != '\0') { target[i] = source[i]; i++; } target[i] = '\0'; }
void cutStart(char* t, int c, char* out) {
    int i = 0;
    if (c >= len(t)) { out[0] = '\0'; return; }
    while (t[c] != '\0') { out[i] = t[c]; i++; c++; }
    out[i] = '\0';
}
void trimStart(char* t, char c, char* out) {
    int i = 0;
    while (t[i] == c) { i++; }
    cutStart(t, i, out);
}
int contains(char* str, char* search) {
    int i = 0, j = 0;
    while (str[i] != '\0') {
        if (str[i] == search[j]) {
            int temp = i;
            while (str[temp] == search[j] && search[j] != '\0') { temp++; j++; }
            if (search[j] == '\0') return 1;
            j = 0;
        }
        i++;
    }
    return 0;
}
void replace(char* t, char* w, char* r, char* out) {
    int i = 0, j = 0, t_len = len(t), w_len = len(w), r_len = len(r);
    while (i < t_len) {
        int match = 1;
        for (int m = 0; m < w_len; m++) { if (t[i + m] != w[m]) { match = 0; break; } }
        if (match) { for (int m = 0; m < r_len; m++) { out[j++] = r[m]; } i += w_len; } else { out[j++] = t[i++]; }
    }
    out[j] = '\0';
}
