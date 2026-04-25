#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "operations.h"
#include "permissions.h"
#include "fileHeandling.h"

//mi am facut acest tablou de caractere in caz ca pe viitor voi avea si alte fisiere
char fileNames[MAX_NUM_OF_FILES][MAX_FILE_NAME_LENGTH] ={  "reports.dat","district.cfg","logged_district"};

//pt ca le folosesc desc o sa le creez cu o functie
void createFilePaths(char filePaths[MAX_NUM_OF_FILES][MAX_FILE_PATH_LENGTH],
                     char *dirPath)
{
    for(int i = 0; i < MAX_NUM_OF_FILES; i++)
    {
        char *path = findFilePath(dirPath, fileNames[i]);

        if(path == NULL)
        {
            exit(-1);
        }

        strcpy(filePaths[i], path);
        free(path);
    }
}

//functia de citire din fisier
//fataq de varianta nateroara acum functia e in stare sa citeasca raporte cu id diferit sa nu se adauge mereu pria linie din fisierul de test
ReportContent_t *createContentFromFile(const char *filename,int argc,char *argv[],char *reportsPath)
{
    FILE *f = fopen(filename, "r");

    if(f == NULL)
    {
        perror("Eroare la deschiderea fisierului");
        return NULL;
    }

    int id;
    int severityLevel;
    float latitude, longitude;
    char issue[100];
    char description[256];

    while(fscanf(f, "%d %f %f\n", &id,&latitude,&longitude) == 3)
    {
        if(fgets(issue, sizeof(issue), f) == NULL)
        {
            fclose(f);
            return NULL;
        }

        if(fscanf(f,"%d ",&severityLevel)!=1){
            fclose(f);
            return NULL;
        }

        if(fgets(description, sizeof(description), f) == NULL)
        {
            fclose(f);
            return NULL;
        }
        
        //prima data cand folosesc asta willing 
        //daca raman \n se afiseaza ff dubios in temrinal
        issue[strcspn(issue, "\n")] = '\0';
        description[strcspn(description, "\n")] = '\0';

        //dupa ce am citit raportul verific daca e ok raportul adica ca nu a mai fost introdus in fucntie de id
        // functia reportIdExists este in bibiblioteca de files deoarece am folosit encapsulare prin biblioteci si atunci nu pot accesa direct campuri din structura 
        if(reportIdExists(reportsPath, id) == 0)
        {   
            fclose(f);

            char *user = getUser(argc, argv);
            //creez informatia pe care urmeaza sa o pun in fisier
            return createContent(id, user,latitude,longitude,issue,severityLevel,description);
        }
    }

    fclose(f);
    return NULL;
}

//functiia care imi face ioperatiile specifice pt add
int addOperation(Role_t role, char *dirPath, int argc, char *argv[])
{   
    char filePaths[MAX_NUM_OF_FILES][MAX_FILE_PATH_LENGTH];
    createDir(dirPath);
    //creez fisierele cu permisiunile dorite
    createFileWithPermission(dirPath, fileNames[0], 0664);
    createFileWithPermission(dirPath, fileNames[1], 0640);
    createFileWithPermission(dirPath, fileNames[2], 0644);

    createFilePaths(filePaths, dirPath);

    //creez continutul 
    ReportContent_t *content =createContentFromFile("datePtRaports.txt",argc, argv,filePaths[0]);

    if(content == NULL)
    {
        printf("No new reports found!\n");
        return -1;
    }
    
    //il adaug in reports.dat
    addNewReport(role, content, dirPath, fileNames[0]);
    
    //acum adaug threshold in config 
    addThresholdInConfig(role,filePaths[1], "4");

    //adaug in log 
    addLogInDistrict(filePaths[2],role,getUser(argc,argv),"add");

    free(content);

    return 0;
}

int listOperation(Role_t role,char *dirPath,int argc,char *argv[])
{   
    if(dirExists(dirPath)==0){
        printf("dir doesnt exist!\n");
        exit(-1);
    }
    char filePaths[MAX_NUM_OF_FILES][MAX_FILE_PATH_LENGTH];

    createFilePaths(filePaths, dirPath);
    
    //printez permisiunile
    printPermissionsForFile(filePaths[0], fileNames[0]);

    //printez continutul raportului
    printReports(role,filePaths[0]);
    
    //adaug in log
    addLogInDistrict(filePaths[2],role,getUser(argc,argv),"list");

    return 0;
}


int viewOpereation(Role_t role,char *dirPath,int argc,char *argv[]){

    //este valabil pt amandouaroluri
    if(role != inspector && role != manager){
       printf("No allowed roles for view");
       return -1;
    }

     if(dirExists(dirPath)==0){
        printf("dir doesnt exist!\n");
        exit(-1);
    }
    char filePaths[MAX_NUM_OF_FILES][MAX_FILE_PATH_LENGTH];

    createFilePaths(filePaths, dirPath);
    //gasesc id ul din argument
    int id=getIdReport(argc,argv);
    
    //aplez functia de afisarea a unui raport dupa id
    printRaport(role,filePaths[0],id);
    
    //adaug in log
    addLogInDistrict(filePaths[2],role,getUser(argc,argv),"view");

    return 0;
}


int deleteOperation(Role_t role,char *dirPath,int argc,char *argv[]){
     //doar manager-ul
    if(role != manager){
       printf("No allowed roles for removing\n");
       return -1;
    }

     if(dirExists(dirPath)==0){
        printf("dir doesnt exist!\n");
        exit(-1);
    }
    char filePaths[MAX_NUM_OF_FILES][MAX_FILE_PATH_LENGTH];

    createFilePaths(filePaths, dirPath);
    //gasesc id ul din argument
    int id=getIdReport(argc,argv);

    deleteRaport(role,filePaths[0],id);
    addLogInDistrict(filePaths[2],role,getUser(argc,argv),"remove_repot");
    return 0;
}


int updateOperation(Role_t role,char *dirPath,int argc,char *argv[]){
     if(role != manager){
       printf("No allowed roles for uptating threshold\n");
       return -1;
    }

     if(dirExists(dirPath)==0){
        printf("dir doesnt exist!\n");
        exit(-1);
    }
    char filePaths[MAX_NUM_OF_FILES][MAX_FILE_PATH_LENGTH];

    createFilePaths(filePaths, dirPath);

    int newTheersold=getThreshold(argc,argv);

    insertNewThresold(role,filePaths[1],newTheersold);

    addLogInDistrict(filePaths[2],role,getUser(argc,argv),"updateThreshold");
    return 0;
}

int filterOperation(Role_t role,char *dirPath,int argc,char *argv[]){
    
     if(role != inspector && role != manager){
       printf("No allowed roles for fileter\n");
       return -1;
    }

     if(dirExists(dirPath)==0){
        printf("dir doesnt exist!\n");
        exit(-1);
    }
    char filePaths[MAX_NUM_OF_FILES][MAX_FILE_PATH_LENGTH];

    createFilePaths(filePaths, dirPath);

    char **conditions=getConditions(argc,argv);

    filterRaports(filePaths[0], conditions);

    return 0;

}