#include "headers.h"
int f=0;
void handler(int fl)
{
	signal(SIGINT,handler);
	f++;
}
void clk(char tm[])
{
	f=0;
	signal(SIGINT,handler);
	char dte[1002],tme[1002];
	int t=atoi(tm);
	pid_t pid=fork();
	if(pid==0)
	{
		printf("\n");
		while(f==0)
		{
			FILE* pt=fopen("/proc/driver/rtc","r");
			while(1)
			{
				fscanf(pt,"%s",tme);
				if(tme[0]>='0' && tme[0]<='9')
					break;
			}
			while(1)
			{
				fscanf(pt,"%s",dte);
				if(dte[0]>='0' && dte[0]<='9')
					break;
			}
			printf("%s %s\n",dte,tme);
			sleep(t);
		}
		exit(0);
	}
}
