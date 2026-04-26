//Prima varianta generata 

//i-am trimis arhiva cu codul sursa de pe git
//promt: Fa-mi o functie match_condition care primeste un pointer la struct si o conditie de forma field:operator:value si returneaza 1 daca recordul satisface conditia.
//am folosit claude ai 
//Am observat dupa 2 rulari ca indiferent de filtru imi da acelasi rezultat (nu facea diferenta intre >= <=)
//intra pe ramura cu ==


int match_condition(ReportContent_t *r,char *condition)
{
    char field[100];
    char op[10];
    char value[256];

    if(sscanf(condition,"%[^:]:%[^:]:%[^:\n]",field,op,value) != 3)
        return 0;

    if(strcmp(field,"severity") == 0)
    {
        int val = atoi(value);
        
        //AICI
        if(op[0] == '>')
            return r->severityLevel > val;

        if(strcmp(op,">=") == 0)
            return r->severityLevel >= val;
        
        //AICI
        if(op[0] == '<')
            return r->severityLevel < val;

        if(strcmp(op,"<=") == 0)
            return r->severityLevel <= val;

        if(strcmp(op,"==") == 0)
            return r->severityLevel == val;

        if(strcmp(op,"!=") == 0)
            return r->severityLevel != val;
    }

    if(strcmp(field,"category") == 0)
    {
        if(strcmp(op,"==") == 0)
            return strcmp(r->issue,value) == 0;

        if(strcmp(op,"!=") == 0)
            return strcmp(r->issue,value) != 0;
    }

    if(strcmp(field,"inspector") == 0)
    {
        if(strcmp(op,"==") == 0)
            return strcmp(r->inspectorName,value) == 0;

        if(strcmp(op,"!=") == 0)
            return strcmp(r->inspectorName,value) != 0;
    }

    if(strcmp(field,"timestamp") == 0)
    {
        long val = atol(value);

        if(strcmp(op,"==") == 0)
            return r->time == val;

        if(strcmp(op,"!=") == 0)
            return r->time != val;

        if(strcmp(op,">") == 0)
            return r->time > val;

        if(strcmp(op,"<") == 0)
            return r->time < val;

        if(strcmp(op,">=") == 0)
            return r->time >= val;

        if(strcmp(op,"<=") == 0)
            return r->time <= val;
    }

    return 0;
}



//dupa mi-a dat implementarea buna .

//A fost un pic bataie de cap deoarece am implement cu biblioteci is atunci mi a fost mai greu sa mi explic structura datelor + sa intelegeaga ce trimit ca armunte
//in bibliotecare care se ocupa de procesarea argumentelor in fucntie de operatia dorinta am fct un getter pt condtii care imi returneaza un tabloul de stringuri fiecare reprezentand un string
//sub forma field:operator:value 
//cred ca daca nu as fi gandit logica asta separat ar fi fost extrem de greu ca ai-ul sa intelegea toata structura proiectului meu 


//Ar fi fost mai usor sa ma gandesc eu cum sa fac functia si doar sa pun anumite intrebari de implementare.


//UPDATE: 
//recitind cerintele proiectului am realizat ca nu i am dat un promt corect si in loc sa fac cele doua functii separate le am implemntat in aceiasi
//promt:
//separa functia in doua: una parse_condition care primeste stringul field:operator:value si il imparte in cele 3 parti, si una match_condition care primeste partile deja parsate si face comparatia.
//desi e odar partea asta in match ea trebuia sa fie in parse
    if(sscanf(condition,"%[^:]:%[^:]:%[^:\n]",field,op,value) != 3)
        return 0;

//am inv ca daca nu dai un prompt exact care sa reflecte cerinta, AI-ul face ce i se pare logic dar nu neaparat ce trebuie. A trebuit sa recitesc cerinta si sa reformulez promptul.