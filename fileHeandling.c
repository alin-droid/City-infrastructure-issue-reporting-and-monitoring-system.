#include "fileHeandling.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include<time.h>
#include "permissions.h"

#define SIZE_NAME 10000
#define SIZE_CATEGORY_ISSUE 100
#define SIZE_LENGTH_DESCRIPTION 100000
// -1 nu e ok 1 facut deja 0 ok !!!!!!! nu uita

struct content{
   int reportID;
   char inspectorName[SIZE_NAME];
   float latitude,longitude;
   char issue[SIZE_CATEGORY_ISSUE];
   time_t time;
   char description[SIZE_LENGTH_DESCRIPTION];
};


// dacă un director există la calea specificată.
int dirExists(char *path)
{
   // m am uitat man 2 stat() unde e un exemplu de cod si foloseste asa
   struct stat sb;
   return (stat(path, &sb) == 0);
}


// dacă un fisierul exista.
int fileExists(char *path)
{
   // stat() prezentat in lab
   struct stat sb;
   return (stat(path, &sb) == 0);
}


// creează un director dacă acesta nu există deja.
int createDir(const char *path)
{
   // verific sa nu existe
   if(dirExists((char *)path))
      return 1;

   if(mkdir(path, 0750) == 0)
      return 0;

   return -1;
}


// basically ce faceam si inainte doar ca intr-o functie
char *findDirPath(char *dirname)
{
   char *path = malloc(1024 * sizeof(char));

   if(path == NULL)
      return NULL;

   // imi da calea catre locul inde ma aflu
   // o sa am calea user/home/proeictSo
   if(getcwd(path, 1024) == NULL)
   {
      free(path);
      return NULL;
   }

   // pun numele directorului district unde sa mi puna fisierele
   // o sa am calea user/home/proeictSo/district
   strcat(path, "/");
   strcat(path, dirname);

   return path;
}


// imi creaza calea pt fisier
char *findFilePath(char *dirPath, char *fileName)
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
int createFile(char *dirPath, char *fileName)
{
   char *path = findFilePath(dirPath, fileName);

   if(path == NULL)
      return -1;

   if(fileExists(path))
   {
      free(path);
      return 1;
   }

   // creez fisierul daca nu exista fara permisiuni pt ca le setez mai tarziu
   int fd = open(path, O_CREAT | O_RDWR, 0000);

   if(fd==-1)
   {
      free(path);
      return -1;
   }

   close(fd);
   free(path);

   return 0;
}


int createFileWithPermission(char *dirPath, char *fileName, mode_t perm)
{  
    //verific daca s-a creat

    if( createFile(dirPath, fileName) == -1)
        return -1;

    char *filePath = findFilePath(dirPath, fileName);

    if(filePath == NULL)
        return -1;

    if(chmod(filePath, perm) != 0)
    {
        printf("cannot change %s permission!\n", fileName);
        free(filePath);
        return -1;
    }

    free(filePath);
    return 0;
}
   //modific cu permisiunea specifica


 ReportContent_t  *createContent(int reportID,char *inspectorName,float latitude,float longitude,char *issue,char *description){

    ReportContent_t *content=malloc(sizeof(ReportContent_t));
    if(content==NULL){
      fprintf(stderr,"memory cannot be alocated!\n");
      exit(-1);
    }
    content->reportID=reportID;
    strcpy(content->inspectorName,inspectorName);
    content->latitude=latitude;
    content->longitude=longitude;
    strcpy(content->issue,issue);
    content->time=time(NULL);
    strcpy(content->description,description);
    return content;
}

//char *inspectorName,float latitude,float longitude,char *issue,char *description

int addNewReport(Role_t role, ReportContent_t *content, char *dirPath, char *fileName)
{
    if(content == NULL)
        return -1;

    char *filePath = findFilePath(dirPath, fileName);

    if(filePath == NULL)
        return -1;

    checkPermissions(role, filePath, fileName);

    FILE *f = fopen(filePath, "ab");

    if(f == NULL)
    {
        perror("fopen");
        free(filePath);
        return -1;
    }

    fwrite(&content->reportID, sizeof(int), 1, f);

    int len = strlen(content->inspectorName);
    fwrite(&len, sizeof(int), 1, f);
    fwrite(content->inspectorName, sizeof(char), len+1, f);

    fwrite(&content->latitude, sizeof(float), 1, f);
    fwrite(&content->longitude, sizeof(float), 1, f);

    len = strlen(content->issue);
    fwrite(&len, sizeof(int), 1, f);
    fwrite(content->issue, sizeof(char), len+1, f);

    fwrite(&content->time, sizeof(time_t), 1, f);

    len = strlen(content->description);
    fwrite(&len, sizeof(int), 1, f);
    fwrite(content->description, sizeof(char), len+1, f);

    fclose(f);
    free(filePath);

    return 0;
}