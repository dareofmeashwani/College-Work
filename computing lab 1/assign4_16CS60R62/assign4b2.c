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
 //////ref: https://www.cs.cf.ac.uk/Dave/C/node24.html
void sigproc(int signal);
 
void quitproc(int signals); 
 
int main()
{		 if(signal(SIGINT, sigproc)==SIG_ERR)
		  printf("can't catch SIGUSR1\n");
		 if(signal(SIGQUIT, quitproc)==SIG_ERR)
		  printf("can't catch SIGUSR1\n");
		 printf("ctrl-c disabled use ctrl-\\ to quit\n");
		 for(;;); /* infinite loop */
		 return 0;
}
 
void sigproc(int signals)
{ 		 signal(SIGINT, sigproc); /*  */
		 /* NOTE some versions of UNIX will reset signal to default
		 after each call. So for portability reset signal each time */
 
		 printf("you have pressed ctrl-c \n");
}
 
void quitproc(int signals)
{ 		 printf("ctrl-\\ pressed to quit\n");
		 exit(0); /* normal exit status */
}
