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
	printf("Process Id:%d:\n",getpid());
    if (signal(SIGUSR1,handler) == SIG_ERR)
        printf("can't catch SIGUSR1\n");
    if (signal(SIGKILL,handler) == SIG_ERR)
	printf("can't catch SIGKILL\n");
    if (signal(SIGSTOP,handler) == SIG_ERR)
	printf("can't catch SIGSTOP\n");
    if (signal(SIGINT,handler) == SIG_ERR)
        printf("can't catch SIGUSR1\n");
    for(;;)
    {
    
    }
}
void handler(int signal) 
{
	if(signal== SIGUSR1)
	printf("\n SIGUSR1 Received            ");
	if(signal== SIGINT)
	exit(3);
}