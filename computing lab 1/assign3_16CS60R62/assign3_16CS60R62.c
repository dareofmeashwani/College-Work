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
//////this is custom shell designed by ashwani kumar verma
///// it's just implemented with few basic command
struct node
{
    char data[200];
    struct node* next;
};

struct node* push(struct node* head,char *data)
{
    	struct node* tmp = (struct node*)malloc(sizeof(struct node));
    	if(tmp != NULL)
    	{
    	strcpy(tmp->data,data);
    	tmp->next = head;
    	head = tmp;
    	return head;
	}
}
/*
    pop an element from the stack
*/
struct node* pop(struct node *head,char *element)
{
	
	if(head!=NULL)
	{
    	struct node* tmp = head;
	if(strcmp(head->data,"")==0||head->data[0]=='\0'){*(element+0)='\0';}
	else 
    	strcpy(element,head->data);
    	head = head->next;
    	free(tmp);
	return head;
	}
	return NULL;
}
struct node* mergestack(struct node *up,struct node *down)
{
	struct node *temp;
	while(down!=NULL)
	{
		temp=down;
		down=down->next;
		temp->next=up;
		up=temp;
	}
}
struct node* reverselist(struct node* head)
{
    struct node* prev   = NULL;
    struct node* current = head;
    struct node* next;
    while (current != NULL)
    {
        next  = current->next;  
        current->next = prev;   
        prev = current;
        current = next;
    }
    return prev;
}
writehistory(struct node *up)
{
	struct node *temp;
	FILE *fph=fopen("history.txt","w");
	while(up!=NULL)
	{
	if(strcmp(up->data,"")==0||up->data[0]=='\0'||up->data[0]=='\n'){}
	else 
	{	
		fputs(up->data,fph);
		//fputs("\n",fph);
	}
	temp=up;
	up=up->next;
	free(temp);
	}
	close(fph);
}
void display(struct node* head)
{
    struct node *current;
    current = head;
    if(current!= NULL)
    {
        printf("Stack: ");
        do
        {
            printf("%s ",current->data);
            current = current->next;
        }
        while (current!= NULL);
        printf("\n");
    }
    else
    {
        printf("The Stack is empty\n");
    }

}
int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}
pid_t proc_find(const char* name) 
{
    DIR* dir;
    struct dirent* ent;
    char buf[512];

    long  pid;
    char pname[100] = {0,};
    char state;
    FILE *fp=NULL; 

    if (!(dir = opendir("/proc"))) {
        perror("can't open /proc");
        return -1;
    }

    while((ent = readdir(dir)) != NULL) {
        long lpid = atol(ent->d_name);
        if(lpid < 0)
            continue;
        snprintf(buf, sizeof(buf), "/proc/%ld/stat", lpid);
        fp = fopen(buf, "r");

        if (fp) {
            if ( (fscanf(fp, "%ld (%[^)]) %c", &pid, pname, &state)) != 3 ){
                printf("fscanf failed \n");
                fclose(fp);
                closedir(dir);
                return -1; 
            }
            if (!strcmp(pname, name)) {
                fclose(fp);
                closedir(dir);
                return (pid_t)lpid;
            }
            fclose(fp);
        }
    }


closedir(dir);
return -1;
}
///// this is entry point of our shell
int main (int argc, char **argv)
{
char currentdir[200];
char line[1000],historystring[200],string[200],*linetemp;
pid_t  pid,backprocesslist[10];
struct node *up=NULL,*down=NULL;
char command[500],argument[500];
char commands[10][500],arguments[10][500];
int pipes[9][2];
int skip=0,i,c_end,status,backprocess=0,b,move=0,validcommand=0,processcounter=0,n,l,j;
const char *homedir;
if ((homedir = getenv("HOME")) == NULL) {                               ////// setting default directory to home directory
    homedir = (*(getpwuid(getuid()))).pw_dir;
}
for(i=0;i<10;i++)backprocesslist[i]=0;
FILE *fph=fopen("history.txt","r");
if(fph!=NULL)
{
while((fgets(historystring, 200, fph) != NULL))
{
if(strcmp(historystring,"")==0||historystring[0]=='\0'||historystring[0]=='\n'){}
else up=push(up,historystring);
}
close(fph);
//display(up);
}
	strcpy(currentdir,homedir);
	do
	{
		int ch0,ch1,ch2;
		char sub[5];sub[0]='\0';
		skip=0;
		//display(up);
		for(i=0;i<500;i++){command[i]='\0',argument[i]='\0';}
		for(i=0;i<10;i++)for(j=0;j<500;j++) commands[i][j]='\0';
		if (move==0) printf("\n%s/>",currentdir);
		ch0=getch();
		if(ch0==27)
		{
			ch1=getch();
			if(ch1==91)
			{
				
				ch2=getch();
				if(ch2==65)
				{
				char *temp=(char*)malloc(sizeof(sizeof(char)*200));
				up=pop(up,temp);
				if (temp[strlen(temp) - 1] == '\n')
                        	temp[strlen(temp) - 1] = '\0';
				strcpy(string,temp);
				if(strcmp(string,"")==0||string[0]=='\0'||string[0]=='\n'){}
				else{ 	down=push(down,string);
					printf("\r                                                               ");
					printf("\r%s/>%s",currentdir,string);
					}
				free(temp);
				}
				else if(ch2==66)
				{
				char *temp=(char*)malloc(sizeof(sizeof(char)*200));
				down=pop(down,temp);
				if (temp[strlen(temp) - 1] == '\n')
                        	temp[strlen(temp) - 1] = '\0';
				strcpy(string,temp);
				if(strcmp(string,"")==0||string[0]=='\0'||string[0]=='\n'){}
				else {
					up=push(up,string);
					printf("\r                                                               ");
					printf("\r%s/>%s",currentdir,string);
				}
				free(temp);
				}
				else skip=1;
				move=1;
			}
			else skip=1;
			
		}
		else {skip=1;}
		if (skip==1)
		{
			linetemp=NULL;
			skip=0;
	    		int linelen;
			size_t bufsize=1024;
			if(move==0&&ch0!=10)
			{	
				printf("%c",ch0);
				linetemp = (char *)malloc(bufsize * sizeof(char)); 
	    			linelen = getline(&linetemp,&bufsize,stdin);
				if( linetemp == NULL)
	    			{
	        			perror("Unable to allocate buffer");
	        			exit(1);
	    			}
				line[0]=ch0;line[1]='\0';
				strcat(line,linetemp);
				if (line[strlen(line) - 1] == '\n')
                        		line[strlen(line) - 1] = '\0';
				free(linetemp);
			}
			else if(move==1) {strcpy(line,string);move=0;}
			else {line[0]='\0';}
			/////////////////////////////////////////////////////////////////////
			i=0;
			c_end=0;
			l=0;
			while(1)
			{
				if(line[i]=='\0')
				{
				strncpy(commands[l],line+c_end,i-c_end);
				l++;
				break;
				}
				if(line[i]=='|')
				{
				strncpy(commands[l],line+c_end,i-c_end-1);
				l++;
				c_end=i+2;	
				}
				i++;
			}
			for(i=1;i<l;i++)
			{pipe(pipes[i-1]); printf("pipe running %d",i);}
			n=0;
			while(n<l)
			{
				b=0;
				for(i=0;i<strlen(commands[n]);i++)
				{	
					if(commands[n][i]=='&') {b=i;break;}
				}
				if(i>2)strncpy(sub,commands[n]+b-1,2);
			
				if(strcmp(sub," &")==0&&strlen(commands[n])-1==b)
				{
					commands[n][b-1]='\0';
					backprocess++;
				}
				c_end=0;
				for(i=0;i<strlen(commands[n]);i++)					
	 			{			
					if(commands[n][i]==' '||commands[n][i]=='\0')break;;
					c_end++;
	 			}
				strncpy(command,commands[n]+0,c_end);
				//command[i]='\0';					/////// command extracted here
				while(i<=strlen(commands[n]))
				{
					argument[i-c_end-2]=commands[n][i-1];
					i++;
				}
				char pc[5];
				if(l>1&&n==0)strcpy(pc,"f");
				else if(l-n>1)strcpy(pc,"p");
				else if(l>2)strcpy(pc,"l"); 
				else strcpy(pc,"n");
				
				
				///////// calling fork and taking a corresponding action of it
				
				if(strcmp(command,"mypwd")==0)
				{
					validcommand=1;
					pid=fork();
					if(pid==0)
					{	
					if(strcmp(pc,"f")==0){dup2(pipes[n][1], 1);close(pipes[n][0]);} 	
					else if(strcmp(pc,"p")==0){dup2(pipes[n-1][0],0);close(pipes[n-1][1]);dup2(pipes[n][1],1);close(pipes[n][0]);}
					else if(strcmp(pc,"l")==0){dup2(pipes[n-1][0],0);close(pipes[n-1][1]);}
					execlp("./mypwd.out","mypwd",currentdir,argument,pc,NULL);
					exit(3);
					}
				}			
				else if(strcmp(command,"mymkdir")==0)	
				{
					validcommand=1;
					pid=fork();
					if(pid==0)
					{
					if(strcmp(pc,"f")==0){dup2(pipes[n][1], 1);close(pipes[n][0]);} 	
					else if(strcmp(pc,"p")==0){dup2(pipes[n-1][0],0);close(pipes[n-1][1]);dup2(pipes[n][1],1);close(pipes[n][0]);}
					else if(strcmp(pc,"l")==0){dup2(pipes[n-1][0],0);close(pipes[n-1][1]);}
					execlp("./mymkdir.out","mymkdir",currentdir,argument,pc,NULL);
					exit(3);
					}
				}
				else if(strcmp(command,"mycd")==0)
				{	
					validcommand=1;
					pid=fork();
					if(pid==0)
					{
					if(strcmp(pc,"f")==0){dup2(pipes[n][1], 1);close(pipes[n][0]);} 	
					else if(strcmp(pc,"p")==0){dup2(pipes[n-1][0],0);close(pipes[n-1][1]);dup2(pipes[n][1],1);close(pipes[n][0]);}
					else if(strcmp(pc,"l")==0){dup2(pipes[n-1][0],0);close(pipes[n-1][1]);}
					execlp("./mycd.out","mycd",currentdir,argument,pc,NULL);
					exit(3);
					}
				}
				else if(strcmp(command,"myrm")==0)	
				{
					validcommand=1;
					pid=fork();
					if(pid==0)
					{
					if(strcmp(pc,"f")==0){dup2(pipes[n][1], 1);close(pipes[n][0]);} 	
					else if(strcmp(pc,"p")==0){dup2(pipes[n-1][0],0);close(pipes[n-1][1]);dup2(pipes[n][1],1);close(pipes[n][0]);}
					else if(strcmp(pc,"l")==0){dup2(pipes[n-1][0],0);close(pipes[n-1][1]);}					
					execlp("./myrm.out","myrm",currentdir,argument,pc,NULL);
					exit(3);
					}
				}
				else if(strcmp(command,"mymv")==0)	
				{
					validcommand=1;
					pid=fork();
					if(pid==0)
					{
					if(strcmp(pc,"f")==0){dup2(pipes[n][1], 1);close(pipes[n][0]);} 	
					else if(strcmp(pc,"p")==0){dup2(pipes[n-1][0],0);close(pipes[n-1][1]);dup2(pipes[n][1],1);close(pipes[n][0]);}
					else if(strcmp(pc,"l")==0){dup2(pipes[n-1][0],0);close(pipes[n-1][1]);}					
					execlp("./mymv.out","mymv",currentdir,argument,pc,NULL);
					exit(3);}
				}
				else if(strcmp(command,"myps")==0)	
				{printf("ps:%s:",pc);
					validcommand=1;
					pid=fork();
					if(pid==0)
					{
					if(strcmp(pc,"f")==0){dup2(pipes[n][1], 1);close(pipes[n][0]);} 	
					else if(strcmp(pc,"p")==0){dup2(pipes[n-1][0],0);close(pipes[n-1][1]);dup2(pipes[n][1],1);close(pipes[n][0]);}
					else if(strcmp(pc,"l")==0){dup2(pipes[n-1][0],0);close(pipes[n-1][1]);}					
					printf("ps");execlp("./myps.out","myps",currentdir,argument,pc,NULL);
					exit(3);
					}
				}
				else if(strcmp(command,"myls")==0)	
				{
					validcommand=1;
					pid=fork();
					if(pid==0)
					{
					if(strcmp(pc,"f")==0){dup2(pipes[n][1], 1);close(pipes[n][0]);} 	
					else if(strcmp(pc,"p")==0){dup2(pipes[n-1][0],0);close(pipes[n-1][1]);dup2(pipes[n][1],1);close(pipes[n][0]);}
					else if(strcmp(pc,"l")==0){dup2(pipes[n-1][0],0);close(pipes[n-1][1]);}					
					execlp("./myls.out","myls",currentdir,argument,pc,NULL);
					exit(3);
					}
				}
				else if(strcmp(command,"mytail")==0)	
				{
					validcommand=1;
					pid=fork();
					if(pid==0)
					{
					if(strcmp(pc,"f")==0){dup2(pipes[n][1], 1);close(pipes[n][0]);} 	
					else if(strcmp(pc,"p")==0){dup2(pipes[n-1][0],0);close(pipes[n-1][1]);dup2(pipes[n][1],1);close(pipes[n][0]);}
					else if(strcmp(pc,"l")==0){dup2(pipes[n-1][0],0);close(pipes[n-1][1]);}					
					execlp("./mytail.out","myls",currentdir,argument,pc,NULL);
					exit(3);
					}
				}
				else if(strcmp(command,"grep")==0)	
				{
					validcommand=1;
					pid=fork();
					if(pid==0)
					{
					if(strcmp(pc,"f")==0){dup2(pipes[n][1], 1);close(pipes[n][0]);} 	
					else if(strcmp(pc,"p")==0){dup2(pipes[n-1][0],0);close(pipes[n-1][1]);dup2(pipes[n][1],1);close(pipes[n][0]);}
					else if(strcmp(pc,"l")==0){dup2(pipes[n-1][0],0);close(pipes[n-1][1]);}					
					execlp("./grep.out","grep",currentdir,argument,pc,NULL);
					exit(3);
					}
				}
				else if(strcmp(command,"history")==0)	
				{
					pid=fork();
					if(pid==0)
					{
					if(strcmp(pc,"f")==0){dup2(pipes[n][1], 1);close(pipes[n][0]);} 	
					else if(strcmp(pc,"p")==0){dup2(pipes[n-1][0],0);close(pipes[n-1][1]);dup2(pipes[n][1],1);close(pipes[n][0]);}
					else if(strcmp(pc,"l")==0){dup2(pipes[n-1][0],0);close(pipes[n-1][1]);}					
					execlp("./history.out","history",currentdir,argument,pc,NULL);
					exit(3);
					}
				}
				else 
				{
					//setenv("PATH", currentdir, 1);
					if(strcmp(command,"exit")==0) validcommand=1;
					else if(command[0]=='\0'||strcmp(command,"")==0) {} 
					else 
					{
					if(strcmp(pc,"f")==0){dup2(pipes[n][1], 1);close(pipes[n][0]);} 	
					else if(strcmp(pc,"p")==0){dup2(pipes[n-1][0],0);close(pipes[n-1][1]);dup2(pipes[n][1],1);close(pipes[n][0]);}
					else if(strcmp(pc,"l")==0){dup2(pipes[n-1][0],0);close(pipes[n-1][1]);}
					printf("command:%s:line:%s:argument:%s:",command,commands[n],argument);	
					int x=system(command);
					if(x!=-1) validcommand=1;
					}
				}		
				if(strcmp(sub," &")!=0&&strcmp("n",pc)==0) while(wait(&status)>0);
				if(backprocess>0) 
				{
					char temp[20];
					struct stat sts;
					int c;
					backprocesslist[processcounter]=pid;
					processcounter++;
					//ckeck everyone and decrement background
					for(i=0;i<processcounter;i++)
					{
						sprintf(string, "%d",backprocesslist[i]);
						pid = proc_find(string);
						if(pid==-1)						
						{
							backprocess--;
							backprocesslist[i]=0;
							if(backprocesslist[i]==0)
							for(c_end=i;c_end<processcounter;c_end++)
							backprocesslist[c_end]=backprocesslist[c_end+1];
							backprocesslist[c_end+1]=0;
							c++;
						}
					}
					processcounter=processcounter-c;
				}
				FILE *fp=fopen("cdresult.txt","r");///////////// updating a change directory result
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
					strcpy(currentdir,temp);         /////// cd updated
					//printf("current:%s",currentdir);
				}	
				
			n++;
			}
			if(validcommand==1){up=mergestack(up,down);up=push(up,line);validcommand=0;}
			n=0;			
			while(l-n>1)						////// argument extracted here
			{
				close(pipes[n][0]);
				close(pipes[n][1]);
			}
		}
	}
	while(strcmp(command,"exit")!=0);
	up=reverselist(up);
	writehistory(up);
	if(backprocess>0){printf("\n\n\t\t Wainting Still Some Process Is Pending May Be In Background");while(wait(&status)>0);}
	
return 0;
}
