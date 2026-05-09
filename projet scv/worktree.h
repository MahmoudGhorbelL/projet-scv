#ifndef WORKTREE_H
#define WORKTREE_H

#define MAXSIZE 100

typedef struct {
    char* name;
    char* hash;
    int mode;
} WorkFile;

typedef struct {
    WorkFile* tab;
    int size;
    int n;
} WorkTree;

// WorkFile
WorkFile* createWorkFile(char* name);
char* wfts(WorkFile* wf);
WorkFile* stwf(char* ch);

// WorkTree
WorkTree* initWorkTree();
int inWorkTree(WorkTree* wt, char* name);
int appendWorkTree(WorkTree* wt, char* name, char* hash, int mode);
char* wtts(WorkTree* wt);
WorkTree* stwt(char* ch);
int wttf(WorkTree* wt, char* file);
WorkTree* ftwt(char* file);

// blob
char* blobWorkTree(WorkTree* wt);
char* saveWorkTree(WorkTree* wt, char* path);
void restoreWorkTree(WorkTree* wt, char* path);

#endif
#include <sys/stat.h>

int getChmod(const char* path);
void setMode(int mode, char* path);
int isDirectory(char* path);