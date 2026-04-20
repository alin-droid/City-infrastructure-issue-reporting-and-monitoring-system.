#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processArguments.h"
#include "fileHeandling.h"

int main(int argc, char *argv[])
{
    if(validateArguments(argc, argv) == 0){
        return 1;
    }
    
    //iau numele distrctului 
    char *district = getDistrict(argc, argv);
    
    //creez calea pt folder
    char *dirPath = buildDirPath(district);
    //printf("%s",dirPath);
 
    if(dirPath == NULL){
        printf("error creating path\n");
        return 1;
    }
   
    //construiesc folder-ul 
    if(createDir(dirPath)==1){
        printf("the directory is already created!\n");
    }


    //creez fisierle pt folder
    if(createFile(dirPath,"reports.dat")<0){
        printf("cannot create reports.data!");
    }
    
     if(createFile(dirPath,"district.cfg")<0){
        printf("cannot create reports.data!");
    }

     if(createFile(dirPath,"logged_district")<0){
        printf("cannot create reports.data!");
    }

    free(dirPath);

    return 0;
}