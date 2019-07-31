#include "headers.h"

int HandleRedirection(char** args)
{
	int i;
	int ind=-1;
	int in=0;
	int op=0;
	int ap=0;
	char INP_FILE[100],OUT_FILE[100];
	pid_t pid=fork();
	if(pid==0)
	{
		for(i=0;args[i]!=NULL;i++)
		{
			if(strcmp(args[i],"<")==0)
			{
				in=1;
				ind=i;
				strcpy(INP_FILE,args[i+1]);
			}
			if(strcmp(args[i],">")==0)
			{
				op=1;
				if(ind==-1)
					ind=i;
				strcpy(OUT_FILE,args[i+1]);
			}
			if(strcmp(args[i],">>")==0)
			{
				ap=1;
				if(ind==-1)
					ind=i;
				strcpy(OUT_FILE,args[i+1]);
			}
		}
		struct stat st;
		if(in)
		{
			if(stat(INP_FILE,&st)!=0)
			{
				printf("Error: Input File Doesn't Exist\n");
				exit(0);
			}
			int fin=open(INP_FILE,O_RDONLY);
			if(fin<0)
			{
				printf("Error: Opening Input File\n");
				exit(0);
			}
			dup2(fin,STDIN_FILENO);
			close(fin);
		}
		if(op)
		{
			if(stat(OUT_FILE,&st)!=0)
			{
				int fout=creat(OUT_FILE,0644);
				if(fout<0)
				{
					printf("Error: Creating Output File\n");
					exit(0);
				}
				dup2(fout,STDOUT_FILENO);
				close(fout);
			}
			else
			{
				int fout=open(OUT_FILE,O_WRONLY | O_TRUNC);
				if(fout<0)
				{
					printf("Error: Opening Output File\n");
					exit(0);
				}
				dup2(fout,STDOUT_FILENO);
				close(fout);
			}
		}
		if(ap)
		{
			if(stat(OUT_FILE,&st)!=0)
			{
				int fout=creat(OUT_FILE,0644);
				if(fout<0)
				{
					printf("Error: Creating Output File\n");
					exit(0);
				}
				dup2(fout,STDOUT_FILENO);
				close(fout);
			}
			else
			{
				int fout=open(OUT_FILE,O_APPEND | O_WRONLY);
				if(fout<0)
				{
					printf("Error: Opening Output File\n");
					exit(0);
				}
				dup2(fout,STDOUT_FILENO);
				close(fout);
			}
		}
		args[ind]=NULL;
		execvp(args[0],args);
	}
	else
	{
		wait(NULL);
	}
	return 1;
}