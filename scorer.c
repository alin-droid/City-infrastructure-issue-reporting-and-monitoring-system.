#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include "fileHeandling.h"
//#include "permissions.h"

#define MAX 100
#define MAX_SIZE_INSPECTORS 1000


typedef struct{
    char inspectorName[MAX];
    int workloadScore;
}Inspector;


int inspectorPosition(Inspector *arr,int n,char *name){
     
     for(int i=0;i<n;i++){
        if(strcmp(arr[i].inspectorName,name)==0){
            return i;
        }
     }

     return -1;
}



void printInspector(Inspector *inspector){

    printf("Name= %s    Workload Score =%d \n",inspector->inspectorName,inspector->workloadScore);

}

void printAllInspector(Inspector *arr , int n){

    for(int i=0;i<n;i++)
    {
        printInspector(&arr[i]);
    }
}

int main(int argc,char *argv[]){
    
    //trm ca argument numele districtului
    if(argc!=2){
        printf("you have to give us a district name!\n");
        exit(-1);
    }
    
    //vad daca exista districtul si incep sa citesc numele inspoectorilor din el
    char *districtPath=NULL;

    districtPath=findDirPath(argv[1]);
    
    if(districtPath==NULL){
        printf("the district doesnt exist!\n");
        return 0;
    }

    char *filePath=NULL;
     
    filePath=findFilePath(districtPath,"reports.dat");

    if(filePath==NULL){
        printf(" It doesn't have a report.dat file!\n");
        return 0;
    }

    
    FILE *f=fopen(filePath,"r");
    if(f==NULL){
        printf("could not open file!\n");
        return 0;
    }

    Inspector *arr=malloc(MAX_SIZE_INSPECTORS*sizeof(Inspector));

    if(arr==NULL){
        perror("memorry cannot br allocated!\n");
        exit(-1);
    }

    int n=0;
    
    
    ReportContent_t raport;
   
    while(fread(&raport, sizeof(ReportContent_t), 1, f) == 1)
    {  
       char *name=getContentInsepctorName(raport);
       int severityLevel=getContentSeverityLevel(raport);

       int index=inspectorPosition(arr,n,name);

       if(index==-1){
         strcpy(arr[n].inspectorName,getContentInsepctorName(raport));
         arr[n].workloadScore=0;
         arr[n].workloadScore+=getContentSeverityLevel(raport);
         n++;
       }
       else{
              arr[index].workloadScore+=severityLevel;
            }
        
       //printf("%d\n",getContentSeverityLevel(raport));
    }
    
    printAllInspector(arr,n);

    return 0;
   
    free(arr);
}