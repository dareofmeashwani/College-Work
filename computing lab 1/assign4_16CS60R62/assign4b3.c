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
#include<signal.h>
void handler(int signal);
int main (int argc, char **argv)
{
    if (signal(SIGINT,handler) == SIG_ERR)
        printf("\ncan't catch SIGUSR1\n");
    for(;;)
    {
    
    }
}
void handler(int signal) 
{
    const char *signal_name;

    // Find out which signal we're handling
    switch (signal) 
    {
        case SIGINT:
	{
	  FILE *fp=fopen("temp.txt","r");
	  if(fp==NULL)
	  {
	    fp=fopen("temp.txt","w");
	    fputs("Interrupt",fp);
	    fclose(fp);
	    
	    
	  }
	  else 
	  {
	    char string[2000];
	    fgets(string,1000,fp);
	    fclose(fp);
	    fp=fopen("temp.txt","w");
	    if(strcmp(string,"Interrupt")==0){fputs("I am taking rest now",fp);fclose(fp);}
	    else if(strcmp(string,"I am taking rest now")==0){fclose(fp);exit(3);}
	    
	  }
	}
    }
}
