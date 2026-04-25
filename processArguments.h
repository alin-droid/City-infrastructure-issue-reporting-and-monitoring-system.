#ifndef PROCESS_ARGUMENTS_H
#define PROCESS_ARGUMENTS_H

//am ales sa folosesc enum ca sa pot folosi case mai departe
//argumentele fiind string-uri ar fi foarte greu sa folosesc multe if-uri de fiecare data

//plus ca daca pe parcurs vor aparea si alte comenzi + roluri va trebui sa modific
//multe if-uri din cod -> nu mai e scalabit

typedef enum
{   //rolurile pe care le-am vazut momentan in exemple
    invalid_role = -1,
    manager,
    inspector
} Role_t;

typedef enum
{   
    //come nziile din exemple
    no_command,
    add,
    list,
    view,
    remove_report,
    filter,
    update_threshold
} Operation_t;

//gettere pt info din argumente
Role_t getRole(int argc, char *argv[]);
Operation_t getOperation(int argc, char *argv[]);
char *getUser(int argc, char *argv[]);
char *getDistrict(int argc, char *argv[]);

int getIdReport(int argc,char *argv[]);
int getThreshold(int argc,char *argv[]);
int validateArguments(int argc, char *argv[]);
char **getConditions(int argc,char *argv[]);

#endif