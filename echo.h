#include "headers.h"
void Echo(char** args)
{
	for(int i=1;args[i]!=NULL;i++)
		printf("%s ",args[i]);
	printf("\n");
	return;
}