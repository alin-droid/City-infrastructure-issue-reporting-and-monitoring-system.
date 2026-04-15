#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/limits.h>
#define minNumberOfArguments 5
#define sizeDirName 1000

int main(int argc, char *argv[]){

   if(argc < minNumberOfArguments){
     fprintf(stderr,"There are missing arguments!\n");
     exit(-1);
    }
    
    if( strcmp(argv[2],"inspector")==0){
        printf("am rolul de %s\n" , argv[2]);
    }

    if(strcmp(argv[2], "manager")==0) { 
        printf("am rolul de %s\n" , argv[2]);
    }
    
    char *dirName=malloc(sizeDirName*sizeof(char));
    if(dirName==NULL){
         fprintf(stderr,"Failed malloc!\n");
         exit(-1);
    }
    
    dirName=getcwd(dirName,sizeof(dirName)*8);
    
    if(dirName==NULL){
        fprintf(stderr,"Cannot find working dir path!\n");
        exit(-1);
    }

    if(strcat(dirName,argv[4])==0){
        fprintf(stderr,"Cannot create dir file for the district\n");
        exit(-1);
    }
    

    if(mkdir(dirName,0777)!=0){
        fprintf(stderr,"Unable to create directory\n");
    }
    else{
         printf("am creat directorul!");
    } 
    
    free(dirName);
    return 0;

}