#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processArguments.h"
#include "fileHeandling.h"
#include "permissions.h"

#define MAX_FILE_NAME_LENGTH 100
#define MAX_NUM_OF_FILES 3
#define MAX_FILE_PATH_LENGTH 1024

char fileNames[MAX_NUM_OF_FILES][MAX_FILE_NAME_LENGTH] = {"reports.dat","district.cfg","logged_district"};

ReportContent_t *createContentFromFile(const char *filename , int argc , char *argv[]) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        perror("Eroare la deschiderea fisierului");
        return NULL;
    }
    
    int id;
    float latitude, longitude;
    char issue[100];
    char description[256];


    if (fscanf(f, "%d %f %f\n", &id,&latitude, &longitude) != 3) {
        fclose(f);
        return NULL;
    }
    //printf("%d %f %f\n",id,latitude,longitude);

    if (fgets(issue, sizeof(issue), f) == NULL) {
        fclose(f);
        return NULL;
    }
    
    //printf("%s\n",issue)

    if (fgets(description, sizeof(description), f) == NULL) {
        fclose(f);
        return NULL;
        
    }
    //printf("%s\n",description);

    fclose(f);
    
    char *user=getUser(argc,argv);
    //printf("%s",user)
    return createContent(id,user,latitude, longitude, issue, description);
}

int main(int argc, char *argv[])
{
    if (validateArguments(argc, argv) == 0) {
        printf("Invalid arguments! Please try again.\n");
        return 1;
    }

    Operation_t op = getOperation(argc, argv);
    Role_t role = getRole(argc, argv);

    char *district = getDistrict(argc, argv);
    char *dirPath = findDirPath(district);

    if (dirPath == NULL) {
        printf("Error creating path\n");
        return 1;
    }

    switch (op) {

        case add: {
            if (createDir(dirPath) == 1) {
                printf("Directory already exists!\n");
            }

            createFileWithPermission(dirPath, fileNames[0], 0664);
            createFileWithPermission(dirPath, fileNames[1], 0640);
            createFileWithPermission(dirPath, fileNames[2], 0644);
 
            ReportContent_t *content =createContentFromFile("datePtRaports.txt",argc,argv);
            if(content==NULL){
                printf("content is not rigth!");
                return -1;
            }
            addNewReport(role, content, dirPath, fileNames[0]);
            break;
        }

        case list: {
            char filePaths[MAX_NUM_OF_FILES][MAX_FILE_PATH_LENGTH];

            for (int i = 0; i < MAX_NUM_OF_FILES; i++) {
                char *path = findFilePath(dirPath, fileNames[i]);

                if (path == NULL) {
                    printf("Error generating file path for %s\n", fileNames[i]);
                    continue;
                }

                strcpy(filePaths[i], path);
            
                printf("%s\n", filePaths[i]);
            }
            break;
        }

        default:
            printf("Unknown operation\n");
            break;
    }

    return 0;
}