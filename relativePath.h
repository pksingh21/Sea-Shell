#include "headers.h"
char* getpath(char* cwd,char* root)
{
	int i,cs=strlen(cwd),rs=strlen(root);
	if(cs==rs)
		return "~";
	if(rs>cs)
	{
		return cwd;
	}
	char* new=(char* )(malloc(100));
	for(i=0;i<rs;i++)
		if(cwd[i]!=root[i])
			return cwd;
	new[0]='~';
	new[1]='/';
	for(i=0;i<cs-rs-1;i++)
		new[i+2]=cwd[rs+i+1];
	return new;
}