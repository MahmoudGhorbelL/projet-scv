#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

typedef struct cell {
    char* data;
    struct cell* next;
} Cell;

typedef Cell* List;

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
    if (f == NULL) return initList();

    char buffer[1000];
    fgets(buffer, 1000, f);
    fclose(f);

    buffer[strcspn(buffer, "\n")] = '\0';

    return stol(buffer);
}

/* Q3.1 */
List* listdir(char* root_dir) {
    List* L = initList();

    DIR* dp = opendir(root_dir);
    struct dirent* ep;

    if (dp != NULL) {
        while ((ep = readdir(dp)) != NULL) {
            insertFirst(L, buildCell(ep->d_name));
        }
        closedir(dp);
    }

    return L;
}

/* Q3.2 */
int file_exists(char* file) {
    List* L = listdir(".");
    return (searchList(L, file) != NULL);
}

/* Q3.3 */
void cp(char* to, char* from) {
    if (!file_exists(from)) return;

    FILE* src = fopen(from, "r");
    FILE* dest = fopen(to, "w");

    if (src == NULL || dest == NULL) return;

    char buffer[1000];

    while (fgets(buffer, 1000, src) != NULL) {
        fputs(buffer, dest);
    }

    fclose(src);
    fclose(dest);
}

/* Q3.4 */
char* hashToPath(char* hash) {
    char* path = (char*) malloc(strlen(hash) + 2);

    path[0] = hash[0];
    path[1] = hash[1];
    path[2] = '/';

    strcpy(path + 3, hash + 2);

    return path;
}

/* Q3.5 */
void blobFile(char* file) {
    char hash[100] = "abcdef123456789"; /* exemple */

    char dir[3];
    strncpy(dir, hash, 2);
    dir[2] = '\0';

    char* path = hashToPath(hash);

    char cmd[200];
    sprintf(cmd, "mkdir %s", dir);
    system(cmd);

    cp(path, file);

    free(path);
}