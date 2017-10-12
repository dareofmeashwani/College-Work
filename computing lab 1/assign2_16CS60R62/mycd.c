#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/stat.h>
#include<fcntl.h>
#include <errno.h>
int checkdir(char *directory)
{
	DIR* dir = opendir(directory);
	if (dir)
	{
	    	/* Directory exists. */
	    	closedir(dir);
		return 1;
	}
	else if (ENOENT == errno)
	{
	    	/* Directory does not exist. */
		return 0;
	}
	else
	{
	    /* opendir() failed for some other reason. */
		return -1;
	}
}
int main(int argc, char* argv[])
{
	char directory[200];
	int len;
	directory[0]='\0';
	strcat(directory,argv[1]);
	strcat(directory,"/");
	strcat(directory,argv[2]);
	len=strlen(directory);
	directory[len-1]='\0';     					/////// adjust a parsed argument
	if (checkdir(directory)==1) 
		{
			FILE *fp=fopen("cdresult.txt","w");
			fputs(directory,fp);
			close(fp);
		} 							/////// saving a directory result in file later it will use to update the currentdir to 									///////change directory
	else if (checkdir(directory)==0) 
		printf("Directory Not Exist");
	else if (checkdir(directory)==-1) 
		printf("ERROR: Directory Openning Error");
}
