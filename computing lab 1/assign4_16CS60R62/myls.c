#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>
int main(int argc, char* argv[])
{
    	DIR *mydir;
    	struct dirent *myfile;
    	struct stat permissions;
	char argument[10];
	argument[0]=*(*(argv+2)+0);
	argument[1]=*(*(argv+2)+1);
	argument[2]='\0'; 							//////// parsing the arguments passed by a myshell.out
	if(strcmp(argument,"-l")==0)						//////// making decision based on the parameter passed
    	{
		char buf[512];
	    	mydir = opendir(argv[1]);					//////// openning a current directory
	    	while((myfile = readdir(mydir)) != NULL)			/////// reading a one by one file or directory
	    	{	
	    		sprintf(buf, "%s/%s", argv[1], myfile->d_name);
	        	stat(buf, &permissions);				/////// reading the access permissions on files and directory
			printf((S_ISDIR(permissions.st_mode))  ? "d" : "-");
		    	printf((permissions.st_mode & S_IRUSR) ? "r" : "-");
			printf((permissions.st_mode & S_IWUSR) ? "w" : "-");
    			printf((permissions.st_mode & S_IXUSR) ? "x" : "-");
    			printf((permissions.st_mode & S_IRGRP) ? "r" : "-");
    			printf((permissions.st_mode & S_IWGRP) ? "w" : "-");
    			printf((permissions.st_mode & S_IXGRP) ? "x" : "-");
    			printf((permissions.st_mode & S_IROTH) ? "r" : "-");
    			printf((permissions.st_mode & S_IWOTH) ? "w" : "-");
    			printf((permissions.st_mode & S_IXOTH) ? "x" : "-");
			struct group *grp=getgrgid(permissions.st_gid);		////// reading the group info
			struct passwd *user=getpwuid(permissions.st_uid);	////// reading the group info
			printf("  %12s    %12s  ",grp->gr_name,user->pw_name); 	
			printf("%10jd ",permissions.st_size);			////// reading the size of file and directory
    			struct tm *foo;
			foo = gmtime(&(permissions.st_mtime));			///// reading date and time
   			printf(" %d", foo->tm_year+1900);
    			printf(":%d", foo->tm_mon+1);
   			printf(":%d", foo->tm_mday);
    			printf("  %d", foo->tm_hour);
    			printf("'%d", foo->tm_min);
			printf("  %15s\n",myfile->d_name);
			
	        	
	    	}
	    	closedir(mydir);
	}
	else
	{
		char buf[512];
	    	mydir = opendir(argv[1]);
	    	while((myfile = readdir(mydir)) != NULL)			/////// reading a one by one file or directory
	    	{
	        	printf(" %s\n", myfile->d_name);			/////// printing the result one by one
	    	}
	    	closedir(mydir);
	}
}
