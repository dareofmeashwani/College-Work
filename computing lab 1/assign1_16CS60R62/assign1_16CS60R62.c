#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<strings.h>
#include<math.h>
void tostring(char *, int );                       ///// declaration of function which convert Integer to string
int indexoff(const char * text, const char toFind);///// declaration of function which find out the particular characters index
void copier(char target[],char source[]);          ///// declaration of function which generate the copy of source file in target filename 
/* Mergeresult function gets the data from different files which are sorted 
   and they  merge  their there  result  which are which are also sorted and
   saves their result into files                       */
int mergeresult(char *filename1,char *filename2,int lines,int *uni,int *inter)
{
	FILE *fp1=fopen(filename1,"r");
	if (fp1 == NULL) 
	{
		 printf("merge file 1 getting a error in opening\n");      ///// checking the file correctly opening or not
	}   
	FILE *fp2=fopen(filename2,"r");
	if (fp2 == NULL) 	
	{ 
		printf("merge file 2 getting a error in opening\n");       ///// checking the file correctly opening or not
	}   
	int i,l=0,exit=0,j,l1,l2;
	char str1[lines][100],str2[lines][100];
	int counter1[lines],counter2[lines];
	char temp[1000],lasttemp[1000];
	for(l=0;l<lines;l++){strcpy(str1[l],"");strcpy(str2[l],"");counter1[l]=0;counter2[l]=0;}
	l=0;l1=0,l2=0;*uni=0;*inter=0;
 	while(l<lines)                         	        ///// this loop to read the complete files into the array str1
 	{						
 		i=0;					
 		char ch;				
 	 	while(1)				///// here fetching only one string from txt files
 	 	{
 	 		ch=fgetc(fp1);
 	 		if(ch==EOF) 
			{
				exit=1;
				break;
			}
	 	 	temp[i]=ch;
 		 	if(temp[i]=='\n')
			{
				temp[i]='\0';
				break;
			}
 	 	i++;
 	 	}
		if (exit==1) break;                       ///// fetching of only one string done here
 	 	char sub[1000];
 	 	int position=indexoff(temp,':');
		int c=0;
		while (c < position) 
		{
 	     		sub[c] = temp[c];
 	     		c++;
 	  	}
 	  	sub[c] = '\0';
		counter1[l]=atoi(sub);	
		fputs("\n",fp2);     			      ///// extracting integer and word from string fetched from file and copying
		strncpy(sub, temp + position+1,strlen(temp)); ///// to str1 and counter1
		strcpy(str1[l],sub);
 		l++;
 	}
 	l=0,exit=0;
 	while(l<lines)                                       ///// this loop to read the complete files into the array str1
 	{
 		i=0;
 		char ch;
  		while(1)	                              ///// here fetching only one string from txt files
  		{
  			ch=fgetc(fp2);
  			if(ch==EOF) 
			{
				exit=1;break;
			}
  			temp[i]=ch;
  			if(temp[i]=='\n')
			{
				temp[i]='\0';break;
			}
  			i++;
  		}
		if (exit==1) break;                           ///// feaching of only one string done here
  		char sub[1000];	
		fputs("\n",fp2);
  		int position=indexoff(temp,':');
		int c=0;
		while (c < position) 
		{
      			sub[c] = temp[c];
	      		c++;
   		}
   		sub[c] = '\0';
		counter2[l]=atoi(sub); 			      ///// extracting integer and word from string fetched from file and copying
		strncpy(sub, temp + position+1,strlen(temp)); ///// to str2 and counter2
		strcpy(str2[l],sub);
 		l++;
	 }
	close(fp1);
	close(fp2);
	int count1=0,count2=0;
	remove(filename1);				      ///// removing old processed files
	remove(filename2);
	fp2=fopen(filename2,"w");	
	fputs("\n",fp2);
	l=0;
	for(i=0,j=0;i+j<lines*2&&counter1[i]!=0&&counter2[j]!=0;) ///// megreing starts here 
	{
		int r=strcmp(str1[i],str2[j]);
		if (r==0)
		{
			tostring(temp,counter1[i]+counter2[j]);
			fputs(temp,fp2);
			fputs(":",fp2);
			fputs(str1[i],fp2);
			fputs("\n",fp2);
			i++;
			j++;
			*uni=*uni+1;
			*inter=*inter+1;
		}
		else if(r>0)
		{
			tostring(temp,counter2[j]);
			fputs(temp,fp2);
			fputs(":",fp2);
			fputs(str2[j],fp2);
			fputs("\n",fp2);
			j++;
			*uni=*uni+1;
		}
		else
		{
			tostring(temp,counter1[i]);
			fputs(temp,fp2);
			fputs(":",fp2);
			fputs(str1[i],fp2);
			fputs("\n",fp2);
			i++;
			*uni=*uni+1;
		}
	}
	for(;counter1[i]!=0&&i<lines;i++)
	{
		tostring(temp,counter1[i]);
		fputs(temp,fp2);
		fputs(":",fp2);	fputs("\n",fp2);
		fputs(str1[i],fp2);
		fputs("\n",fp2);
		*uni=*uni+1;
	}
	for(;counter2[j]!=0&&j<lines;j++)
	{
		tostring(temp,counter2[j]);
		fputs(temp,fp2);
		fputs(":",fp2);
		fputs(str2[j],fp2);
		fputs("\n",fp2);
		*uni=*uni+1;
	}
close(fp2);
return *uni;
}
/* this function gets the data from different files which are sorted 
   and finds the union of thier words after that are result stored in files. */

int unionoflist(char *filename1,char *filename2,int lines,int *uni,int *inter)
{
	FILE *fp1=fopen(filename1,"r");
	if (fp1 == NULL) { printf("merge file 1 getting a error in opening\n");}  ///// checking the file correctly opening or not
	FILE *fp2=fopen(filename2,"r");
	if (fp2 == NULL) { printf("merge file 2 getting a error in opening\n");}  ///// checking the file correctly opening or not
	int i,l=0,exit=0,j,l1,l2;
	char str1[lines][100],str2[lines][100];
	int counter1[lines],counter2[lines];
	char temp[1000],lasttemp[1000];
	for(l=0;l<lines;l++){strcpy(str1[l],"");strcpy(str2[l],"");counter1[l]=0;counter2[l]=0;}
	l=0;l1=0,l2=0;*uni=0;*inter=0;
	while(l<lines)                         	  ///// this loop to read the complete files into the array str1
	{						
	 	i=0;					
	 	char ch;				
	  	while(1)				
	  	{
	  		ch=fgetc(fp1);
	  		if(ch==EOF) 
			{
				exit=1;
				break;
			}
		  	temp[i]=ch;
		  	if(temp[i]=='\n')
			{
				temp[i]='\0';
				break;
			}
		  	i++;
	  	}
		if (exit==1) break;                  ///// here fetching only one string from txt files
	  	char sub[1000];
	  	int position=indexoff(temp,':');
		int c=0;
		while (c < position) 
		{
	      		sub[c] = temp[c];
	      		c++;
	   	}
	   	sub[c] = '\0';
		counter1[l]=atoi(sub);
		strncpy(sub, temp + position+1,strlen(temp));
		strcpy(str1[l],sub);
	 	l++;
	 }
	 l=0,exit=0;
	 while(l<lines)                                       ///// this loop to read the complete files into the array str2
	 {
	 	i=0;
	 	char ch;
	  	while(1)	                              ///// here fetching only one string from txt files
	  	{
	  	ch=fgetc(fp2);
		  	if(ch==EOF) 
			{
				exit=1;
				break;
			}
		  	temp[i]=ch;
		  	if(temp[i]=='\n')
			{
				temp[i]='\0';
				break;
			}
			  	i++;
	  	}
		if (exit==1) break;                           ///// here feaching of string done
	  	char sub[1000];
	  	int position=indexoff(temp,':');
		int c=0;
		while (c < position) 
		{
	      		sub[c] = temp[c];
	      		c++;
	   	}
	   	sub[c] = '\0';
		counter2[l]=atoi(sub);
		strncpy(sub, temp + position+1,strlen(temp)); ///// extracting integer and word from string fetched from file and copying
		strcpy(str2[l],sub);			      ///// to str2 and counter2
	 	l++;
	 }
	close(fp1);
	close(fp2);
	int count1=0,count2=0;
	l=0;
	for(i=0,j=0;i+j<lines*2&&counter1[i]!=0&&counter2[j]!=0;) //// calculation to Union starts here using merge procedure of merge sort
	{
		int r=strcmp(str1[i],str2[j]);
		if (r==0)
		{
			i++;
			j++;
			*uni=*uni+1;
			*inter=*inter+1;
		}
		else if(r>0)
		{
			j++;
			*uni=*uni+1;
		}
		else
		{
			i++;
			*uni=*uni+1;
		}
	}
	for(;counter1[i]!=0&&i<lines;i++)
	{
		*uni=*uni+1;
	}
	for(;counter2[j]!=0&&j<lines;j++)
	{
		*uni=*uni+1;
	}
	return *uni;
}
/* this function gets the data from different files which are sorted 
   and finds the Intersection of thier words after that are result stored in files. */

int intersectionoflist(char *filename1,char *filename2,int lines,int *uni,int *inter)
{
	FILE *fp1=fopen(filename1,"r");
	if (fp1 == NULL) { printf("merge file 1 getting a error in opening\n");}
	FILE *fp2=fopen(filename2,"r");
	if (fp2 == NULL) { printf("merge file 2 getting a error in opening\n");}
	int i,l=0,exit=0,j,l1,l2;
	char str1[lines][100],str2[lines][100];
	int counter1[lines],counter2[lines];
	char temp[1000],lasttemp[1000];
	for(l=0;l<lines;l++){strcpy(str1[l],"");strcpy(str2[l],"");counter1[l]=0;counter2[l]=0;}
	l=0;l1=0,l2=0;*uni=0;*inter=0;
	 while(l<lines)                         	      ///// this loop to read the complete files into the array str1
	 {						
	 	i=0;					
	 	char ch;				
	  	while(1)				
	  	{
	  		ch=fgetc(fp1);
		  	if(ch==EOF) 
			{
				exit=1;
				break;
			}
		  	temp[i]=ch;
		  	if(temp[i]=='\n')
			{
				temp[i]='\0';
				break;
			  	i++;
		  	}
		}
		if (exit==1) break;			     ///// here fetching only one string from txt files
	  	char sub[1000];
	  	int position=indexoff(temp,':');	
		int c=0;
		while (c < position)
		{
      			sub[c] = temp[c];
      			c++;
   		}
	   	sub[c] = '\0';
		counter1[l]=atoi(sub);                       ///// extracting integer and word from string fetched from file and copying
		strncpy(sub, temp + position+1,strlen(temp));///// to str1 and counter1
		strcpy(str1[l],sub);
	 	l++;
	}
	l=0;
	exit=0;
	while(l<lines)                                       ///// this loop to read the complete files into the array str2
	{
	 	i=0;
	 	char ch;
	  	while(1)	                              ///// here fetching only one string from txt files
	  	{
	  	ch=fgetc(fp2);
	  	if(ch==EOF) 
		{
			exit=1;
			break;
		}
	  	temp[i]=ch;
	  	if(temp[i]=='\n')
		{
			temp[i]='\0';
			break;
		}
	  	i++;
	  	}
		if (exit==1) break;                           ///// here feaching of string done
	  	char sub[1000];
	  	int position=indexoff(temp,':');
		int c=0;
		while (c < position) 
		{
	      		sub[c] = temp[c];
	      		c++;
	   	}
	   	sub[c] = '\0';
		counter2[l]=atoi(sub);			      ///// extracting integer and word from string fetched from file and copying
		strncpy(sub, temp + position+1,strlen(temp)); ///// to str2 and counter2
		strcpy(str2[l],sub);
	 	l++;
	 }
	close(fp1);
	close(fp2);
	int count1=0,count2=0;
	l=0;
	for(i=0,j=0;i+j<lines*2&&counter1[i]!=0&&counter2[j]!=0;) ///// calculation for Intersection using merge procedure of merge sort
	{
		int r=strcmp(str1[i],str2[j]);
		if (r==0)
		{
			i++;
			j++;
			*uni=*uni+1;
			*inter=*inter+1;
		}
		else if(r>0)
		{
			j++;
			*uni=*uni+1;
		}
		else
		{
			i++;
			*uni=*uni+1;
		}
	}
	for(;counter1[i]!=0&&i<lines;i++)
	{
		*uni=*uni+1;
	}
	for(;counter2[j]!=0&&j<lines;j++)
	{	
		*uni=*uni+1;
	}
	close(fp2);
	return *inter;
}
/* this function extract specific protion of text files save into files
   extraction parameter are index which describe specific chunk of lines 
   and lines describe how many to have to extract   */
int splitsortfile(char *filename,int index,int lines)
{
	FILE *fp;
	char ch;
	char temp[1000];
	int t=0,exit=0,j,i;
	int count=(index-1)*lines;
	fp = fopen(filename, "r");
	char str[lines][1000];
	for(i=0;i<lines;i++) strcpy(str[i],"");
	while(1)	    					//////seeeking to correct location in file 
	{ 
		if(count==t)break;
		ch= fgetc(fp);	
		if(ch=='\n') t++;
	}
	t=0;count=0;
	char temp1[1000];
	for(j=0;j<1000;j++)temp1[j]='\0';
	tostring(temp1,index);
	strcat(temp1,"temp");
	strcat(temp1,filename);
	FILE *fpw = fopen(temp1, "w");
	while(t<lines)
	{
		char temp[1000];
		int i=0;
		while(1) 				 	////// lines reading starts from from file
		{
			temp[i]= fgetc(fp);
			if(temp[i]==EOF)
			{
				exit=1;
				break;
			}
			if(temp[i]=='\n')break;
			i++;
		}
		temp[i]='\0';					////// lines reading done here
		t++;
		if (exit==1) break;
		strcpy(str[t], temp);			        ////// copying words in array str 
	}
	close(fp);
	 for (i = 1; i < lines; i++) {				///// applying bubble sort to sort the words of files
	      for (j = 1; j < lines; j++) {
	         if (strcmp(str[j - 1], str[j]) > 0) {
	            strcpy(temp,str[j - 1]);
	            strcpy(str[j - 1], str[j]);
	            strcpy(str[j], temp);
	         }
	      }
	}
 	strcpy(temp,str[0]);
	char num[10];
	j=0;
	for(i=0;i<lines;i++)
	{
		if(strcmp(temp,str[i])==0)j++;
		else
		{
			tostring(num,j);
			fputs(num,fp);
			fputs(":",fp);
			fputs(temp,fp);
			fputs("\n",fp);
			strcpy(temp,str[i]);
			count++;
			j=1;
		}
	}
	close(fpw);
	return count;
}

/* this method actually convert the integer to string using few mathmatical calculation */

void tostring(char str[], int num)
{
int i,rem,len=0,n;
n=num;
while(n!=0)
{
	len++;
	n/=10;
	}
	for(i=0;i<len;i++)
	{
		rem=num%10;
		num=num/10;
		str[len-(i+1)]=rem+'0';
	}
	str[len]='\0';
}
/* this method finds the specific character occurence in string return its index value */
int indexoff(const char * text, const char toFind)
{
	int index=-1;
	int i,len;
	len = strlen(text);
	for(i=0;i<len;i++)
	{
		if(text[i]==toFind)
		{
		index=i;
		break;
		}
	}
	return index;
}
/*this method generate new copy of source file*/

int copy(char target[],char source[])
{ 
	int count=0;
	char ch;
	FILE *fp2= fopen(source, "r");
	FILE *fp1= fopen(target, "w");
	if (fp2 == NULL) { printf("file 2 getting a error in opening\n");}
	while( ( ch = fgetc(fp2) ) != EOF )
	{
		fputc(ch, fp1);
		if(ch=='\n') count++;
	}
	close(fp1);
	close(fp2);
	printf("exit");
	return count;   
}

int main(int argc, char *argv[])
{
	FILE *fp1,*fp2;
	int k,i,count1=0,count2=0,count3,count4,uni,inter,c=0,l1=0,l2=0; 
	float z;
	char ch;
	pid_t  pid1,pid2;
	char file1[100],file2[100],filename1[100],filename2[100];
	strcpy(filename1,argv[1]);
	strcpy(filename2,argv[2]);
	fp1 = fopen(filename1, "r");
	if (fp1 == NULL) { printf("file 1 getting a error in opening\n");} ///// checking the file correctly opening or not
	fp2 = fopen(filename2, "r");
	if (fp2 == NULL) { printf("file 2 getting a error in opening\n");} ///// checking the file correctly opening or not
	printf("\n\n\t What You Wanted To Do \n\t Press 1 For Sequencial \n \t Press 2 For Using fork System Call\n\t ::");
	int choice;
	scanf("%d",&choice);						   ///// reading a user input for menu
	if(choice==1)
	{
    		l1=copy("Original_file1.txt",filename1);
    		l2=copy("original_file2.txt",filename2);
   		int u1=splitsortfile(filename1,1,l1);
    		int u2=splitsortfile(filename2,1,l2);
    		uni=unionoflist("s_sort_1.txt","s_sort_2.txt",u1+u2,&uni,&inter);
    		inter=intersectionoflist("s_sort_1.txt","s_sort_2.txt",u1+u2,&uni,&inter);
		if(uni==0)z=-1; else z=inter/uni;
    		printf("\n\n\tSequentials Result::\n\n\tNo. Of Unique Words In File 1 : %d\n\n\tNo. Of Unique In Words File 2 : %d 			\n\n\tNo. Of Union in Both Files : %d\n\n\tNo. Of Intersection in Both Files : %d\n\n\tNo. Of Unique Words In File 1 : %f 			\n\n",u1-1,u2-1,uni,inter,z);
    		exit(0);
	}
	else if(choice==2){}
	else 
    	{ 
    		printf("\n\n\t ERROR: You Made A Wrong Choice\n\n");
    		exit(0);
    	}  
	do
	{
		printf("\nplease enter k value\n \t ::  ");
	scanf("%d",&k);
	}
	while (k>1000);
	int status;
	while (1) 
	{
		ch= fgetc(fp1);
	        if (ch == EOF) break;
	        if(ch=='\n') count1++;
	}								///// number of lines counted in first file
	while (1) 
	{
	        ch= fgetc(fp2);
	        if (ch == EOF) break;
        	if( ch=='\n') count2++;
    	}								///// number of lines counted in second file
	count3=count1/k+(count1 % k != 0);				///// here count3 and count4 describe how many lines we allot 
	count4=count2/k+(count2 % k != 0);				///// to child process.
	close(fp1);
	close(fp2);
	i=1;
	int n=k*2;
	int c1=count1,c2=count2;
	while(c1>0)							///// generate to k child process which will spilts the file in  
	{								///// k sub files each have a count3 lines and sort them
		pid1=fork();
		if(pid1==0)
		{
			splitsortfile(filename1,i,count3);	
			exit(0);
		}
		else 
		{
			c1=c1-count3;
			i++;
		}
	}	
	i=1;	
	while(c2>0)
	{	
		pid2=fork();						///// generate to k child process which will spilts the file in 
		if(pid2==0)						///// k sub files each have a count3 lines and sort
		{
			splitsortfile(filename2,i,count4);
			exit(0);
			}
		else 
		{
			c2=c2-count4;
			i++;
		}
	}		
	while (n > 0) 
	{
		  pid1 = wait(&status);					///// waiting for currently executing child processes 
		  --n;
	}	
	i=1;	
	while(i<k)
	{
		tostring(file1,i);
		strcat(file1,"temp");
		strcat(file1,filename1);
		tostring(file2,i+1);
		strcat(file2,"temp");
		strcat(file2,filename1);
		l1=mergeresult(file1,file2,count3,&uni,&inter);    	///// merge the k sorted files generate by k child process
		file1[0]='\0';file2[0]='\0';				///// of first file
		i++;
	}
	i=1;
	while(i<k)
	{	
		tostring(file1,i);
		strcat(file1,"temp");
		strcat(file1,filename2);
		tostring(file2,i+1);
		strcat(file2,"temp");
		strcat(file2,filename2);
		l2=mergeresult(file1,file2,count4,&uni,&inter);		///// merge the k sorted files generate by k child process
		file1[0]='\0';file2[0]='\0';				///// of first file
		i++;
	}
	tostring(file1,i);
	strcat(file1,"temp");
	strcat(file1,filename1);
	tostring(file2,i);
	strcat(file2,"temp");
	strcat(file2,filename2);
	uni=0;
	inter=0;
	uni=unionoflist(file1,file2,l1+l2,&uni,&inter);			///// after merging here we get the union  and intersection 
	inter=intersectionoflist(file1,file2,l1+l2,&uni,&inter);	///// of two different files
	if(uni==0)z=-1; else z=inter/uni;
	printf("\n\n\tFork System Call Result::\n\n\tNo. Of Unique Words In File 1 : %d\n\n\tNo. Of Unique In Words File 2 : %d 		\n\n\tNo. Of Union in Both Files : %d\n\n\tNo. Of Intersection in Both Files : %d \n\n\t No. Of Unique Words In File 1 : %f 		\n\n",l1,l2,uni,inter,z);
	return 0;
}

