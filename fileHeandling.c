#include "fileHeandling.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

// -1 nu e ok 1 facut deja 0 ok !!!!!!! nu uita


// dacă un director există la calea specificată.
int dirExists(char *path) {
   // m am uitat man 2 stat() unde e un exemplu de cod si foloseste asa 
   struct stat sb;
   return (stat(path, &sb) == 0 );
}

//  dacă un fisierul exista.
int fileExists(char *path) {
  //stat() prezentat in lab 
  struct stat sb;
  return (stat(path, &sb) == 0 );
}

// creează un director dacă acesta nu există deja.
int createDir(const char *path)
{   
    //verific sa nu existe 
    if(dirExists((char *)path))
        return 1;   

    if(mkdir(path, 0750) == 0)
        return 0;   

    return -1;     
}

//basically ce faceam si inainte doar ca intr-o functie
char* buildDirPath(char *dirname) {
    char *path = malloc(1024*sizeof(char));
    if (path == NULL) return NULL;

    //imi da calea catre locul inde ma aflu
    //o sa am calea user/home/proeictSo
    if (getcwd(path, 1024) == NULL) {
        free(path);
        return NULL;
    }
    //pun numele directorului district unde sa mi puna fisierele
    // o sa am //o sa am calea user/home/proeictSo/district
    strcat(path, "/");
    strcat(path, dirname);
    return path;
}

//imi creaza calea pt fisier
char* buildFilePath(char *dirPath, char *fileName)
{
    char *path = malloc(1024);

    if(path == NULL)
        return NULL;
    // o sa am calea user/home/proeictSo/district/reports.dat de ex
    strcpy(path, dirPath);
    strcat(path, "/");
    strcat(path, fileName);

    return path;
}

// imi creaza un fisier si de scriere si de citire
int createFile(char *dirPath, char *fileName) {

    char *path=buildFilePath(dirPath,fileName);

    if (fileExists(path)) {
        return 1;
    }

    FILE *f = fopen(path, "w+");
    if (f == NULL) {
        return -1;
    }
    
    fclose(f);
    return 0;
}