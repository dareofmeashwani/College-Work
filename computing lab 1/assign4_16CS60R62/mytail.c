#include <fcntl.h>
#include <stdio.h>
#define blocksize 4096
/*Tail remake written by Dan O'Boyle for Tom Limoncelli 6/13/2013*/
/*In short: This version of tail will read blocks 1 at a time until it finds at least 10 lines
It will then do its best to  print up to the last 10 lines of a file.*/

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
main (int argc, char *argv[])
{
	char argument[1000],ch,parameter[1000],temp[1000];
	FILE *fp;
	int sign=-1,i,lines=0,seekvalue;
	strcpy(argument,argv[2]);
	if (argc > 4 || argc < 1 )
	{
	      	printf("Parameter Problem: tail requires exactly 1 file");
	      	return 1;
	}
	int position=indexoff(argument,' ');
	
	if(argument[0]=='+'||argument[0]=='1'||argument[0]=='2'||argument[0]=='3'||argument[0]=='4'||argument[0]=='5'||argument[0]=='5'||argument[0]=='6'||
	argument[0]=='7'||argument[0]=='8'||argument[0]=='9'||argument[0]=='0') sign=0;
	else if(argument[0]=='-')sign=1;
	else 
	{
		printf("Parameter Problem: tail requires exactly 1 file");
	      	return 1;
	}
	//for(i=1;argument[i]!='\n'||argument[i]!='\0';i++){}
	i=0;
	char sub[10];
	while (i < position) 
	{
      		sub[i] = argument[sign+i];
      		i++;
   	}
	sub[i]='\0';
	seekvalue=atoi(sub);
	i=0;
	while (i<strlen(argument+position+1)) 
	{
      		parameter[i] = argument[position+i+1];
      		i++;
   	}
	parameter[i-1]='\0';
	seekvalue=atoi(sub);
	strcpy(temp,argv[1]);
	strcpy(temp,parameter);
	fp = fopen (parameter, "r");
	if (fp == NULL) 
	{
		printf("ERROR: file opening error");
		exit(0);      ///// checking the file correctly opening or not
	} 
	while (1) 
	{
		ch= fgetc(fp);
	        if (ch == EOF) break;
	        if(ch=='\n') lines++;
	}
	fseek (fp, 0, SEEK_SET);
	//printf ("parameter:%s:lines:%d:position:%d:parlength:%d:",parameter,lines,position,strlen(parameter));
	if(sign==1)
	{
		lines=lines-seekvalue;
		i=0;
		while(i<lines)	    					//////seeeking to correct location in file 
		{ 
			ch= fgetc(fp);	
			if(ch=='\n') i++;
		}
		while((ch= fgetc(fp))!=EOF)	    					//////seeeking to correct location in file  	
		printf("%c",ch);
	}
	else if(sign==0)
	{	
		i=0;	
		while(i<seekvalue)	    					//////seeeking to correct location in file 
		{ 
			ch= fgetc(fp);
			printf("%c",ch);	
			if(ch=='\n') i++;
		}
		
	} 
	close(fp);
}
