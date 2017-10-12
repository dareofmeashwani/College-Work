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
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <sys/sem.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
int lines=0;
int filemutexkey=-1;
int filemutexid;
int taskcount=0,taskdone=0,jobcount=0;
sem_t *filemutexptr;
struct job *processingqueue=NULL;;
struct job  
{
	int id;
	char name[100];
	int currenttask;
	char currenttaskname[100];
	struct slave_node *task;
	struct job *next;	
};
struct result
{
int pid;
int count;
struct slave_node *next;
};
struct slave_node  
{
	char name[100];
	char string[100];
   	int value;
	struct slave_node *next;
};
struct list  
{
	char name[100];
	int number;
	int id;
	int shmkey;
	int shmid;
	struct job *shmptr;
	int semrkey;
	int semrid;
	sem_t *mutexr;
	int semwkey;
	int semwid;
	sem_t *mutexw;
	int statuskey;
	int statusid;
	int *statusptr;
	struct slave_node *next;
	struct job *queue;
	int queuecount;
};

struct slave_node *insertFirst(struct slave_node *head,char name[],char string[], int value)
{
   	struct slave_node *link = (struct slave_node*) malloc(sizeof(struct slave_node));
   	strcpy(link->string,string);
	strcpy(link->name,name);
   	link->value = value;
   	link->next = head;
   	head = link;
	return head;
}
struct slave_node *insertlast(struct slave_node *head,char name[],char string[], int value)
{
	struct slave_node *new_node,*temp;
	new_node=(struct slave_node *)malloc(sizeof(struct slave_node));
	if(new_node == NULL)
	printf("nFailed to Allocate Memory");
 	new_node->next=NULL;
	strcpy(new_node->string,string);
	strcpy(new_node->name,name);
	new_node->value=value;
	if(head==NULL)
 	{
   		head=new_node;
 	}
 	else
 	{
   		temp = head;
     		while(temp->next!=NULL)
     		{
     		temp = temp->next;
     		}
   		temp->next = new_node;
 	}
	return head;
}
int covertdomain(struct list *datasheet,char name[],char key[])
{
	int i=0;
	int value=-1;
	for(i=0;i<lines;i++)
	{
	if(strcmp(name,datasheet[i].name)==0)break;
	}
	struct slave_node *temp=datasheet[i].next;
	while(temp!=NULL)
	{
	if(strcmp(temp->name,key)==0) {value=temp->value;break;}
	temp=temp->next;
	}
	return value;
}
int findjobqueue(struct list *datasheet,struct job *joblink)
{
	int i=0;
	for(i=0;i<lines;i++)
	{
	if(strcmp(joblink->task->name,datasheet[i].name)==0)break;
	}
	return i;
}
void taskmanager(struct result *rs,int mc,struct slave_node *task)
{
	struct slave_node *temp=rs[mc].next;
	rs[mc].count=rs[mc].count+1;
	while(temp!=NULL)
	{
	if(strcmp(temp->name,task->string)==0) {temp->value=temp->value+1;break;}
	temp=temp->next;
	}
}
void list_resetter(struct result *rs,int mc)
{
	int i;
	for(i=0;i<mc;i++)
	{
   		struct slave_node *ptr = rs[i].next;
   		while(ptr!= NULL)
		{        
      			strcpy(ptr->string,"");
			ptr->value=0;
      			ptr = ptr->next;
   		}
	}
}
void createrslist(struct result *rs,struct list *datasheet)
{
	int i,j,index=0;
	for(j=0;j<lines;j++)
	{	
		for(i=0;i<datasheet[j].number;i++)
		{
   			struct slave_node *ptr2 = datasheet[j].next;
			struct slave_node *ptr1=NULL;
   			while(ptr2!= NULL)
			{        
				ptr1=insertFirst(ptr1,ptr2->name,ptr2->string,ptr2->value);
				ptr2=ptr2->next;
			}
			rs[index].next=ptr1;
			index++;
   		}
	}

}
struct job *enqueue(struct job *head,struct job *joblink)
{
	struct job *temp;
	if(head==NULL)
 	{
   		head=joblink;
 	}
 	else
 	{
   		temp = head;
     		while(temp->next!=NULL)
     		{
     		temp = temp->next;
     		}
   		temp->next = joblink;
		joblink->next=NULL;
 	}
	return head;
}
uniquekeygenerator(struct list *datasheet)
{
	int i=0,j,n,status=0;
	n = rand() % 167;n = (n+rand()) % 43+10;n = (n+rand()) % 21+100;n = (n+rand()) % 33+500;n = (n+rand()) % 21+1000;
	n = (n+rand()) % 133+5000;n =(n+ rand() )% 9999;
	filemutexkey=n;
	for (i = 0; i <=lines; i++) 
	{	
	do{
		status=0;
		n = rand() % 167;n = (n+rand()) % 43+10;n = (n+rand()) % 21+100;n = (n+rand()) % 33+500;n = (n+rand()) % 21+1000;n = 			(n+rand()) % 133+5000;n =(n+ rand() )% 9999;
		for(j=0;j<i;j++)
		{
		if(datasheet[j].shmkey==n||filemutexkey==n)
		{status=1;break;}
		}
		if(status==0)
		{datasheet[j].shmkey=n;break;}
		
	}while(1);
	do{
		status=0;
		n = rand() % 167;n = (n+rand()) % 43+10;n = (n+rand()) % 21+100;n = (n+rand()) % 33+500;n = (n+rand()) % 21+1000;n = 			(n+rand()) % 133+5000;n =(n+ rand() )% 9999;
		for(j=0;j<i;j++)
		{
		if(datasheet[j].semrkey==n||datasheet[j].shmkey==n||filemutexkey==n)
		{status=1;break;}
		}
		if(status==0)
		{datasheet[j].semrkey=n;break;}
	}while(1);
	do{
		status=0;
		n = rand() % 167;n = (n+rand()) % 43+10;n = (n+rand()) % 21+100;n = (n+rand()) % 33+500;n = (n+rand()) % 21+1000;n = 			(n+rand()) % 133+5000;n =(n+ rand() )% 9999;
		for(j=0;j<i;j++)
		{
		if(datasheet[j].semwkey==n||datasheet[j].semrkey==n||datasheet[j].shmkey==n||filemutexkey==n)
		{status=1;break;}
		}
		if(status==0)
		{datasheet[j].semwkey=n;break;}
	}while(1);
	do{
		status=0;
		n = rand() % 167;n = (n+rand()) % 43+10;n = (n+rand()) % 21+100;n = (n+rand()) % 33+500;n = (n+rand()) % 21+1000;n = 			(n+rand()) % 133+5000;n =(n+ rand() )% 9999;
		for(j=0;j<i;j++)
		{
	       if(datasheet[j].statuskey==n||datasheet[j].semwkey==n||datasheet[j].semrkey==n||datasheet[j].shmkey==n||filemutexkey==n)	  			{status=1;break;}
		}
		if(status==0)
		{datasheet[j].statuskey=n;break;}
	}while(1);
				
	}
}
int get_job_discription(struct list *datasheet,char filename[])
{
	FILE *fp = fopen(filename,"r");
	char line[1000];
	int n=0;
	while(fgets(line, 1000, fp) != NULL)
	{
    		int i = 0,j,l=0,index=0;
    		char token[200][100];
		for(i=0;i<200;i++)for(j=0;j<100;j++)token[i][j]='\0';
		struct slave_node *head=NULL;
		i=0;
		while(1)
     		{			
	    		if(line[i]=='\n'||line[i]=='\0')
	    		{
				
				strncpy(token[l],line+index,i-index);
				token[i-1][strlen(token[i-1])-1]='\0';
				l++;
				break;
			}
			else if(line[i]==' ') 
			{
				strncpy(token[l],line+index,i-index);			
				l++; 
				index=i+1; 
			}
			i++;
      		}
		for (j = 2; j < l; j=j+2)
		{
			head=insertFirst(head,token[j],token[j+1],atoi(token[j+1]));			
		}
		strcpy(datasheet[n].name,token[0]);
		datasheet[n].number=atoi(token[1]);
		datasheet[n].next=head;
		datasheet[n].id=n;
		datasheet[n].queue=NULL;
		datasheet[n].shmkey=-1;
		datasheet[n].semrkey=-1;
		datasheet[n].semwkey=-1;
		datasheet[n].statuskey=-1;
		datasheet[n].queuecount=0;
		n++;
	}
	fclose(fp);
	return n;
}
void getjob(struct list *datasheet,char filename[])
{
	FILE *fp = fopen(filename,"r");
	char line[1000];
	int id=0;
	while(fgets(line, 1000, fp) != NULL)
	{
	
		int l=0,index=0,i,j;
    		char token[200][100];
		for(i=0;i<200;i++)for(j=0;j<100;j++)token[i][j]='\0';
		i=0;
		while(1)
     		{			
	    		if(line[i]=='\n'||line[i]=='\0')
	    		{
				
				strncpy(token[l],line+index,i-index);
				token[i-1][strlen(token[i-1])-1]='\0';
				l++;
				break;
			}
			else if(line[i]==' '||line[i]==':') 
			{
				strncpy(token[l],line+index,i-index);			
				l++; 
				index=i+1; 
			}
			i++;
      		}
		struct job *joblink = (struct job*) malloc(sizeof(struct job));
		strcpy(joblink->name,token[0]);
		joblink->id=jobcount;
		jobcount++;
		struct slave_node *head=NULL;
		for(i=1;i<l;i=i+2)
		{
			int x=covertdomain(datasheet,token[i],token[i+1]);
			head=insertlast(head,token[i],token[i+1],x);
			taskcount++;
		}
		joblink->task=head;
		joblink->currenttask=head->value;
		strcpy(joblink->currenttaskname,head->string);
		int x=findjobqueue(datasheet,joblink);
		datasheet[x].queue=enqueue(datasheet[x].queue,joblink);
	}
	fclose(fp);
}
void printList(struct slave_node *head)
{
   struct slave_node *ptr = head;
   while(ptr!= NULL)
	{        
      printf("(%s,%s,%d)\n",ptr->name,ptr->string,ptr->value);
      ptr = ptr->next;
   }
}
void writeList(struct result *rs,int mc)
{
	int j;
	FILE *fp=fopen("result.txt","a+");
	for(j=0;j<mc;j++)
	{
		char str[3][10];
		sprintf(str[0], "%d",rs[j].pid);
		fputs(str[0],fp);
		fputs("  ",fp);
		sprintf(str[1], "%d",rs[j].count);
		fputs(str[1],fp);
		fputs("  ",fp);
	   	struct slave_node *ptr = rs[j].next;
	   	while(ptr!= NULL)
		{        
	      		fputs(ptr->name,fp);
			sprintf(str[2], "%d",ptr->value);
			fputs(" ",fp);
			fputs(str[2],fp);
			fputs(" ",fp);
	      		ptr = ptr->next;
	   	}
		fputs("\n",fp);
	}
	fclose(fp);
}
void printqueue(struct job *head)
{
   struct job *ptr = head;
   while(ptr!= NULL)
	{        
      printf("(%s  %d %s:%d)\n",ptr->name,ptr->id,ptr->currenttaskname,ptr->currenttask);
      ptr = ptr->next;
   }
}
void updatequeuecount(struct list *datasheet)
{
	int i,count=0;
	for(i=0;i<lines;i++)
	{
   		struct job *ptr = datasheet[i].queue;
		count=0;
   		while(ptr!= NULL)
		{        
      			count++;
      			ptr = ptr->next;
		}
		datasheet[i].queuecount=count;
   	}
}
void* MapSharedMemory(int id)
{
	void* addr;
	addr = shmat(id, NULL, 0);
	return addr;
}
void destroySharedMemory(int id)
{
	shmctl(id, IPC_RMID, NULL);
}
void allocateresouces(struct list *datasheet)
{
	int i;
	for(i=0;i<lines;i++)
	{
		datasheet[i].shmid=shmget(datasheet[i].shmkey,2*sizeof(struct job),IPC_CREAT | 0666);
		datasheet[i].shmptr=(struct job *)MapSharedMemory(datasheet[i].shmid);
		datasheet[i].statusid=shmget(datasheet[i].statuskey,(3+datasheet[i].number)*sizeof(int),IPC_CREAT | 0666);
		datasheet[i].statusptr=(int *)MapSharedMemory(datasheet[i].statusid);
		datasheet[i].semrid=shmget(datasheet[i].semrkey,sizeof(sem_t),IPC_CREAT | 0666);
		datasheet[i].mutexr=(sem_t *)MapSharedMemory(datasheet[i].semrid);
		datasheet[i].semwid=shmget(datasheet[i].semwkey,sizeof(sem_t),IPC_CREAT | 0666);
		datasheet[i].mutexw=(sem_t *)MapSharedMemory(datasheet[i].semwid);
	}
	filemutexid=shmget(filemutexkey,sizeof(sem_t),IPC_CREAT | 0666);
	filemutexptr=(sem_t *)MapSharedMemory(filemutexid);

}
void deallocateresouces(struct list *datasheet)
{
	int i=0;
	char str1[10];
	sprintf(str1, "%d",filemutexid);
	for(i=0;i<lines;i++)
	{
		char str[2][10];
		sprintf(str[0], "%d",datasheet[i].semrid);
		sprintf(str[1], "%d",datasheet[i].semwid);
	    	shmdt((void *)datasheet[i].mutexr);
		shmdt((void *)datasheet[i].mutexw);
		shmdt((void *)datasheet[i].shmptr);
		shmdt((void *)datasheet[i].statusptr);
		destroySharedMemory(datasheet[i].shmid);
		destroySharedMemory(datasheet[i].semrid);
		destroySharedMemory(datasheet[i].semwid);
		destroySharedMemory(datasheet[i].statusid);
	}
	shmdt((void *)filemutexptr);
	destroySharedMemory(filemutexid);
}
void initializesharedmm(struct list *datasheet)
{
	int i,j;
	for(j=0;j<lines;j++)
	{
		datasheet[j].statusptr[0]=0;
		datasheet[j].statusptr[1]=0;
		datasheet[j].statusptr[2]=0;
		for(i=0;i<datasheet[j].number;i++)
		datasheet[j].statusptr[i+3]=0;
		
	}
	for(j=0;j<lines;j++)
	{
	    	sem_init(datasheet[j].mutexr,1, 1);
		sem_init(datasheet[j].mutexw,1, 1);
		if(datasheet[j].queue!=NULL)	
		{
			printf("insert   %d\n",taskdone+1);			
			jobcopier(datasheet[j].shmptr+0,datasheet[j].queue);
			struct job *temp=datasheet[j].queue;
			datasheet[j].queue=datasheet[j].queue->next;
			temp->next=NULL;
			datasheet[j].shmptr->next=NULL;						
			datasheet[j].queuecount--;			
			processingqueue=enqueue(processingqueue,temp);
			datasheet[j].statusptr[1]=datasheet[j].queuecount;
			datasheet[j].statusptr[0]=1;
			taskdone++;
		}
	}
	sem_init(filemutexptr,1, 1);
}
struct job *findremove(struct job *joblink)
{
	struct job *temp=processingqueue;
	struct job *p=NULL;
	struct job *q=NULL;
	//printf("hkhjkhkjbkjbkjbkjbkj(%s  %d %s:%d)\n",joblink->name,joblink->id,joblink->currenttaskname,joblink->currenttask);
  	while(temp!=NULL)
  	{
    		if(temp->id == joblink->id) 
		{
			if(p==NULL)
			{
				processingqueue=NULL;
				temp->next=NULL;
				return temp;
			}
			else if (temp->next==NULL)
			{
				p->next=NULL;
				return temp;
			}
			else
			{
				p->next=temp->next;
				temp->next=NULL;
				return temp;
			}
			return temp;
		}
		q=p;
		p=temp;
		temp = temp->next;
  	}
}
void covertvaluetotask(char *task,int value,int j,struct list *datasheet)
{
	struct slave_node *ptr = datasheet[j].next;
   	while(ptr!= NULL&&value!=ptr->value)
	{        
      	ptr = ptr->next;
   	}
	strcpy(task,ptr->string);
}
void jobcopier(struct job *t,struct job *s)
{
	t->id=s->id;
	strcpy(t->name,s->name);
	strcpy(t->currenttaskname,s->currenttaskname);
	t->currenttask=s->currenttask;
	t->task=s->task;
	t->next=t->next;
}
void handler(int signal) 
{
	if(signal== SIGINT)
	{
		exit(3);
	}
	if(signal== SIGUSR1)
	{
		exit(3);
	}
}
int main(int argc ,char *argv[])
{
	if(argc<4) exit(3);
	char filename1[50],filename2[50];
	int m=1;
	struct timeval tv;
	strcpy(filename1,argv[1]);
	strcpy(filename2,argv[2]);
	m=atoi(argv[3]);
	if (signal(SIGINT,handler) == SIG_ERR)
        printf("can't catch SIGUSR1\n");
    	if (signal(SIGUSR1,handler) == SIG_ERR)
        printf("can't catch SIGUSR1\n");
	FILE *fp = fopen(filename1,"r");
	int ch;
	int n=0,i,j,mc=0,index=0;
	int status;
	int ppid=getpid();
	char temp[250];
    	while ((ch = fgetc(fp)) != EOF)  
    	{
        	if(ch == '\n') lines++;   
    	}
	fclose(fp);
   	struct list datasheet[lines];
	n=get_job_discription(datasheet,filename1);
	if(n!=lines) printf("\nget_job_discrition parsed data incorrectly\n");
	for(i=0;i<m;i++)getjob(datasheet,filename2);
	for(i=0;i<lines;i++)mc=mc+datasheet[i].number;
	uniquekeygenerator(datasheet);
	updatequeuecount(datasheet);
	struct result rs[mc];
	allocateresouces(datasheet);
	initializesharedmm(datasheet);
	for(i=0;i<mc;i++){rs[i].next=NULL;rs[i].count=0;}
	createrslist(rs,datasheet);
	list_resetter(rs,mc);
	fp=fopen("result.txt","w");
	strcpy(temp,"./master ");
	strcat(temp," ");
	strcat(temp,filename1);
	strcat(temp," ");
	strcat(temp,filename2);
	strcat(temp," ");
	strcat(temp,argv[3]);
	fputs(temp,fp);
	fputs("\n",fp);
	if(sem_wait(filemutexptr)<0)perror("sem_wait");
	for(j=0;j<lines;j++)
	{
		for(i=0;i<datasheet[j].number;i++)
		{
			char str[11][10];
			sprintf(str[0], "%d", datasheet[j].number);
			sprintf(str[1], "%d", ppid);
			sprintf(str[2], "%d", i);
			sprintf(str[3], "%d", datasheet[j].shmkey);
			sprintf(str[4], "%d", datasheet[j].semrkey);
			sprintf(str[5], "%d", datasheet[j].semwkey);
			sprintf(str[6], "%d", index);
			sprintf(str[7], "%d", datasheet[j].statuskey);
			sprintf(str[8], "%d", filemutexkey);
			sprintf(str[9], "%d", sizeof(struct job));
			rs[index].pid=fork();
			if(rs[index].pid==0)
			{
			execl("child.out",datasheet[j].name,str[0],str[1],str[2],str[3],str[4],str[5],str[6],str[7],str[8],str[9],NULL);
			exit(3);
			}
			strcpy(temp,datasheet[j].name);
			strcat(temp,"  ");
			sprintf(str[11], "%d",rs[index].pid);
			strcat(temp,str[11]);
			fputs(temp,fp);
			fputs("\n",fp);
			index++;
		}
	}
	fclose(fp);
	if(sem_post(filemutexptr)<0)perror("sem_post");
	gettimeofday(&tv, NULL);
	int st2,st1 = tv.tv_sec * 1.0 + tv.tv_usec / 1000000.0;
	/*sleep(10);
	gettimeofday(&tv, NULL);
	st2 = tv.tv_sec * 1.0 + tv.tv_usec / 1000000.0;
	printf("%d",st2-st1);
	sleep(100);*/
	do
	{
		int count=0;
		for(j=0;j<lines;j++)
		{	
			for(i=0;i<datasheet[j].number;i++)
			{
				
				if(datasheet[j].statusptr[3+i]==3)
				{
					struct job *joblink;
					joblink=findremove(datasheet[j].shmptr+1);					
					datasheet[j].statusptr[3+i]==0;
					datasheet[j].statusptr[2]=1;					
					if(joblink!=NULL)
					{
						struct slave_node *temp=joblink->task;
						joblink->task=joblink->task->next;
						taskmanager(rs,count,temp);
						free(temp);
						if(joblink->task!=NULL)
						{					
						strcpy(joblink->currenttaskname,joblink->task->string);
						joblink->currenttask=joblink->task->value;
						int x=findjobqueue(datasheet,joblink);
						datasheet[x].queue=enqueue(datasheet[x].queue,joblink);
						datasheet[x].queuecount=datasheet[x].queuecount+1;
						}
						else free(joblink);
						
					}
				}
				count++;
			}
			if(datasheet[j].queue!=NULL&&datasheet[j].statusptr[0]==0)	
			{
			printf("insert   %d\n",taskdone+1);			
			jobcopier(datasheet[j].shmptr+0,datasheet[j].queue);
			struct job *temp=datasheet[j].queue;
			datasheet[j].queue=datasheet[j].queue->next;
			temp->next=NULL;
			datasheet[j].shmptr->next=NULL;						
			datasheet[j].queuecount--;			
			processingqueue=enqueue(processingqueue,temp);
			datasheet[j].statusptr[1]=datasheet[j].queuecount;
			datasheet[j].statusptr[0]=1;
			taskdone++;
			}
		}
	gettimeofday(&tv, NULL);
	st2 = tv.tv_sec * 1.0 + tv.tv_usec / 1000000.0;
	if((st2-st1)>=taskcount*.1)
	{
		system("clear");
		printf("\n\n\n\n\n\n\n\t\t\t\t DeadLock Detected\n\n\n\n\n");
		exit(3);	
	}
	}
	while(taskdone!=taskcount);
	sleep(0.5);
	for(j=0;j<mc;j++)
	{
	char str[10];
	sprintf(str,"%d",rs[j].pid);
	strcpy(temp,"kill ");
	strcat(temp,"-USR1 ");
	strcat(temp,str);
	system(temp);	
	}
	writeList(rs,mc);
	deallocateresouces(datasheet);
	
}
