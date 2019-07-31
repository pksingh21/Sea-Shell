#include "headers.h"
void SetEnv(char** args)
{
	if(args[1]==NULL || args[3]!=NULL)
	{
		printf("No of Argumnets to setenv should be 1\n");
		return;
	}
	if(args[2]==NULL)
		setenv(args[1]," ",1);
	else
		setenv(args[1],args[2],1);
	return;
}
void UnSetEnv(char** args)
{
	if(args[1]==NULL)
	{
		printf("At least 1 command line Argumnet expected\n");
		return;
	}
	for(int i=1;args[i]!=NULL;i++)
	{
		if(unsetenv(args[i])==-1)
			printf("unsetenv: \n");
	}
	return;
}