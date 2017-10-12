#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<strings.h>
#include<math.h>
#define maxstringlen 1000
void tostring(char *, int );                       ///// declaration of function which convert Integer to string
int indexoff(const char * text, const char toFind);///// declaration of function which find out the particular characters index
void copier(char target[],char source[]);          ///// declaration of function which generate the copy of source file in target filename
int uni=0,inter=0;
/* this function extract specific protion of text files save into files
   extraction parameter are index which describe specific chunk of lines
   and lines describe how many to have to extract   */
void split_file(char *filename,char *output,int index,int chunksize)
{
    int start=(index-1)*chunksize+1;
    int end=(index)*chunksize;
    extract_file_chunk(filename,output,start,end);
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
    fclose(fp1);
    fclose(fp2);
    printf("exit");
    return count;
}
int extract_file_chunk(char *source,char *output,int to,int from)
{
    FILE *fr=fopen(source,"r");
    FILE *fw=fopen(output,"w");
    int  i=1;
    char line[maxstringlen];
    while(fgets(line, maxstringlen, fr) != NULL&&i<=from)
    {
        if(i>=to)
            fputs(line,fw);
        i++;
    }
    fclose(fr);
    fclose(fw);
}
int sort_file(char *source,char *output)
{
    FILE *fr=fopen(source,"r");
    FILE *fw=fopen(output,"w");
    char line[maxstringlen];
    int l=0,i=0,j;
    while(fgets(line, maxstringlen, fr) != NULL) l++;
    fclose(fr);
    fr=fopen(source,"r");
    char str[l][maxstringlen];
    char count[l];
    while(fgets(line, maxstringlen, fr) != NULL)
    {
        line[strlen(line)-1]='\0';
        strcpy(str[i++],line);
    }
    for (i = 1; i < l; i++) {				///// applying bubble sort to sort the words of files
        for (j = 1; j < l; j++) {
            if (strcmp(str[j - 1], str[j]) > 0) {
                strcpy(line,str[j - 1]);
                strcpy(str[j - 1], str[j]);
                strcpy(str[j], line);
            }
        }
    }
   // for(i=0;i<l;i++) {
   //     fputs(str[i],fw);
   //     fputs("\n",fw);
   // }
    j=0;
    count[0]=1;
    for(i=1;i<l;i++)
    {
        if(strcmp(str[j],str[i])==0)
        {
           count[j]++;
        }
        else
        {
            j++;
            strcpy(str[j],str[i]);
            count[j]=1;
        }
    }
    for(i=0;i<j;i++) {
        char temp[maxstringlen+10];
        sprintf(temp,"%d:%s\n",count[i],str[i]);
        fputs(temp,fw);
    }
    fclose(fr);
    fclose(fw);
    return j;
}
int merge_file(char *filename1,char *filename2,char *output)
{
    uni=0;
    inter=0;
    FILE *fr1=fopen(filename1,"r");
    if (fr1 == NULL) printf("merge file 1 getting a error in opening\n");      ///// checking the file correctly opening or not
    FILE *fr2=fopen(filename2,"r");
    if (fr2 == NULL) printf("merge file 2 getting a error in opening\n");       ///// checking the file correctly opening or not
    FILE *fw=fopen(output,"w");
    char line[maxstringlen];
    int l1=0,l2=0,i=0,j=0;
    while(fgets(line, maxstringlen, fr1) != NULL) l1++;
    while(fgets(line, maxstringlen, fr2) != NULL) l2++;
    fclose(fr1);
    fclose(fr2);
    fr1=fopen(filename1,"r");
    fr2=fopen(filename2,"r");
    char str1[l1][maxstringlen],str2[l2][maxstringlen];
    int count1[l1],count2[l2];
    char temp[10];
    while(fgets(line, maxstringlen, fr1) != NULL)
    {
        line[strlen(line)-1]='\0';
        int position=indexoff(line,':');
        strncpy(str1[i],line+position+1,strlen(line)-position+1);
        strncpy(temp,line,position);
        temp[position]='\0';
        count1[i]=atoi(temp);
        i++;
    }
    i=0;
    while(fgets(line, maxstringlen, fr2) != NULL)
    {
        line[strlen(line)-1]='\0';
        int position=indexoff(line,':');
        strncpy(str2[i],line+position+1,strlen(line)-position+1);
        strncpy(temp,line,position);
        temp[position]='\0';
        count2[i]=atoi(temp);
        //printf("  :%s:    :%s:    %d    %d\n",str1[i],str2[i],count1[i],count2[i]);
        i++;
    }
    for(i=0,j=0;j<l2&&i<l1;) ///// megreing starts here
    {

        int r=strcmp(str1[i],str2[j]);
        printf("%d\n",r);
        if (r==0)
        {
            sprintf(temp,"%d",count1[i]+count2[j]);
            fputs(temp,fw);
            fputs(":",fw);
            fputs(str1[i],fw);
            fputs("\n",fw);
            i++;
            j++;
            inter++;
            uni++;
        }
        else if(r<0)
        {
            sprintf(temp,"%d",count1[i]);
            fputs(temp,fw);
            fputs(":",fw);
            fputs(str1[i],fw);
            fputs("\n",fw);
            i++;
            uni++;
        }
        else
        {
            sprintf(temp,"%d",count2[j]);
            fputs(temp,fw);
            fputs(":",fw);
            fputs(str2[j],fw);
            fputs("\n",fw);
            j++;
            uni++;
        }
    }
    for(;count1[i]!=0&&i<l1;i++)
    {
        sprintf(temp,"%d",count1[i]);
        fputs(temp,fw);
        fputs(":",fw);
        fputs(str1[i],fw);
        fputs("\n",fw);
        uni++;
    }
    for(;count2[j]!=0&&j<l2;j++)
    {
        sprintf(temp,"%d",count2[j]);
        fputs(temp,fw);
        fputs(":",fw);
        fputs(str2[j],fw);
        fputs("\n",fw);
        uni++;
    }
    fclose(fw);
    fclose(fr1);
    fclose(fr2);
}
int main(int argc, char *argv[])
{
    //split_file("sam.csv","split.txt",2,50);
    //sort_file("split.txt","sort.txt");
    //merge_file("sort.txt","sort.txt","merge.txt");
    FILE *fp1,*fp2;
    int k,i,count1=0,count2=0,count3,count4,c=0,l1=0,l2=0;
    float z;
    char ch;
    pid_t  pid1,pid2;
    char file1[100],file2[100],filename1[1000],filename2[1000];
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
        l1=copy("backup_file1.txt",filename1);
        l2=copy("backup_file2.txt",filename2);
        int unique_word1=sort_file(filename1,"sort_output1");
        int unique_word2=sort_file(filename2,"sort_output2");
        merge_file("sort_output1","sort_output2","merge_output");
        if(uni==0) z=-1; else z=inter/uni;
        printf("\n\n\tSequentials Result::\n\n\tNo. Of Unique Words In File 1 : %d\n"
                       "\n\tNo. Of Unique In Words File 2 : %d \n\n\tNo. Of Union in Both Files : %d"
                       "\n\n\tNo. Of Intersection in Both Files : %d\n\n\tjacard coffiecient : %f 			\n\n",
               unique_word1,unique_word2,uni,inter,z);
        exit(0);
    }
    else if(choice==2)
    {/*
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
        fclose(fp1);
        fclose(fp2);
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
    */}
    else
    {
        printf("\n\n\t ERROR: You Made A Wrong Choice\n\n");
        exit(0);
    }
    return 0;
}
