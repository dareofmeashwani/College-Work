#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include <pwd.h>
#include <termios.h>
#include <fcntl.h>
int main (int argc, char **argv)
{
	char historystring[200];
	FILE *fph=fopen("history.txt","r");
	if(fph!=NULL)
	{
		while((fgets(historystring, 200, fph) != NULL))
		{
			printf("%s",historystring);
		}
	}
}
