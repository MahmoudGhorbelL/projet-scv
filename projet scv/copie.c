#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "list.h"
#include "files.h"

// Q-1.3
int hashFile(char* source, char* dest) {
    char cmd[1000];
    sprintf(cmd, "sha256sum %s > %s", source, dest);
    system(cmd);
    return 0;
}

// Q-1.4
char* sha256file(char* file) {
    char fname[] = "tmp_hash.tmp";

    char cmd[2000];
    sprintf(cmd, "sha256sum %s > %s", file, fname);
    system(cmd);

    FILE* f = fopen(fname, "r");
    char* hash = malloc(65);
    fscanf(f, "%64s", hash);
    fclose(f);

    remove(fname);
    return hash;
}

int main() {
    // test Q-1.3
    printf("-- hashFile --\n");
    hashFile("main.c", "file.tmp");
    system("cat file.tmp");

    // test Q-1.4
    printf("-- sha256file --\n");
    char* h = sha256file("main.c");
    printf("Hash : %s\n", h);
    free(h);

    // test Q-2
    printf("-- liste chainee --\n");
    List* L = initList();
    insertFirst(L, buildCell("hello"));
    insertFirst(L, buildCell("world"));
    insertFirst(L, buildCell("!"));
    printf("Liste : %s\n", ltos(L));
    Cell* c = searchList(L, "hello");
    printf("Trouve : %s\n", c ? c->data : "non trouve");
    ltof(L, "liste.txt");
    List* L2 = ftol("liste.txt");
    printf("Depuis fichier : %s\n", ltos(L2));

    // test Q-3.1
    printf("-- listdir --\n");
    List* L3 = listdir(".");
    printf("%s\n", ltos(L3));

    // test Q-3.2
    printf("-- file_exists --\n");
    printf("main.c existe : %d\n", file_exists("main.c"));
    printf("toto.c existe : %d\n", file_exists("toto.c"));

    // test Q-3.3
    printf("-- cp --\n");
    cp("copie.c", "main.c");
    printf("copie faite\n");

    // test Q-3.4
    printf("-- hashToPath --\n");
    char* p = hashToPath("6dfsq25a4fdfq");
    printf("chemin : %s\n", p);
    free(p);

    // test Q-3.5
    printf("-- blobFile --\n");
    blobFile("main.c");
// 
//     // test Q-4
//     printf("-- WorkTree --\n");
//     WorkTree* wt = initWorkTree();
//     appendWorkTree(wt, "main.c", NULL, 0);
//     appendWorkTree(wt, "list.c", NULL, 0);
//     printf("WorkTree :\n%s\n", wtts(wt));
//     wttf(wt, "worktree.txt");
//     printf("Sauvegarde faite\n");
//     WorkTree* wt2 = ftwt("worktree.txt");
//     printf("Depuis fichier :\n%s\n", wtts(wt2));

//     // test Q-5
//     printf("-- saveWorkTree --\n");
//     WorkTree* wt3 = initWorkTree();
//     appendWorkTree(wt3, "main.c", NULL, 0);
//     appendWorkTree(wt3, "list.c", NULL, 0);
//     appendWorkTree(wt3, "files.c", NULL, 0);
//     char* hash = saveWorkTree(wt3, ".");
//     printf("Hash du WorkTree : %s\n", hash);

//     printf("\n-- restoreWorkTree --\n");
//     system("mkdir restauration 2>nul");
//     restoreWorkTree(wt3, "restauration");
//     printf("Restauration terminee !\n");
//     system("ls restauration");

    return 0;
}