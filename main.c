#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processArguments.h"
#include "fileHeandling.h"
#include "permissions.h"

#define MAX_FILE_NAME_LENGTH 100
#define MAX_NUM_OF_FILES 3
#define MAX_FILE_PATH_LENGTH 1000000

char fileNames[MAX_NUM_OF_FILES][MAX_FILE_NAME_LENGTH]={"reports.dat","district.cfg","logged_district"};


int main(int argc, char *argv[])
{
    if(validateArguments(argc, argv) == 0){
        return 1;
    }
    
    //iau numele distrctului 
    char *district = getDistrict(argc, argv);
    
    //creez calea pt folder
    char *dirPath = findDirPath(district);
    //printf("%s",dirPath)
 
    if(dirPath == NULL){
        printf("error creating path\n");
        return 1;
    }
   
    //construiesc folder-ul 
    if(createDir(dirPath)==1){
        printf("the directory is already created!\n");
    }


    //creez fisierle pt folder cu permisiunile specificate
    createFileWithPermission(dirPath, fileNames[0], 0664);
    createFileWithPermission(dirPath, fileNames[1], 0640);
    createFileWithPermission(dirPath, fileNames[2], 0644);
    
    //fac vectori cu calea catre fisier ca sa mi fie mai usor la verificari
    char filePaths[MAX_NUM_OF_FILES][MAX_FILE_PATH_LENGTH];
    for(int i=0;i<MAX_NUM_OF_FILES;i++){

       if(strcpy(filePaths[i],findFilePath(dirPath,fileNames[i]))==NULL){
          printf("Cannot add the filepath because it s too long");
       }

    }
    /*for(int i=0;i<MAX_NUM_OF_FILES;i++){
        printf("%s\n",filePaths[i]);
    }
    free(dirPath);*/
    
    /*test checkPermissions
    Role_t role=getRole(argc,argv);

    for(int i=0;i<MAX_NUM_OF_FILES;i++){
    checkPermissions(role,filePaths[i],fileNames[i]);
    }
    */
    
    /*test printPermissions
    
    for(int i=0;i<MAX_NUM_OF_FILES;i++){
         printPermissionsForFile(filePaths[i]);
    }
    */
    Operation_t op=getOperation(argc,argv);

    switch(op){
        case list: printPermissionsForFile(filePaths[0],fileNames[0]);
                     break;
        default: break;
    }


    return 0;
}