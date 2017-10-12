#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include <pwd.h>
#include <termios.h>
#include <fcntl.h>
#include <error.h>
#include <dirent.h>
char *addr[50][2],*special[2];
int total=0,f;
int linecount=0;
int matchreg(char *exp, char *text)
{
int status=0;
linecount=0;
        do 
	{    	/* must look even if string is empty */
            	if (matchherereg(exp, text))
                {status=1;addr[linecount][0]=text;linecount++;
		if(f==total)
		{
			special[0]=text;
		}total++;
		}
        } while (*text++ != '\0');
        return status;
    }
int matchstarreg(int c, char *exp, char *text)
{
       	do 
	{    /* a * matches zero or more instances */
  	       	if (matchherereg(exp, text))
                return 1;
        } while (*text != '\0' && (*text++ == c || c == '.'));
        return 0;
}
int matchherereg(char *exp, char *text)
{
        if (exp[0] == '\0')
            	{addr[linecount][1]=text;if(f==total-1)special[1]=text;return 1;}
        if (exp[1] == '*')
            	return matchstarreg(exp[0], exp+2, text);
	if (exp[1] == '+')
            	return matchherereg(exp-1, text+1)||matchstarreg(exp[0], exp+2, text+1);
	if (exp[1] == '?')
            	return matchsquesreg(exp[0], exp+2, text);
        if (exp[0] == '$' && exp[1] == '\0')
            	return *text == '\0';
        if (*text!='\0' && (exp[0]=='.' || exp[0]==*text))
            	return matchherereg(exp+1, text+1);
        return 0;
}
int matchsplusreg(int c,char *exp,char *text)
{
	if (matchherereg(exp-1, text))//&&(*text++ == c || c == '.'))
	{
		do 
		{    /* a * matches zero or more instances */
            		if (matchherereg(exp, text))return 1;
        	} while (*text != '\0' && (*text++ == c || c == '.'));
        	return 0;
	}
	return 0;
}
int matchsquesreg(int c,char *exp,char *text)
{
       	if (matchherereg(exp, text))
        return 1;
	if (matchherereg(exp, text+1))
      	return 1;
	return 0;
}
void grepreg(char filenames[],char filenamed[],char pattern[],char replace[])
{
    	int status=0,i=0,exit1=0,j;
	FILE *fpr,*fpw;
    	char temp[500],line1[500],line2[500];
	int lendifference=strlen(replace)-strlen(pattern);
	fpr=fopen(filenames,"r");
	if(fpr==NULL)printf("Error opening in filename:%s:\n",filenames);
    	else
    	{
        while(fgets(line1, 500, fpr) != NULL)
   		{
			int length=strlen(line1);
			status=matchreg(pattern,line1);
			if(f==-1&&status==1)
			{	for (j=0;j<500;j++){line2[j]='\0';}
				strcpy(line2,line1);
				for(i=0;i<linecount;i++) 
				{
					for (j=0;j<500;j++){temp[j]='\0';}
					
					char *t=line1;
					int k=0;
					while(addr[i][1]!=t){t++;k++;}
					strncpy(temp,line2+k+2*i*(lendifference),length-k-1);
					strcpy(line2+k-strlen(pattern)+i*(lendifference),replace);
					strcpy(line2+k+i*(lendifference+1),temp);			
				}				
			}
			else if (f<=total&&status==1&&exit1==0)
			{
			exit1=1;
			strncpy(temp,special[0],special[1]-special[0]);
			}
			status=0;
   		}
		fclose(fpr);
		if(f==-1){strcpy(temp,"sed 's/");
		strcat(temp,pattern);strcat(temp,"/");strcat(temp,replace);strcat(temp,"/' ");strcat(temp,filenames);strcat(temp," > ");
		strcat(temp,filenamed);}
		else if(f>0){strcpy(temp,"sed 's/");strcat(temp,pattern);strcat(temp,"/");
		strcat(temp,replace);strcat(temp,"/");sprintf(line1, "%d",f);strcat(temp,line1);strcat(temp,"' ");strcat(temp,filenames);strcat(temp," > ");
		strcat(temp,filenamed);}
		i=fork();if(i==0){execlp("./abc.out",temp,NULL);exit(3);}
    	while(wait(&status)>0);
    	//remove("temp.txt");
    	}
	
}
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
void grep(char filename[],char pattern[])
{
    	int r=0,j=0;
	FILE *fpr,*fpw;
    	char line[500];
	fpr=fopen(filename,"r");
	fpw=fopen("temp.txt","w");
	if(fpr==NULL)printf("Error opening in filename:%s:\n",filename);
	else if(fpw==NULL)printf("Error opening in filename:temp.txt:\n");
    	else
    	{
        	while(fgets(line, 500, fpr) != NULL)
   		{

				if(match(pattern,line)==1) 
				{
					//printf("%s",line);
					fputs(line,fpw);
				}
   		}
		fclose(fpr);
		fclose(fpw);
    	}
	
}
void deletecontinous(int firstline,int lastline,char filename[],int invert)
{
    	int j=0;
	FILE *fpr,*fpw;
    	char line[500];
	fpr=fopen(filename,"r");
	fpw=fopen("result.txt","w");
	if(fpr==NULL)printf("Error opening in filename:%s:\n",filename);
	else if(fpw==NULL)printf("Error opening in filename:temp.txt:\n");
    	else
    	{
        	while(fgets(line, 500, fpr) != NULL)
   		{
				j++;
				if((j<firstline||j>lastline)&&invert==0)
					fputs(line,fpw);
				else if((j>=firstline&&j<=lastline)&&invert==1)
					fputs(line,fpw);
   		}
		fclose(fpr);
		fclose(fpw);
		remove(filename);
		rename ("result.txt",filename);
    	}
}
void main(int argc, char *argv[])
{
char command[500],regexop[200],regexpattern[200],regexreplace[200],flag[10],filename[50],temp[50];
int i=0,j,index=0,dot=0,firstline,lastline,linelist[100],n=0,flagnum=0;
strcpy(command,argv[2]);
	command[strlen(command)-1]='\0';
//strcpy(command,"[3]d b.c");
//strcpy(command,"[2,5,8]d b.c");
//strcpy(command,"[2...6]d b.c");
//strcpy(command,"\\[5...10]d b.c");
for(i=0;i<100;i++)linelist[i]=-1;
i=0;
  if(command[0]==39)
  {
	i=1;
	while(i<strlen(command))
	{
		if(command[i]=='/'&&command[i+1]=='s'&&command[i+2]=='/'){index=i;break;}
		i++;
	}
	if(i==strlen(command)-1||index==0) regexop[0]='\0';
	else{strncpy(regexop,command+1,index-1);regexop[index-1]='\0';}
	i=i+3;
	index=i;
	while(i<strlen(command))
	{
		if(command[i]=='/'){break;}
		i++;
	}
	strncpy(regexpattern,command+index,i-index);
	regexpattern[i-index]='\0';
	i=i+1;
	index=i;
	while(i<strlen(command))
	{
		if(command[i]=='/'){break;}
		i++;
	}
	strncpy(regexreplace,command+index,i-index);
	regexreplace[i-index]='\0';
	i=i+1;
	index=i;
	while(i<strlen(command))
	{
		if(command[i]==' '){break;}
		i++;
	}
	strncpy(flag,command+index,i-index-1);
	flag[i-index-3]='\0';
	if(strcmp(flag,"g")==0){flagnum=-1;f=flagnum;}
	else 
	{
	flagnum=atoi(flag);
	f=flagnum;
	}
	i=i+1;
	strncpy(filename,command+i,strlen(command)-i);
	filename[strlen(command)-i]='\0';
	if(regexop[0]!='\0') 
	{
		grep(filename,regexop);
		grepreg("temp.txt",filename,regexpattern,regexreplace);
	}
	else 
	{
	    grepreg(filename,"result.txt",regexpattern,regexreplace);
	    remove(filename);
	    rename ("result.txt",filename);
	}
	
	
  }
else if(command[0]=='[')
{	
	i=0;
	while(i<strlen(command))
	{
		if(command[i]=='.'&&command[i+1]=='.'&&command[i+2]=='.'){dot=1;break;}
		i++;
	}
	if(dot==1)
	{
		strncpy(temp,command+1,i-1);
		temp[i-1]='\0';
		firstline=atoi(temp);
		i=i+3;
		index=i;		
		while(i<strlen(command))
		{
			if(command[i]==']'&&command[i+1]=='d')break;
			i++;
		}
		strncpy(temp,command+index,i-index);
		temp[i-index]='\0';
		lastline=atoi(temp);
		i=i+3;
		index=i;
		strncpy(filename,command+index,strlen(command)-index);
		filename[strlen(command)-index]='\0';
		deletecontinous(firstline,lastline,filename,0);
	}
	else
	{
		i=1;
		index=i;
		n=0;
		while(1)
     		{			
	    		if(command[i]==']'&&command[i+1]=='d')
	    		{
				for(j=0;j<50;j++)temp[j]='\0';
				strncpy(temp,command+index,i-index);
				linelist[n]=atoi(temp);
				n++;
				break;
			}
			else if(command[i]==',') 
			{
				for(j=0;j<50;j++)temp[j]='\0';
				strncpy(temp,command+index,i-index);
				linelist[n]=atoi(temp);
				n++; 
				index=i+1; 
			}
			i++;
      		}
		index=i+3;
		strncpy(filename,command+index,strlen(command)-index);
		filename[strlen(command)-index]='\0';
		index=0;
		i=0;
		int delete=0;
		FILE *fpr,*fpw;
    		char line[500];
		fpr=fopen(filename,"r");
		fpw=fopen("result.txt","w");
		if(fpr==NULL)printf("Error: opening in filename:%s:\n",filename);
		else if(fpw==NULL)printf("Error: writing in filename:%s:\n",filename);
    		else
    		{
        		while(fgets(line, 500, fpr) != NULL)
   			{
				index++;
				delete=0;
				for(i=0;i<n;i++)if(index==linelist[i]){delete=1;break;}
				if(delete==0) fputs(line,fpw);
   			}
			fclose(fpr);
			fclose(fpw);
			remove(filename);
			rename ("result.txt",filename);
    		}
		
		
	}
}
else if(command[0]==92)
{
	while(i<strlen(command))
	{
		if(command[i]=='.'&&command[i+1]=='.'&&command[i+2]=='.'){dot=1;break;}
		i++;
	}
	if(dot==1)
	{
		strncpy(temp,command+2,i-1);
		temp[i-1]='\0';
		firstline=atoi(temp);
		i=i+3;
		index=i;		
		while(i<strlen(command))
		{
			if(command[i]==']'&&command[i+1]=='d')break;
			i++;
		}
		strncpy(temp,command+index,i-index);
		temp[i-index]='\0';
		lastline=atoi(temp);
		i=i+3;
		index=i;
		strncpy(filename,command+index,strlen(command)-index);
		filename[strlen(command)-index]='\0';
		deletecontinous(firstline,lastline,filename,1);
	}

}
else
{
	printf("\ncommand not found\n");exit(3);	
} 
}
