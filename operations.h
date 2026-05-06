#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "processArguments.h"
#include "fileHeandling.h"

#define MAX_FILE_NAME_LENGTH 100
#define MAX_NUM_OF_FILES 3
#define MAX_FILE_PATH_LENGTH 1024

extern char fileNames[MAX_NUM_OF_FILES][MAX_FILE_NAME_LENGTH];

ReportContent_t *createContentFromFile(const char *filename,int argc,char *argv[],char *reportsPath);

int addOperation(Role_t role, char *dirPath, int argc, char *argv[] , int modalitateCitire);

int listOperation(Role_t role,char *dirPath,int argc,char *argv[]);

int viewOpereation(Role_t role,char *dirPath,int argc,char *argv[]);

int deleteOperation(Role_t role,char *dirPath,int argc,char *argv[]);

int updateOperation(Role_t role,char *dirPath,int argc,char *argv[]);

int filterOperation(Role_t role,char *dirPath,int argc,char *argv[]);

int removeOperation(Role_t role,char *dirPath,int argc,char *argv[]);

#endif