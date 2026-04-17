#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#define minNumberOfArguments 5
#define sizeDirName 1000

int directoryExists(const char *path)
{
    struct stat st;

    if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
        return 1;

    return 0;
}

int fileExists(const char *path)
{
    struct stat st;

    if (stat(path, &st) == 0 && S_ISREG(st.st_mode))
        return 1;

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < minNumberOfArguments)
    {
        fprintf(stderr, "There are missing arguments!\n");
        exit(-1);
    }

    if (strcmp(argv[2], "inspector") == 0)
        printf("am rolul de %s\n", argv[2]);

    if (strcmp(argv[2], "manager") == 0)
        printf("am rolul de %s\n", argv[2]);

    char *dirName = malloc(sizeDirName);
    if (dirName == NULL)
    {
        fprintf(stderr, "Failed malloc!\n");
        exit(-1);
    }

    if (getcwd(dirName, sizeDirName) == NULL)
    {
        fprintf(stderr, "Cannot find working dir path!\n");
        free(dirName);
        exit(-1);
    }

    strcat(dirName, "/");
    strcat(dirName, argv[4]);

    if (directoryExists(dirName))
    {
        printf("Directorul exista deja.\n");
    }
    else
    {
        if (mkdir(dirName, 0777) != 0)
        {
            fprintf(stderr, "Unable to create directory\n");
            free(dirName);
            exit(-1);
        }

        printf("Director creat.\n");
    }

    if (chdir(dirName) != 0)
    {
        fprintf(stderr, "Cannot move into district directory.\n");
        free(dirName);
        exit(-1);
    }

  
    if (fileExists("reports.dat"))
    {
        printf("reports.dat exista deja.\n");
    }
    else
    {
        FILE *f = fopen("reports.dat", "wb");
        if (f == NULL)
            fprintf(stderr, "Cannot create reports.dat\n");
        else
        {
            printf("reports.dat creat.\n");
            fclose(f);
        }
    }

    
    if (fileExists("district.cfg"))
    {
        printf("district.cfg exista deja.\n");
    }
    else
    {
        FILE *f = fopen("district.cfg", "w");
        if (f == NULL)
            fprintf(stderr, "Cannot create district.cfg\n");
        else
        {
            fprintf(f, "severity_threshold=2\n");
            printf("district.cfg creat.\n");
            fclose(f);
        }
    }

    FILE *logFile = fopen("logged_district", "a");
    if (logFile == NULL)
    {
        fprintf(stderr, "Cannot open logged_district\n");
    }
    else
    {

        fprintf(logFile, "am acest fisier deja!");
    }

    free(dirName);
    return 0;
}