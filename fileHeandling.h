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

//functii pt fisierul raport.dat

//functii de adaugare
int addNewReport(Role_t role,ReportContent_t *content,char *dirPath,char *fileName);
ReportContent_t  *createContent(int reportID,char *inspectorName,float latitude,float longitude,char *issue,int severityLevel,char *description);

//de afisare sub anumite forme
void printReports(Role_t role,char *filePath);
void printReportContent(ReportContent_t raport);
void printRaport(Role_t role,char *filePath,int id);
//de check
int reportIdExists(char *filePath, int searchedID);
//stegrere
void deleteRaport(Role_t role,char *filePath,int id);
//filtrare
void filterRaports(char *filePath, char **conditions,int numOfCondtions);

//linkire
void createActiveReportsLink(char *districtName, char *reportsPath);
void checkActiveReportsLinks();


//functii pt config
void addThresholdInConfig(Role_t role,char *filePath, char *thresholdValue);
void insertNewThresold(Role_t role,char *filePath,int newThreshold);

//functii pt log
void addLogInDistrict(char *filePath, Role_t role, char *userName, char *actionName);

//remove district

void removeDistrict(Role_t role,char *dirPath,char *district);

#endif