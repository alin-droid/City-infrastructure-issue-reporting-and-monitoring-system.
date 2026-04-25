#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include "processArguments.h"

//comenziile sunt sub forma --role rolul_propiu_zis
Role_t getRole(int argc, char *argv[]){
    //parcurg vectorul de string-uri de argumente
    for(int i = 1; i < argc - 1; i++){
        //cand gasesc --role stiu ca urm poz e rolul in sine
        if(strcmp(argv[i], "--role") == 0){
            //in functie de ce rol e il returnez
            if(strcmp(argv[i + 1], "manager") == 0)
                return manager;
            
            if(strcmp(argv[i + 1], "inspector") == 0)
                return inspector;
        }
    }
    //daca  nu am --role insemana ca argumentele sunt gresite returnez un cod de eroare
    return invalid_role;
}

//este asemanator cu functia de getRole doar ca aici nu am cum sa imi fac un tip de date
//pt user deoarece aceste poate fi ales diferit de fricare data
char *getUser(int argc, char *argv[])
{
    for(int i = 1; i < argc - 1; i++)
    {
        if(strcmp(argv[i], "--user") == 0)
        {
            return argv[i + 1];
        }
    }

    return NULL;
}

//deoarece pot avea --user sau nu (din exemple)
//trebe sa verific cu --comanda deoarece nu am o piztie fixa
Operation_t getOperation(int argc, char *argv[])
{
    for(int i = 1; i < argc; i++)
    {   
        if(strcmp(argv[i], "--add") == 0) return add;
        if(strcmp(argv[i], "--list") == 0) return list;
        if(strcmp(argv[i], "--remove_report") == 0) return remove_report;
        if(strcmp(argv[i], "--filter") == 0) return filter;
        if(strcmp(argv[i], "--update_threshold") == 0) return update_threshold;
        if(strcmp(argv[i], "--view") == 0) return view;
    }

    return no_command;
}
//stiu ca dupa comanda o sa am numele districtului
char *getDistrict(int argc, char *argv[])
{
    for(int i = 1; i < argc - 1; i++)
    {
        if(strcmp(argv[i], "--add") == 0 ||
           strcmp(argv[i], "--list") == 0 ||
           strcmp(argv[i], "--filter") == 0 ||
           strcmp(argv[i], "--remove_report") == 0 ||
           strcmp(argv[i], "--update_threshold") == 0 ||
           strcmp(argv[i],"--view") ==0
         )
        {
            return argv[i + 1];
        }
    }

    return NULL;
}

int getIdReport(int argc,char *argv[]){
    
     Operation_t op=getOperation(argc,argv);
     if(op==view){
        int id=atoi(argv[argc-1]);
        if(id<=0){
            printf("argumments error!");
            exit(-1);
        }
        return id;
     }
     return -1;
}

//pretty self explenatory 
int validateArguments(int argc, char *argv[])
{   
    if(getRole(argc, argv) == invalid_role)
    {
        printf("the role is missing or does not have --role\n");
        return 0;
    }

    if(getOperation(argc, argv) == no_command)
    {
        printf("command is missing or does not hace --command\n");
        return 0;
    }

    return 1;
}