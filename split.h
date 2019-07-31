#include "headers.h"
#define Delim " ,\t,\n,\r"
char** getAll(char* s)
{
	char **cmds = malloc(100* (sizeof(char*)));
    char *tk;
    int i;
	tk  = strtok(s,";,\n");
    for (i = 0;; i++)
    {
        if (tk!= NULL)
        {
        	cmds[i] = tk; 
            tk = strtok(NULL,";,\n");
            continue;
        }
        break;
    }
    cmds[i] = NULL;
    return cmds;
}
char** Split(char* s)
{
	char **cmds = malloc(10000* (sizeof(char*)));
    char *tk;
    int i;
	tk  = strtok(s,Delim);
    for (i = 0;; i++)
    {
        if (tk!= NULL)
        {
        	cmds[i] = tk; 
            tk = strtok(NULL,Delim);
            continue;
        }
        break;
    }
    cmds[i] = NULL;
    return cmds;
}