#include "headers.h"
void printPromt(char* root)
{
	char hostname[1024],cwd[1024];
	hostname[1023]='\0';
	gethostname(hostname,1023);
	char* username=getenv("USER");
	getcwd(cwd,sizeof(cwd));
	char *path=getpath(cwd,root);
	printf("%s@%s:%s$ ",username,hostname,path);
}