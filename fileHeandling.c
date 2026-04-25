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




//FUNCTIILE PT REPORTS.DAT
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

//pt ca am vzt ca folosesc des afisarea asta mi am facut o fucntie
void printReportContent(ReportContent_t raport){
     printf("ID: %d ", raport.reportID);
     printf("Inspector: %s ", raport.inspectorName);
     printf("Issue: %s ", raport.issue);
     printf("Coords: %.2f %.2f\n", raport.latitude, raport.longitude);
}

//char *inspectorName,float latitude,float longitude,char *issue,char *description

int addNewReport(Role_t role, ReportContent_t *content, char *dirPath, char *fileName)
{   
   //creez fisierul si fac check la permisiuni
    if(content == NULL)
        return -1;

    char *filePath = findFilePath(dirPath, fileName);
   
    if(filePath == NULL)
        return -1;
 
    checkPermissions(role, filePath);
    //daca am permisiunea de a scrie in fisier
    FILE *f = fopen(filePath, "ab");

    if(f == NULL)
    {
        perror("fopen");
        free(filePath);
        return -1;
    }
    
    //scriu in fisierul binar
    //note to self dai string nume_fisier ca sa vezi ca e adaugat ce trebuie 
    fwrite(content, sizeof(ReportContent_t), 1, f);

    fclose(f);
    free(filePath);

    return 0;
}


void printReports(Role_t role,char *filePath)
{    
   //verific daca am perimisunea de a citi din fisier
    checkPermissions(role,filePath);

    FILE *f = fopen(filePath, "rb");

    if (f == NULL)
    {
        perror("Cannot open file");
        return;
    }
   
    //strucuta in care voi citi contentul actual
    struct content raport;
    size_t bytesRead;
    int found = 0;

    rewind(f);
    
    //contentul trebuie pus intr-un fisier binar deci e important sa aiba numar fix de octeti alocati
    //se vede si in diagrama din documentatie ca altfel se poate duce la alte eroare (MAI ALES LA LIST :) )

    //dupa ce citesc din fisierul reports.dat afisez in consola
    while ((bytesRead = fread(&raport, 1, sizeof(struct content), f)) > 0)
    {
        if (bytesRead < sizeof(struct content))
        {
            break;   
        }

        found = 1;

        printReportContent(raport);
    }

    if (!found)
    {
        printf("No reports found.\n");
    }

    fclose(f);
}

//aici am avut batai de cap si pot spune ca implementarea cu encapsulare poate nu e the best cand vine vorba de a scris cod rapd:) 
//+ era bine sa mi fac o arhitectura de la inceput ca sa stiu clar anumite functii noroc ciu erorile nenumarate

//a trebuit sa o fac aici ca sa am aces la campul reportId 
//acum ma gandesc ca puteam sa si un getter dar totusi cred ca isi are locul mai degraba aici

int reportIdExists(char *filePath, int searchedID)
{
    FILE *f = fopen(filePath, "rb");

    if(f == NULL)
        return 0;

    ReportContent_t raport;
   
    while(fread(&raport, sizeof(ReportContent_t), 1, f) == 1)
    {
        if(raport.reportID == searchedID)
        {
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    return 0;
}

void printRaport(Role_t role,char *filePath,int id){
   
    //verific daca ma permisiunea de a citi din fisier
    checkPermissions(role,filePath);

     //daca estisa il afisez
     if(reportIdExists(filePath,id)==1){

        FILE *f = fopen(filePath, "rb");
        if(f == NULL){
           printf("the reports.dat file cannot be opened!\n");
           return;
         }
   
      ReportContent_t raport;

       while(fread(&raport, sizeof(ReportContent_t), 1, f) == 1){

            if(raport.reportID == id){
              
               printReportContent(raport);
                
             fclose(f);
             return;
           }
        }
      fclose(f);
     }
     //daca nu afisez un mesaj

     printf("please enter a new id because ID=%d cannot be founded",id);
}

//FUNCTII PT CONFIG

void addThresholdInConfig(Role_t role,char *filePath, char *thresholdValue)
{   
   //verific daca am permisiunea de a scrie in config

    checkPermissions(role,filePath);

    FILE *f = fopen(filePath, "r");

    if(f != NULL)
    {   //pt a nu resvrie thresholValue de fiecare data 
        fseek(f, 0, SEEK_END);

        if(ftell(f) > 0)
        {
            fclose(f);
            return;
        }

        fclose(f);
    }

    f = fopen(filePath, "w");

    if(f == NULL)
        return;

    fprintf(f, "severity_threshold=%s\n", thresholdValue);

    fclose(f);
}

//FUNCTII PT LOG

//adaug in log la fiecare operatie 
void addLogInDistrict(char *filePath, Role_t role, char *userName, char *actionName)
{  
   //verific daca am permisiunea de a scrie
   checkPermissions(role,filePath);

    FILE *f = fopen(filePath, "a");

    if(f == NULL){
        return;
    }

    char *roleName;

    if(role == inspector){
        roleName = "inspector";
    }
    else{
        roleName = "manager";
    }

    time_t timpCurent = time(NULL);

    char *data = ctime(&timpCurent);
    
    data[strlen(data) - 1] = '\0';

    fprintf(f, "%s, %s, %s, %s\n",data,roleName,userName,actionName);

    fclose(f);
}