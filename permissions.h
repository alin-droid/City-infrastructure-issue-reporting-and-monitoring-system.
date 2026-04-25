#ifndef PERMISSION_H
#define PERMISSION_H

#include "processArguments.h"
#include "fileHeandling.h"
#include <time.h>

void checkPermissions(Role_t role, char *filePath);
void printPermissionsForFile(char *filePath,char *fileName);

#endif