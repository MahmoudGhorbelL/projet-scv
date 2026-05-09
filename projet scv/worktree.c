#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "worktree.h"
#include "files.h"
#include "list.h"

char* sha256file(char* file);

// getChmod : récupère les permissions d'un fichier
int getChmod(const char* path) {
    struct stat ret;
    if (stat(path, &ret) == -1) return -1;
    return (ret.st_mode & S_IRUSR) | (ret.st_mode & S_IWUSR) | (ret.st_mode & S_IXUSR) |
           (ret.st_mode & S_IRGRP) | (ret.st_mode & S_IWGRP) | (ret.st_mode & S_IXGRP) |
           (ret.st_mode & S_IROTH) | (ret.st_mode & S_IWOTH) | (ret.st_mode & S_IXOTH);
}

// setMode : modifie les permissions d'un fichier
void setMode(int mode, char* path) {
    char buff[100];
    sprintf(buff, "chmod %o %s", mode, path);
    system(buff);
}

// isDirectory : vérifie si c'est un dossier
int isDirectory(char* path) {
    struct stat s;
    if (stat(path, &s) == 0)
        return S_ISDIR(s.st_mode);
    return 0;
}

// Q-4.1 : créer un WorkFile
WorkFile* createWorkFile(char* name) {
    WorkFile* wf = malloc(sizeof(WorkFile));
    wf->name = malloc(strlen(name) + 1);
    strcpy(wf->name, name);
    wf->hash = NULL;
    wf->mode = 0;
    return wf;
}

// Q-4.2 : WorkFile → chaîne "name\thash\tmode"
char* wfts(WorkFile* wf) {
    char* str = malloc(1000);
    sprintf(str, "%s\t%s\t%d",
        wf->name,
        wf->hash ? wf->hash : "NULL",
        wf->mode);
    return str;
}

// Q-4.3 : chaîne → WorkFile
WorkFile* stwf(char* ch)
{
    WorkFile* wf = (WorkFile*) malloc(sizeof(WorkFile));

    if (wf == NULL)
        return NULL;

    char* name = strtok(ch, "\t");
    char* hash = strtok(NULL, "\t");
    char* mode = strtok(NULL, "\t");

    wf->name = name;
    wf->hash = hash;
    wf->mode = mode ? atoi(mode) : 0;

    return wf;
}
// Q-4.4 : initialiser un WorkTree
WorkTree* initWorkTree() {
    WorkTree* wt = malloc(sizeof(WorkTree));
    wt->tab = malloc(sizeof(WorkFile) * MAXSIZE);
    wt->size = MAXSIZE;
    wt->n = 0;
    return wt;
}

// Q-4.5 : cherche un fichier dans le WorkTree
int inWorkTree(WorkTree* wt, char* name) {
    for (int i = 0; i < wt->n; i++) {
        if (strcmp(wt->tab[i].name, name) == 0)
            return i;
    }
    return -1;
}

// Q-4.6 : ajoute un fichier au WorkTree
int appendWorkTree(WorkTree* wt, char* name, char* hash, int mode)
{
    if (wt == NULL)
        return -1;
    /* Vérifier si déjà présent */
    for (int i = 0; i < wt->n; i++)
    {
        if (strcmp(wt->tab[i].name, name) == 0)
            return 0;   // déjà existe
    }
    /* Ajouter nouveau fichier */
    if (wt->n >= wt->size)
        return -1;   // tableau plein
    wt->tab[wt->n].name = name;
    wt->tab[wt->n].hash = hash;
    wt->tab[wt->n].mode = mode;
    wt->n++;
    return 1;
}

// Q-4.7 : WorkTree → chaîne
char* wtts(WorkTree* wt) {
    char* str = malloc(10000);
    str[0] = '\0';
    for (int i = 0; i < wt->n; i++) {
        strcat(str, wfts(&wt->tab[i]));// ajoute la représentation du WorkFile
        strcat(str, "\n");// ajoute un saut de ligne
    }
    return str;
}

// Q-4.8 : chaîne → WorkTree
WorkTree* stwt(char* ch) {
    WorkTree* wt = initWorkTree();
    char* copy = malloc(strlen(ch) + 1);
    strcpy(copy, ch);

    char* line = strtok(copy, "\r\n");// divise la chaîne en lignes
    while (line != NULL) {
        if (strlen(line) > 1) {
            WorkFile* wf = stwf(line);// convertit la ligne en WorkFile
            appendWorkTree(wt, wf->name, wf->hash, wf->mode);// ajoute le WorkFile au WorkTree
        }
        line = strtok(NULL, "\r\n");// passe à la ligne suivante
    }
    free(copy);
    return wt;
}

// Q-4.9 : WorkTree → fichier
int wttf(WorkTree* wt, char* file) {
    FILE* f = fopen(file, "w");
    if (!f) return -1;
    char* str = wtts(wt);
    fprintf(f, "%s", str);
    fclose(f);
    free(str);
    return 0;
}

// Q-4.10 : fichier → WorkTree
WorkTree* ftwt(char* file) {
    FILE* f = fopen(file, "r");
    if (!f) return NULL;

    WorkTree* wt = initWorkTree();
    char line[1000];

    while (fgets(line, sizeof(line), f)) {
        for (int i = 0; i < strlen(line); i++) {
            if (line[i] == '\r' || line[i] == '\n')
                line[i] = '\0';
        }
        if (strlen(line) > 1) {
            WorkFile* wf = stwf(line);
            appendWorkTree(wt, wf->name, wf->hash, wf->mode);
        }
    }
    fclose(f);
    return wt;
}

// Q-5.1 : sauvegarde le WorkTree dans un blob
char* blobWorkTree(WorkTree* wt) {
    char fname[] = "tmp_wt.tmp";
    wttf(wt, fname);

    char* hash = sha256file(fname);
    char* path = hashToPath(hash);
    char* pathT = malloc(strlen(path) + 3);
    sprintf(pathT, "%s.t", path);

    char dir[3];
    dir[0] = hash[0];
    dir[1] = hash[1];
    dir[2] = '\0';
    char cmd[100];
    sprintf(cmd, "mkdir %s 2>nul", dir);
    system(cmd);

    cp(pathT, fname);
    remove(fname);

    free(path);
    free(pathT);
    return hash;
}

// Q-5.2 : sauvegarde récursive du WorkTree
char* saveWorkTree(WorkTree* wt, char* path) {
    for (int i = 0; i < wt->n; i++) {
        char fullpath[1000];
        sprintf(fullpath, "%s/%s", path, wt->tab[i].name);

        if (isDirectory(fullpath)) {
            List* L = listdir(fullpath);
            WorkTree* newWT = initWorkTree();

            Cell* current = *L;
            while (current != NULL) {
                appendWorkTree(newWT, current->data, NULL, 0);
                current = current->next;
            }

            char* h = saveWorkTree(newWT, fullpath);
            wt->tab[i].hash = h;
            wt->tab[i].mode = getChmod(fullpath);
        } else {
            blobFile(fullpath);
            wt->tab[i].hash = sha256file(fullpath);
            wt->tab[i].mode = getChmod(fullpath);
        }
    }
    return blobWorkTree(wt);
}

// Q-5.3 : restauration récursive du WorkTree
void restoreWorkTree(WorkTree* wt, char* path) {
    for (int i = 0; i < wt->n; i++) {
        char* hash = wt->tab[i].hash;
        if (hash == NULL) continue;

        char* blobpath = hashToPath(hash);

        char blobpathT[1000];
        sprintf(blobpathT, "%s.t", blobpath);

        char dest[1000];
        sprintf(dest, "%s/%s", path, wt->tab[i].name);

        if (file_exists(blobpathT)) {
            // c'est un dossier
            char cmd[200];
            sprintf(cmd, "mkdir %s 2>nul", dest);
            system(cmd);

            WorkTree* newWT = ftwt(blobpathT);
            restoreWorkTree(newWT, dest);
        } else {
            // c'est un fichier
            cp(dest, blobpath);
            setMode(wt->tab[i].mode, dest);
        }

        free(blobpath);
    }
}