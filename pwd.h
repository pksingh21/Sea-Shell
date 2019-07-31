#include "headers.h"
void Print_present()
{
	char cwd[1024];
	getcwd(cwd,sizeof(cwd));
	printf("%s\n",cwd );
}