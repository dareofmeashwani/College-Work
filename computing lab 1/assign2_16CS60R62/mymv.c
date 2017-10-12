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
int isFile(const char* name)
{
    	DIR* directory = opendir(name);
	if(directory != NULL)
    	{
     		closedir(directory);
     		return 0;
    	}
    	if(errno == ENOTDIR)
    	{
     		return 1;
    	}
	return -1;
}
void move_file(char *name,char *new_name) {

}
int main( int argc,char *argv[] )
{
	
	//DIR* dirs,dirt;
	char parameter[1000];
	strcpy(parameter,argv[2]);
	char source[1000],target[1000],buf[2],temps[1000],tempt[1000];
	int s_end=0,i,indi=-1;
	for(i=0;parameter[i]!=' ';i++)
 	{			
			if(parameter[i]==' ')break;
			s_end++;
 	}
	i=0;
	while(i<=s_end)
	{
	source[i]=parameter[i];
	i++;
	}
	source[i-1]='\0';
	i=0;
	while(i<=strlen(parameter)-s_end)
	{
	target[i]=*(parameter+s_end+1+i);
	i++;
	}
	target[i-3]='\0';
	temps[0]='\0';
	tempt[0]='\0';
	strcat(temps,argv[1]);
	strcat(temps,"/");
	strcat(temps,source);
	strcat(tempt,argv[1]);
	strcat(tempt,"/");
	strcat(tempt,target);
	if((isFile(tempt) == 1)&&(isFile(temps) == 1)) indi=1;
	else if((isFile(tempt) == 0)&&(isFile(temps) == 0)) indi=0;
	else { printf("ERROR: parameter problem:source:%s:target:%s:",source,target); exit(1);}
	

	if(indi==1)
	{/////////////file code
	move_file(temps,tempt);
	}
	else if(indi==0)
	{
	
	

	}
return 0;
}
