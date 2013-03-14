#include "try.h"
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

#define ____NON_STANDARD_ERROR 32000
#define ____MSG_TAB_SIZE 1024
#define ____SIZE 4096            


__thread int  ____stat;
__thread bool ____stack_trace;
__thread char ____msg[____MSG_TAB_SIZE];

void _____nth(int fd, int line){
   int stat, cline=1, i, j;
   bool nend=true, all=false;
   char buff[____SIZE+1];                   //+ one for eventually null.
   
   while(nend){
      stat=read(fd,buff,____SIZE);
      if(stat==-1)return;
      if(stat<____SIZE)nend=false,all=true;
      for(i=0; cline<line && i<stat; ++i)
         if(buff[i]=='\n')
            ++cline;
      if(cline==line)nend=false;      
   }
   
   if(cline==line){
      for(j=i+1; j<stat && buff[j]!='\n';++j);
      if(j<stat)buff[j]='\0';   //stopped because of end line.
      else{            
         if(!all){               //not all line in buff.
            buff[stat]='\0';
            strcpy(buff,buff+i);
            i=0;
            read(fd,buff+i,____SIZE-i);
            for(j=i+1; j<stat && buff[j]!='\n';++j);
            if(j<stat)buff[j]='\0';     
            else return;        //probably line is longer than 4096 or an error occurred.
         }
         else buff[stat]='\0';
      }
   }
   fprintf(stderr,"%s",buff+i);
}

bool _____err(char *file, int line){
  if(!errno)return false;
  int fd, __errno;   
  __errno=errno;
  
  if(!____stack_trace){
    fprintf(stderr,"\n   ERROR: %s [%6d] \n",getenv("_"),(int)getpid());
    errno=__errno;
    if(errno!=____NON_STANDARD_ERROR) perror("");
    else fprintf(stderr,"%s",____msg);
    fprintf(stderr,"\nerr at line: %s: %5d: ",file, line);
    fd=open(file,O_RDONLY);
    if(fd!=-1)_____nth(fd,line);
    fprintf(stderr,"\n");
    ____stack_trace=true;
  }else{
    fprintf(stderr,"called from: %s: %5d: ",file, line);
    fd=open(file,O_RDONLY);
    if(fd!=-1)_____nth(fd,line);
    fprintf(stderr,"\n");
  }
  
  errno=__errno;      
  return true;
}

bool _____assert(bool cond,...){
  if(cond)return false;
  
  const char * fmt;
  
  ____stack_trace=false;
  errno=____NON_STANDARD_ERROR; 
  va_list args;
  va_start(args,cond);
  fmt=va_arg(args,const char *);
  vsnprintf(____msg,____MSG_TAB_SIZE,fmt,args);
  va_end(args);
  return true;
}

#undef ____NON_STANDARD_ERROR
#undef ____MSG_TAB_SIZE
#undef ____SIZE         