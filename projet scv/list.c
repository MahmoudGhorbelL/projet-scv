#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

List* initList() {
    List* L = (List*) malloc(sizeof(List));
    *L = NULL;
    return L;
}
Cell* buildCell(char* ch) {
    Cell* c = (Cell*) malloc(sizeof(Cell));
    c->data = ch;
    c->next = NULL;
    return c;
}
void insertFirst(List* L, Cell* C) {
    C->next = *L;
    *L = C;
}
char* ctos(Cell* c) {
    if (c == NULL) return NULL;
    return c->data;
}
char* ltos(List* L) {
    if (L == NULL || *L == NULL)
        return NULL;
    int size = 0;
    Cell* tmp = *L;
    while (tmp) {
        char* p = tmp->data;
        while (*p)
            size++, p++;
        if (tmp->next)
            size++;
        tmp = tmp->next;
    }
    char* str = malloc(size + 1);
    char* w = str;
    tmp = *L;
    while (tmp) {
        char* p = tmp->data;
        while (*p)
            *w++ = *p++;
        if (tmp->next)
            *w++ = '|';
        tmp = tmp->next;
    }
    *w = '\0';
    return str;
}
/* Q2.5 */
Cell* listGet(List* L, int i) {
    if (L == NULL) return NULL;
    Cell* tmp = *L;
    int j = 0;
    while (tmp != NULL && j < i) {
        tmp = tmp->next;
        j++;
    }
    return tmp;
}
/* Q2.6 */
Cell* searchList(List* L, char* str) {
    if (L == NULL) return NULL;
    Cell* tmp = *L;
    while (tmp != NULL) {
        if (strcmp(tmp->data, str) == 0)
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

/* Q2.7 */
List* stol(char* s) {
    List* L = initList();
    char* word = NULL;
    int len = 0;
    for (int i = 0; ; i++) {
        if (s[i] != '|' && s[i] != '\0') {
            char* tmp = realloc(word, len + 2); 
            if (!tmp) return L;
            word = tmp;
            word[len++] = s[i];
        }
        else {
            if (word != NULL) {
                word[len] = '\0';
                Cell* c = buildCell(word);
                c->data = word; 
                insertFirst(L, c);
                word = NULL;
                len = 0;
            }
            if (s[i] == '\0')
                break;
        }
    }
    return L;
}
/* Q2.8 */
void ltof(List* L, char* path) {
    FILE* f = fopen(path, "w");
    if (f == NULL) return;
    char* str = ltos(L);
    if (str != NULL) {
        fprintf(f, "%s", str);
        free(str);
    }
    fclose(f);
}

List* ftol(char* path) {
    FILE* f = fopen(path, "r");
    char s[10000];
    fscanf(f, "%s", s);
    fclose(f);
    return stol(s);
}
