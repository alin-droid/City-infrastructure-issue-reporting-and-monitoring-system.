#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "processArguments.h"
#include "operations.h"
#include "fileHeandling.h"

int main(int argc, char *argv[])
{   
    //valiedez argumentele trimise sa fiu sigut ca am primit un rol (specificat cum trebuie ) o operatie :add etc si un un nume de district si user
    if(validateArguments(argc, argv) == 0)
    {
        printf("Invalid arguments!\n");
        return 1;
    }

    //verific daca exista linkuri stricate
    checkActiveReportsLinks();
    
    //optin informatiile neesare
    Operation_t op = getOperation(argc, argv);
    Role_t role = getRole(argc, argv);

    char *district = getDistrict(argc, argv);
    //construiesc calea catre fisierul dorit adica fisierul districtului
    char *dirPath = findDirPath(district);

    if(dirPath == NULL)
    {
        printf("Error creating path\n");
        return 1;
    }
    
    //in functie de comnada fac ce mi se cere
    switch(op)
    {
        case add:
             printf("How would you like to add report?:\n");
             printf("1. read from a file named datePtReports.txt\n");
             printf("2.read from stdin\n");
             printf("Choose from those 2.\n");
             int modalitateCitire;
             scanf("%d",&modalitateCitire);

             if(modalitateCitire!=1 && modalitateCitire!=2){
               printf("please enter 1 or 2\n");
               return 1;
             }

             addOperation(role, dirPath, argc, argv,modalitateCitire);
             break;

        case list:
             listOperation(role,dirPath,argc,argv);
            break;
        
        case view:
             viewOpereation(role,dirPath,argc,argv);
             break;
        
        case remove_report:
             deleteOperation(role,dirPath,argc,argv);
             break;

        case update_threshold:
             updateOperation(role,dirPath,argc,argv);
             break;
        
        case filter:
             filterOperation(role,dirPath,argc,argv);
             break;

        case remove_district:
             removeOperation(role,dirPath,argc,argv);
             break;
               
        default:
            printf("Unknown operation\n");
    }

    free(dirPath);
    return 0;
}