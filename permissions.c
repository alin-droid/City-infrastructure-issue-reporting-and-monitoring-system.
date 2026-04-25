
#include "processArguments.h"
#include "fileHeandling.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>


void checkPermissions(Role_t role,char *filePath){
    
    //strrchr imi returneaza pointer la ultima aparitei la caracterul acela
    char *fileName=strrchr(filePath, '/');
    //pentru a scapa de /
    fileName=fileName+1;
    struct stat sb;
    //verific accesul la fisier
    if(stat(filePath, &sb) != 0)
    {
        printf("Cannot access file %s\n", fileName);
        exit(1);
    }
    //in functie de rol verific permisiunile
    switch (role){
                  //verific permisiunile pt fiecare timp de fisier cu ajutorul codurilor din man  ( manager e owner si inspector e group)
    case manager: if(strcmp(fileName,"reports.dat")==0 || strcmp(fileName,"district.cfg")==0 || strcmp(fileName,"logged_district")==0){
                    if (!(sb.st_mode & S_IRUSR)) {
                    printf("Manager has no read permission for %s\n", fileName);
                    exit(1);
                    }

                    if (!(sb.st_mode & S_IWUSR)) {
                    printf("Manager has no read permission for %s\n", fileName);
                    exit(1); 
                    }
                }
                break;

    case inspector: if(strcmp(fileName,"reports.dat")==0){
                    if (!(sb.st_mode & S_IRGRP)) {
                    printf("Inspector has no read permission for %s\n", fileName);
                    exit(1);
                    }

                    if (!(sb.st_mode &  S_IWGRP)) {
                    printf("Inspector has no read permission for %s\n", fileName);
                    exit(1); 
                    }
                }

                  if(strcmp(fileName,"district.cfg")==0|| strcmp(fileName,"logged_district")==0 ){
                    if (!(sb.st_mode & S_IRGRP)) {
                     printf("Inspector has no read permission for %s\n", fileName);
                    exit(1);
                    }
                } 
                break;
       default: break; 
    }
}


void printPermissionsForFile(char *filePath,char *fileName){
    struct stat sb;
    if(stat(filePath, &sb) != 0)
    {
        printf("Cannot print permissions!\n");
        exit(-1);
    }
    
    //  owner 
    if(sb.st_mode & S_IRUSR){
        printf("r");
    }
    else{
         printf("-");
    }

    if(sb.st_mode & S_IWUSR){
        printf("w");
    }
    else{
         printf("-");
    }

    if(sb.st_mode & S_IXUSR){
        printf("x");
    }
    else{
         printf("-");
    }
    
    // grop

    if(sb.st_mode & S_IRGRP){
        printf("r");
    }
    else{
         printf("-");
    }

    if(sb.st_mode & S_IWGRP ){
        printf("w");
    }
    else{
         printf("-");
    }

    if(sb.st_mode & S_IXGRP){
        printf("x");
    }
    else{
         printf("-");
    }

    //other

    if(sb.st_mode & S_IROTH ){
        printf("r");
    }
    else{
         printf("-");
    }

    if(sb.st_mode & S_IWOTH ){
        printf("w");
    }
    else{
         printf("-");
    }

    if(sb.st_mode & S_IWOTH){
        printf("x");
    }
    else{
         printf("-");
    }
    char *time=ctime(&sb.st_mtime);
    time[strlen(time)-1]='\0';
    printf("       %ld bytes  %s  %s \n", sb.st_size,time,fileName);
}

