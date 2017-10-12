#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <string.h>
#include <pwd.h>
#include <termios.h>
#include <fcntl.h>
#include <error.h>
#include <dirent.h>
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
int ppid,number,localid,jobsize;
int shmkey,semrkey,semwkey,statuskey,globalid,filemutexkey;
char name[50];
int shmid,semrid,semwid,statusid,filemutexid;
struct timeval tv;
struct job *shmptr;
int *statusptr;
sem_t *mutexr,*filemutexptr,*mutexw;
struct job  
{
	int id;
	char name[100];
	int currenttask;
	char currenttaskname[100];
	struct slave_node *task;
	struct job *next;	
};
void* MapSharedMemory(int id)
{
	void* addr;
	addr = shmat(id, NULL, 0);
	return addr;
}
void allocateresources()
{
	char str[3][10];
	sprintf(str[0], "%d", semrkey);
	sprintf(str[1], "%d", semwkey);
	sprintf(str[2], "%d", filemutexkey);
	shmid=shmget(shmkey,2*sizeof(struct job),0644);
	shmptr=(struct job *)MapSharedMemory(shmid);
	statusid=shmget(statuskey,(3+number)*sizeof(int),0644);
	statusptr=(int *)MapSharedMemory(statusid);
	semrid=shmget(semrkey,sizeof(sem_t),0644);
	mutexr=(sem_t *)MapSharedMemory(semrid);
	semwid=shmget(semwkey,sizeof(sem_t),0644);
	mutexw=(sem_t *)MapSharedMemory(semwid);
	filemutexid=shmget(filemutexkey,sizeof(sem_t),0644);
	filemutexptr=(sem_t *)MapSharedMemory(filemutexid);
	//mutexr=sem_open(str[0], O_CREAT, 0600, 1);
	//mutexw=sem_open(str[1], O_CREAT, 0600, 1);
	//filemutexptr=sem_open("file",O_RDONLY);
}
void deallocateresources()
{
	char str[3][10];
	sprintf(str[0], "%d", semrkey);
	sprintf(str[1], "%d", semwkey);
	sprintf(str[2], "%d", filemutexkey);
	shmdt((void *)mutexr);
	shmdt((void *)mutexw);
	shmdt((void *)shmptr);
	shmdt((void *)statusptr);
	shmdt((void *)filemutexptr);
	/*sem_close(mutexr);
	sem_unlink(str[0]);
	sem_close(mutexw);
	sem_unlink(str[1]);
	sem_close(filemutexptr);
	sem_unlink(str[2]);*/
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
deallocateresources();
exit(0);
}
//name//number//parent pid//id//key1//key2//jobsize
int main(int argc,char *argv[])
{
	int pid=getpid();
	int i,waiting=0,c=0;
	char temp[200],temp1[20];
	strcpy(name,argv[0]);
	number=atoi(argv[1]);
	ppid=atoi(argv[2]);
	localid=atoi(argv[3]);
	shmkey=atoi(argv[4]);
	semrkey=atoi(argv[5]);
	semwkey=atoi(argv[6]);
	globalid=atoi(argv[7]);
	statuskey=atoi(argv[8]);
	filemutexkey=atoi(argv[9]);
	jobsize=atoi(argv[10]);
	if (signal(SIGUSR1,handler) == SIG_ERR)
        printf("can't catch SIGUSR1\n");
	gettimeofday(&tv, NULL);
	double st = tv.tv_sec * 1.0 + tv.tv_usec / 1000000.0;
	allocateresources();
	do
	{
		c=0;
		struct job *joblink = (struct job*) malloc(sizeof(struct job));
		statusptr[3+localid]=0;
		sem_wait(mutexr);
		while(statusptr[0]!=1){c++;}
		jobcopier(joblink,shmptr);
		waiting=statusptr[1];	
		statusptr[0]=0;
		sem_post(mutexr);
		statusptr[3+localid]=1;
		if(sem_wait(filemutexptr)<0)perror("sem_wait");
		if(c>10)
		{
			FILE *fp=fopen("result.txt","a+");
			strcpy(temp,joblink->name);
			strcat(temp," ");
			strcat(temp,name);
			strcat(temp,":");
			strcat(temp,joblink->currenttaskname);
			strcat(temp," ");
			sprintf(temp1, "%d", pid);
			strcat(temp,temp1);
			strcat(temp," ");
			sprintf(temp1, "%d", waiting);
			strcat(temp,temp1);
			strcat(temp," waiting");
			fputs(temp,fp);
			fputs("\n",fp);
			printf("%s\n",temp);
			fclose(fp);
			statusptr[3+localid]=2;	
		}
		if(sem_post(filemutexptr)<0) perror("sem_post");	
		sleep(joblink->currenttask/1000);
		if(sem_wait(filemutexptr)<0) perror("sem_wait");
			FILE *fp=fopen("result.txt","a+");
			strcpy(temp,joblink->name);
			strcat(temp," ");
			strcat(temp,name);
			strcat(temp,":");
			strcat(temp,joblink->currenttaskname);
			strcat(temp," ");
			sprintf(temp1, "%d", pid);
			strcat(temp,temp1);
			strcat(temp," ");
			sprintf(temp1, "%d", waiting);
			strcat(temp,temp1);
			strcat(temp," finished");
			printf("%s\n",temp);
			fputs(temp,fp);
			fputs("\n",fp);
			fclose(fp);;
		if(sem_post(filemutexptr)<0)perror("sem_post");	
		sem_wait(mutexw);
		statusptr[2]=0;
		jobcopier(shmptr+1,joblink);
		statusptr[3+localid]=3;
		while(statusptr[2]!=1);
		sem_post(mutexw);
		free(joblink);
	}
	while(1);
}
