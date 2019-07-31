#include "headers.h"
void process_info(char** args,char* root)
{
	int pid,memory;
	char s[102];
	char process_status;
	char Process[1000];
	char fle[1000];
	if(args[1]==NULL)
	{
		//Pid
		strcpy(Process,"/proc/self/stat");
		FILE * fd=fopen(Process,"r");
		fscanf(fd,"%d",&pid);
		fclose(fd);
		printf("Pid %d\n",pid);
		strcat(Process,"us");
		fd=fopen(Process,"r");
		if(fd==NULL)
		{
			perror("Error Opening File: ");
			return;
		}
		while(1)
		{
			fgets(s,100,fd);
			if(s[0]=='S')
				break;
		}
		//status
		printf("Process_");
		printf("%s",s );
		//Memory
		strcpy(Process,"/proc/self/statm");
		fd=fopen(Process,"r");
		if(fd==NULL)
		{
			perror("Error Opening File: ");
			return;
		}
		fscanf(fd,"%d",&memory);
		printf("Memory %d\n",memory );
		fclose(fd);
		//Executable
		strcpy(Process,"/proc/self/exe");
		printf("Executable Path: %s\n",getpath(realpath(Process,NULL),root));
		// printf("Executable Path: ~/a.out\n");
	}
	else
	{
		strcpy(Process,"/proc/");
		strcpy(fle,Process);
		strcat(fle,args[1]);
		struct stat st;
		if(stat(fle,&st)==0)
		{
			//Pid
			strcat(fle,"/stat");
			FILE * fd=fopen(fle,"r");
			if(fd==NULL)
			{
				perror("Error Opening File: ");
				return;
			}
			fscanf(fd,"%d",&pid);
			printf("Pid %d\n",pid );
			fclose(fd);
			strcat(fle,"us");
			fd=fopen(fle,"r");
			if(fd==NULL)
			{
				perror("Error Opening File: ");
				return;
			}
			while(1)
			{
				fgets(s,100,fd);
				if(s[0]=='S')
					break;
			}
			// status
			printf("Process_%s",s);
			fclose(fd);
			strcpy(fle,Process);
			strcat(fle,args[1]);
			strcat(fle,"/statm");
			fd=fopen(fle,"r");
			if(fd==NULL)
			{
				perror("Error Opening File: ");
				return;
			}
			fscanf(fd,"%d",&memory);
			printf("Memory %d\n",memory );
			fclose(fd);
			strcpy(fle,Process);
			strcat(fle,args[1]);
			strcat(fle,"/exe");
			char* path=realpath(fle,NULL);
			if(path==NULL)
				printf("Executable Path: (null)\n");				
			else
				printf("Executable Path: %s\n",getpath(path,root));
		}
		else
			printf("Process with Pid: %s Does not exist\n",args[1]);
	}
}