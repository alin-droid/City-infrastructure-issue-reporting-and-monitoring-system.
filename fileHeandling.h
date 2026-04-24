#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "processArguments.h"

typedef struct content ReportContent_t; 

//functii pt verificare daca existe fisierele
int dirExists(char *path);
int fileExists(char *path);

//functii pentru dir si dirpath
int createDir(const char *path);
char* findDirPath(char *dirname);

//functii pt fisiere si filepath
char* findFilePath(char *directoryPath, char *fileName);
int createFile(char *path,char *filename);
int createFileWithPermission(char *dirPath,char *fileName,mode_t perm);
int addNewReport(Role_t role,ReportContent_t *content,char *dirPath,char *fileName);
ReportContent_t  *createContent(int reportID,char *inspectorName,float latitude,float longitude,char *issue,char *description);

#endif