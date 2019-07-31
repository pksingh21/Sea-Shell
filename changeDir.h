#include "headers.h"
void change_dir(char** args,char* root)
{
	if(args[1]==NULL)
		chdir(root);
	else if(args[1][0]=='~')
	{
		chdir(root);
		if(args[1][2]!='\0')
		{
			if(chdir(&args[1][2])!=0)
			{
				perror("SHELL");
			}
		}
	}
	else if(chdir(args[1])!=0)
		perror("SHELL");
}