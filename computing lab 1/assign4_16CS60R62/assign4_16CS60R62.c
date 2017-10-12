#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include <pwd.h>
int main (int argc, char **argv)
{
char currentdir[200];
char *line;
pid_t  pid;
char command[1000],argument[1000];
int i,c_end,status;
const char *homedir;
if ((homedir = getenv("HOME")) == NULL) {
    homedir = (*(getpwuid(getuid()))).pw_dir;
}
	strcpy(currentdir,homedir);
	do
	{
		
		line=NULL;
		printf("\n%s/>",currentdir);
    		int linelen;
		size_t bufsize=1024;
    		line = (char *)malloc(bufsize * sizeof(char));
    		if( line == NULL)
    		{
        		perror("Unable to allocate buffer");
        		exit(1);
    		}
    		linelen = getline(&line,&bufsize,stdin);
		c_end=0;
		for(i=0;i<linelen;i++)
 		{			
			if(line[i]==' ')break;
			if(linelen-1==i) line[i]='\0';
			c_end++;
 		}
		for(i=0;i<1000;i++){command[i]='\0',argument[i]='\0';}
		i=0;
		while(i<=c_end)
		{
			command[i]=*(line+i);
			i++;
		}
		command[i-1]='\0';
		while(i<=strlen(line))
		{
			argument[i-c_end-1]=*(line+i);
			i++;
		}
		//printf("line:%s:command:%s:argument:%s:c_end:%d:length:%d:",line,command,argument,c_end,linelen);
		free(line);
		if(strcmp(command,"mypwd")==0)		{pid=fork();if(pid==0){execlp("./mypwd.out", "mypwd",currentdir, NULL);exit(0);}}
		else if(strcmp(command,"mymkdir")==0)	{pid=fork();if(pid==0){execlp("./mymkdir.out", "mymkdir",currentdir,argument, NULL);exit(0);}}
		else if(strcmp(command,"mycd")==0)	{pid=fork();if(pid==0){execlp("./mycd.out", "mycd",currentdir,argument, NULL);exit(0);}}
		else if(strcmp(command,"myrm")==0)	{pid=fork();if(pid==0){execlp("./myrm", "myrm",currentdir,argument, NULL);exit(0);}}
		else if(strcmp(command,"mymv")==0)	{pid=fork();if(pid==0){execlp("./mymv.out", "mymv",currentdir,argument, NULL);exit(0);}}
		else if(strcmp(command,"myps")==0)	{pid=fork();if(pid==0){execlp("./myps.out", "myps",currentdir,argument, NULL);exit(0);}}
		else if(strcmp(command,"myls")==0)	{pid=fork();if(pid==0){execlp("./myls.out", "myls",currentdir,argument, NULL);exit(0);}}
		else if(strcmp(command,"mytail")==0)	{pid=fork();if(pid==0){execlp("./mytail.out", "myls",currentdir,argument, NULL);exit(0);}}
		else if(strcmp(command,"mysed")==0)	{pid=fork();if(pid==0){execlp("./mysed.out", "mysed",currentdir,argument, NULL);exit(0);}}
		else { /*strlwr(command);*/ if(strcmp(command,"exit")==0||line[0]=='\0') {} else printf("%s/> Command Not Found",currentdir);}		
		while(wait(&status)>0);
		FILE *fp=fopen("cdresult.txt","r");
		if(fp!=NULL)
		{
			char temp[200],ch;
			int j=0;
			while((ch=fgetc(fp))!=EOF)
				{temp[j]=ch;j++;}
			temp[j]='\0';
			close(fp);
			printf("%s",temp);
			remove("cdresult.txt");
			strcpy(currentdir,temp);
			printf("current:%s",currentdir);
		}
		else {}
	}
	while(strcmp(command,"exit")!=0);
return 0;
}
