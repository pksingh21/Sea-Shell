#include "headers.h"
void print_ls_l(char* fl)
{
	struct stat st;
	struct passwd *pw;
	struct group *gr;
	if(stat(fl,&st)==0)
	{
		//File Permissions
		printf((st.st_mode & S_IFDIR) ? "d":"-");
		printf((st.st_mode & S_IRUSR) ? "r":"-");
		printf((st.st_mode & S_IWUSR) ? "w":"-");
		printf((st.st_mode & S_IXUSR) ? "x":"-");
		printf((st.st_mode & S_IRGRP) ? "r":"-");
		printf((st.st_mode & S_IWGRP) ? "w":"-");
		printf((st.st_mode & S_IXGRP) ? "x":"-");
		printf((st.st_mode & S_IROTH) ? "r":"-");
		printf((st.st_mode & S_IWOTH) ? "w":"-");
		printf((st.st_mode & S_IXOTH) ? "x":"-");
		//NO of Hard Links
		printf("\t%d",(int)st.st_nlink);
		//Group Name and Username
		pw=getpwuid(st.st_uid);
		gr=getgrgid(st.st_gid);
		printf("\t%s\t%s",pw->pw_name,gr->gr_name);
		//Memory used
		printf("\t%lu",st.st_size);
		char date[100];
		//Timestamp of File
		strftime(date, 13, "%b %e %R", localtime(&(st.st_ctime)));
		printf("\t%s\t",date);
	}
}
void Listing(char** args)
{
	char FileName[100];
	char* Dt=(char* )(malloc(100));
	_Bool flag=0;
	if(args[2]!=NULL)
	{
		Dt=args[2];
		flag=1;
		if(strcmp(args[2],"-a")==0 || strcmp(args[2],"-l")==0)
		{
			if(args[3]!=NULL)
				Dt=args[3];
			else
			{
				Dt[0]='.';
				Dt[1]='\0';
			}
		}
	}
	else
	{
		Dt[0]='.';
		Dt[1]='\0';
	}
	if(args[1]==NULL)
	{
		DIR *curr_dir=opendir(Dt);
		if(curr_dir==NULL)
		{
			perror("SHELL");
			return;
		}
		struct dirent *fl;
		if(curr_dir)
		{
			while((fl=readdir(curr_dir))!=NULL)
			{
				if(fl->d_name[0]!='.' && fl->d_name[strlen(fl->d_name)-1]!='~')
					printf("%s ",fl->d_name);
			}
			printf("\n");
			closedir(curr_dir);
		}
		return;
	}
	if(strcmp(args[1],"-a")==0 && !flag)
	{
		DIR *curr_dir=opendir(Dt);
		if(curr_dir==NULL)
		{
			perror("SHELL");
			return;
		}
		struct dirent *fl;
		if(curr_dir)
		{
			while((fl=readdir(curr_dir))!=NULL)
			{
				printf("%s ",fl->d_name);
			}
			printf("\n");
			closedir(curr_dir);
		}
		return;
	}
	if(strcmp(args[1],"-l")==0 || strcmp(args[1],"-al")==0 || strcmp(args[1],"-la")==0 || flag)
	{
		DIR *curr_dir=opendir(Dt);
		if(curr_dir==NULL)
		{
			perror("SHELL");
			return;
		}
		struct dirent *fl;
		if(curr_dir)
		{
			while((fl=readdir(curr_dir))!=NULL)
			{
				if((fl->d_name[0]=='.' || fl->d_name[strlen(fl->d_name)-1]=='~' ) && strcmp(args[1],"-l")==0 && flag==0)
					continue;
				strcpy(FileName,Dt);
				strcat(FileName,"/");
				strcat(FileName,fl->d_name);
				print_ls_l(FileName);
				printf("%s\n",fl->d_name);
			}
			closedir(curr_dir);
		}
		return;
	}
	DIR *curr_dir=opendir(args[1]);
	if(curr_dir==NULL)
		{
			perror("SHELL");
			return;
		}
		struct dirent *fl;
		if(curr_dir)
		{
			while((fl=readdir(curr_dir))!=NULL)
			{
				if(fl->d_name[0]!='.' && fl->d_name[strlen(fl->d_name)-1]!='~')
					printf("%s ",fl->d_name);
			}
			printf("\n");
			closedir(curr_dir);
		}

}