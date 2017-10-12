#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/stat.h>
const char *getUserName(uid_t uid)
{

  struct passwd *pw = getpwuid(uid);
  if (pw)
  {
    return pw->pw_name;
  }

  return "";
}
void print_status(long pid,char *argument)
{
    	char path[100], lineN[100],lineS[100],lineU[100],name[50],state[50],user[50], *pn,*ps,*pu;
	int length=0;
    	FILE *fp;
	snprintf(path, 100, "/proc/%ld/status", pid);
	fp = fopen(path, "r");
    	if(fp==NULL)
        	return;
	while(fgets(lineN, 100, fp)) 
	{
        	if(strncmp(lineN, "Name:", 5) != 0)
            		continue;
        	pn = lineN + 6;
      		while(isspace(*pn)) ++pn;
  		
        	break;
    	}
	close(fp);
	fp = fopen(path, "r");
    	if(fp==NULL)
        	return;
	while(fgets(lineS, 100, fp)) 
	{
        	if(strncmp(lineS, "State:", 6) != 0)
            		continue;
        	ps = lineS + 7;
      		while(isspace(*ps)) ++ps;
  		
        	break;
    	}
	close(fp);
		fp = fopen(path, "r");
    	if(fp==NULL)
        	return;
	while(fgets(lineU, 100, fp)) 
	{
        	if(strncmp(lineU, "Uid:", 4) != 0)
            		continue;
        	pu = lineU + 5;
      		while(isspace(*pu)) ++pu;
  		
        	break;
    	}
	close(fp);
	length=strlen(pn);
	strcpy(name,pn);
	name[length-1]='\0';
	length=strlen(ps);
	strcpy(state,ps);
	state[length-1]='\0';
	strcpy(user,getUserName(atoi(pu)));
	struct passwd *p = getpwuid(getuid()); 
	if (strcmp(argument,"-a")!=0&&strcmp(p->pw_name,user)==0)
		printf("  %4u   %15s   %10s    %10s\n", pid, name,state,user);
	else if 
		(strcmp(argument,"-a")==0) printf("  %4u                     %15s   %10s    %10s\n", pid, name,state,user);   
}

int main (int argc, char **argv)
{
	DIR* proc = opendir("/proc/");
	struct dirent* ent;
	long pid;
	char argument[10];
	argument[0]=*(*(argv+2)+0);
	argument[1]=*(*(argv+2)+1);
	argument[2]='\0';
	if(proc == NULL) 
	{
    		perror("opendir(/proc)");
    		return 1;
	}
	printf(" Process Id      Name        State             User\n");
	while(ent = readdir(proc)) 
	{
		if(!isdigit(*ent->d_name))
	        	continue;
	    	pid = strtol(ent->d_name, NULL, 10);
		print_status(pid,argument);
	}
	closedir(proc);
	return 0;
}
