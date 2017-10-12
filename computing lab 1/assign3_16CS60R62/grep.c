#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>
#include <string.h>
int match(char *exp, char *text)
{
	if (exp[0] == '^')	return matchhere(exp+1, text);
        do 
	{    	/* must look even if string is empty */
            	if (matchhere(exp, text))
                return 1;
        } while (*text++ != '\0');
        return 0;
    }
int matchstar(int c, char *exp, char *text)
{
       	do 
	{    /* a * matches zero or more instances */
  	       	if (matchhere(exp, text))
                return 1;
        } while (*text != '\0' && (*text++ == c || c == '.'));
        return 0;
}
int matchhere(char *exp, char *text)
{
        if (exp[0] == '\0')
            	return 1;
        if (exp[1] == '*')
            	return matchstar(exp[0], exp+2, text);
	if (exp[1] == '+')
            	return matchhere(exp-1, text+1)||matchstar(exp[0], exp+2, text+1);
	if (exp[1] == '?')
            	return matchsques(exp[0], exp+2, text);
        if (exp[0] == '$' && exp[1] == '\0')
            	return *text == '\0';
        if (*text!='\0' && (exp[0]=='.' || exp[0]==*text))
            	return matchhere(exp+1, text+1);
        return 0;
}
int matchsplus(int c,char *exp,char *text)
{
	if (matchhere(exp-1, text))//&&(*text++ == c || c == '.'))
	{
		do 
		{    /* a * matches zero or more instances */
            		if (matchhere(exp, text))return 1;
        	} while (*text != '\0' && (*text++ == c || c == '.'));
        	return 0;
	}
	return 0;
}
int matchsques(int c,char *exp,char *text)
{
       	if (matchhere(exp, text))
        return 1;
	if (matchhere(exp, text+1))
      	return 1;
	return 0;
}
void match_pattern(char filename[],char pattern[],int invert,int max)
{
    	int r=0,j=0;
	FILE *fp;
    	char temp,line[200];
	printf("\t \tResult Of Grep On File %s with Pattern %s \n",filename,pattern);
	fp=fopen(filename,"r");
	if(fp==NULL)printf("Error opening in filename:%s:\n",filename);
    	else
    	{
        	while((fgets(line, 200, fp) != NULL)&&(max>0||max==-1))
   		{
			if(invert==0)
			{
				if(match(pattern,line)==1) 
				{printf("%s",line);if(max!=-1)max--;}
			}
			else
			{
				if(match(pattern,line)!=1) 
				{printf("%s",line);if(max!=-1)max--;}
			}
   		}
    	}
}
int main(int argc,char *argv[])
{
    	struct stat stt;
    	char argument[100],option[15],value[10],pattern[50],filelist[50][50],files[200];
    	int num=-1,n=0,choice=0,invert=0;
	int i,c_end=2;
    	strcpy(argument,argv[2]);
	if (argument[strlen(argument) - 1] == '\n')
                        argument[strlen(argument) - 1] = '\0';
	for(i=0;i<15;i++)option[i]='\0';
	for(i=0;i<10;i++)value[i]='\0';
	for(i=0;i<200;i++)files[i]='\0';
	for(i=0;i<50;i++)pattern[i]='\0';
    	if(argument[0]=='-')
    	{
    		if(argument[1]=='-')
    		{
				for(i=2;i<strlen(argument);i++)					
 				{			
					if(argument[i]==':'||argument[i]=='=')break;
					c_end++;
 				}
 				
				strncpy(option,argument+2,c_end-2);
				if(strcmp(option,"invert-match")==0)
				{ 
				      	printf("option:%s:",option);
					for(i=c_end+2;i<strlen(argument);i++)					
				      	{			
						if(argument[i]==' ')break;
				      	}
				      	strncpy(pattern,argument+c_end+2,i-c_end-2);
					for(c_end=i+1;c_end<strlen(argument);c_end++)					
				      	{			
					  	if(argument[c_end]=='\0')break;
				      	}
				      	strncpy(files,argument+i+1,c_end-i-1);
					choice=1;
					invert=1;
				}
				else if(strcmp(option,"max-count")==0)
				{
				      	for(i=c_end+1;i<strlen(argument);i++)					
				      	{			
						if(argument[i]==' ')break;
				      	}
				      	strncpy(value,argument+c_end+1,i-c_end-1);
				      	num=atoi(value);
				      	for(c_end=i+1;c_end<strlen(argument);c_end++)					
				      	{			
					  	if(argument[c_end]==' ')break;
				      	}
				      	strncpy(pattern,argument+i+1,c_end-i-1);
				      
				       	for(i=c_end+1;i<strlen(argument);i++)					
				      	{			
					  	if(argument[i]=='\0')break;
				      	}
				      	strncpy(files,argument+c_end+1,i-c_end-1);
					choice=2;
					invert=0;
				}
				
				   			
    		}
    		else
    		{
    			if(argument[1]=='v')
			{
				for(c_end=3;c_end<strlen(argument);c_end++)					
				{			
				  	if(argument[c_end]==' ')break;
			      	}
				strncpy(pattern,argument+3,c_end-3);
				for(i=c_end+1;i<strlen(argument);i++)					
				{			
					if(argument[i]=='\0')break;
				}
				strncpy(files,argument+c_end+1,i-c_end-1);
				choice=1;
				invert=1;
			}
    			else if(argument[1]=='m')
			{
				for(i=4;i<strlen(argument);i++)					
			      	{			
					if(argument[i]==' ')break;
			      	}
			      	strncpy(value,argument+3,i-3);
			      	num=atoi(value);
			      	for(c_end=i+1;c_end<strlen(argument);c_end++)					
			      	{			
				  	if(argument[c_end]==' ')break;
			      	}
			      	strncpy(pattern,argument+i+1,c_end-i-1);
			      
			       	for(i=c_end+1;i<strlen(argument);i++)					
			      	{			
				  	if(argument[i]=='\0')break;
			      	}
			      	strncpy(files,argument+c_end+1,i-c_end-1);
				choice=2;
				invert=0;
			}
    		}
    		
    	}
    	else
    	{
		for(c_end=0;c_end<strlen(argument);c_end++)					
		{			
		  	if(argument[c_end]==' ')break;
	      	}
		strncpy(pattern,argument+0,c_end);
		for(i=c_end+1;i<strlen(argument);i++)					
		{			
			if(argument[i]=='\0')break;
		}
		strncpy(files,argument+c_end+1,i-c_end-1);
    		choice=3;
    	}
	c_end=0;i=0;n=0;
  	while(1)
     	{			
	    	if(files[i]=='\0')
	    	{
			strncpy(filelist[n],files+c_end,i-c_end);
			n++;
			break;
		}
		else if(files[i]==' ') 
		{
			strncpy(filelist[n],files+c_end,i-c_end);
			n++; 
			c_end=i+1; 
		}
		i++;
      	}
	i=0;
	while(i<n)
	{
		match_pattern(filelist[i],pattern,invert,num);
		i++;
	}
}
