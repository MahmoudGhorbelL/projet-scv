#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "files.h"
#include "list.h"
char* sha256file(char* file);

/* Q3.1  te9ra les fichier du dossier et les stocke dans une liste chaînée */
List* listdir(char* root_dir) {
    DIR *dp = opendir(root_dir);
    struct dirent *ep;
    List* L = initList();
    if (dp != NULL) {
        while ((ep = readdir(dp)) != NULL) {
            char* name = strdup(ep->d_name);
            insertFirst(L, buildCell(name));
        }
        closedir(dp);
    }
    return L;
}

/* Q3.2  vérifie si un fichier existe */
int file_exists(char *file) {
    DIR *dp = opendir(".");
    struct dirent *ep;
    if (dp != NULL) {
        while ((ep = readdir(dp)) != NULL) {
            if (strcmp(ep->d_name, file) == 0) {
                closedir(dp);
                return 1;
            }
        }
        closedir(dp);
    }
    return 0;
}

/* Q3.3 */
// Q-3.3 : copie un fichier vers un autre
// fix : utilise fopen directement au lieu de file_exists
void cp(char* to, char* from) {
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

// Q-3.5 : enregistre un instantané du fichier
void blobFile(char* file) {
    char* hash = sha256file(file);
    char* path = hashToPath(hash);    
    char dir[3];
    dir[0] = hash[0];
    dir[1] = hash[1];
    dir[2] = '\0';    
    char cmd[100];
    sprintf(cmd, "mkdir %s 2>nul", dir);
    system(cmd);    
    cp(path, file);
    printf("Blob cree : %s\n", path);
    free(hash);
    free(path);
}