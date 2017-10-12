#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<pwd.h>
#include<string.h>
#include <dirent.h>
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
int main (int argc, char **argv)
{
	char argument[1000];
	int i,j;
	for(i=0;i<=strlen(argv[2]);i++)
	{
	argument[i]=*(*(argv+2)+i);
	}
	argument[i]='\0'; 
	char dirlist[100][100],dirpath[100][100];
	for(i=0;i<100;i++)for(j=0;j<100;j++){dirlist[i][j]='\0';dirpath[i][j]='\0';}
	int index=0,n=0;i=0;
	while(1)
 	{			
		if(argument[i]=='\0')
		{
		        strncpy(dirlist[n],argument+index,i-index);
			n++;
			break;
		}
		if(argument[i]==' ') 
		{
			strncpy(dirlist[n],argument+index,i-index);
			n++; 
			index=i+1; 
		}
		if(argument[i]=='/')
		{
			char temp[100];for(j=0;j<100;j++)temp[j]='\0';	
			strncpy(temp,argument+index,i-index);
			strcat(dirpath[n],temp);
			strcat(dirpath[n],"/");
			index=i+1; 
		}
		i++;
		
 	}
	for(i=0;i<n;i++)
	{
		if(dirpath[i][0]=='\0')
		{	
			char temp[100];for(j=0;j<100;j++)temp[j]='\0';
			strcat(temp,argv[1]);
			strcat(temp,dirpath[i]);
			strcat(temp,"//");
			strcat(temp,dirlist[i]);
			mkdir(temp, 0777);
		}
		else 
		{
			for(j=0;j<strlen(dirpath[i]);j++)
			{
				
				if(dirpath[i][j]=='/')
				{
					char temp[100],temp1[100];temp1[0]='\0';temp[0]='\0';
					strcat(temp,argv[1]);
					strncpy(temp1,dirpath[i],j);
					strcat(temp,"/");
					strcat(temp,temp1);
					printf("%s\n",temp);
					mkdir(temp, 0777);
				}
			}
			char tempe[100];for(j=0;j<100;j++)tempe[j]='\0';
			strcat(tempe,argv[1]);
			strcat(tempe,"/");
			strcat(tempe,dirpath[i]);			
			strcat(tempe,dirlist[i]);
			printf("%s\n",tempe);
			mkdir(tempe, 0777);
		}
	}
return 0;
}
