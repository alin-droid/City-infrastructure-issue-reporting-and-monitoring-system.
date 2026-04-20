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
    remove_report,
    filter,
    update_threshold
} Command_t;

Role_t getRole(int argc, char *argv[]);
Command_t getCommand(int argc, char *argv[]);
char *getDistrict(int argc, char *argv[]);
int validateArguments(int argc, char *argv[]);

#endif