#include "headers.h"

void piping(char** args)
{
	int i,Pipes=0;
	for(i=0;args[i]!=NULL;i++)
	{
		if(strcmp(args[i],"|")==0)
			Pipes++;
	}
	int len=i;
	char* commands[100];
	int pipes[50][2];	
	int j=0;
	int curr=0;
	int k,lst=0;
	while(lst==0)
	{
		k=0;
		while(strcmp(args[j],"|")!=0)
		{
			commands[k]=args[j];
			j++;
			k++;
			if(args[j]==NULL)
			{
				lst=1;
				break;
			}			
		}
		j++;
		commands[k]=NULL;
		pipe(pipes[curr]);
		if(curr==0)
		{
			pid_t pid=fork();
			if(pid==0)
			{
				dup2(pipes[curr][1],1);
				close(pipes[curr][0]);
				if(strcmp(commands[k-2],"<")==0)
				{
					int fin=open(commands[k-1],O_RDONLY);
					if(fin<0)
					{
						printf("Error Reading From Input file\n");
						exit(0);
					}
					dup2(fin,0);
					close(fin);
					commands[k-2]=NULL;
				}
				execvp(commands[0],commands);
			}
			else
			{
				wait(NULL);
				close(pipes[curr][1]);
			}
		}
		else if(curr==Pipes)
		{
			pid_t pid=fork();
			if(pid==0)
			{
				dup2(pipes[curr-1][0],0);
				close(pipes[curr-1][1]);
				if(strcmp(commands[k-2],">")==0)
				{
					struct stat st;
					if(stat(commands[k-1],&st)!=0)
					{
						int fout=creat(commands[k-1],0644);
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
						int fout=open(commands[k-1],O_WRONLY | O_TRUNC);
						if(fout<0)
						{
							printf("Error: Opening Output File\n");
							exit(0);
						}
						dup2(fout,STDOUT_FILENO);
						close(fout);
					}
					commands[k-2]=NULL;
				}
				else if(strcmp(commands[k-2],">>")==0)
				{
					struct stat st;
					if(stat(commands[k-1],&st)!=0)
					{
						int fout=creat(commands[k-1],0644);
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
						int fout=open(commands[k-1],O_APPEND | O_WRONLY);
						if(fout<0)
						{
							printf("Error: Opening Output File\n");
							exit(0);
						}
						dup2(fout,STDOUT_FILENO);
						close(fout);
					}
					commands[k-2]=NULL;
				}
				execvp(commands[0],commands);
			}
			else
			{
				wait(NULL);
				close(pipes[curr-1][0]);
			}
		}
		else
		{
			pid_t pid=fork();
			if(pid==0)
			{
				dup2(pipes[curr][1],1);
				dup2(pipes[curr-1][0],0);
				close(pipes[curr][0]);
				close(pipes[curr-1][1]);
				execvp(commands[0],commands);
			}
			else
			{
				wait(NULL);
				close(pipes[curr][1]);
				close(pipes[curr-1][0]);
			}
		}
		curr++;
	}
	return;
}