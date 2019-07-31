#include "headers.h"
void execfg(char** args)
{
	if(strcmp(args[0],"fg")==0)
	{
		if(atoi(args[1])>pcnt)
		{
			printf("Process with job id %s doesnt exist\n",args[1]);
			return;
		}
		else
		{
			if(args[1]!=NULL && args[2]==NULL)
			{
				while(1)
				{
					if(kill(jb[atoi(args[1])-1].PID,0)==-1)
					{
						break;
					}
					else
						wait(NULL);
				}
			}
			else
				printf("There should be 3 arguments\n");
		}
		return;
	}
}