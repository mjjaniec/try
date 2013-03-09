#include "try.h"

__thread int  ____stat;
__thread bool ____stack_trace;
__thread char ____msg[____MSG_TAB_SIZE];

#define size 4096            // a la constant :)
void _____nth(int fd, int line){
   int stat, cline=1, i, j;
   bool nend=true, all=false;
   char buff[size+1];                   //+ one for eventually null.
   
   while(nend){
      stat=read(fd,buff,size);
      if(stat==-1)return;
      if(stat<size)nend=false,all=true;
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
            read(fd,buff+i,size-i);
            for(j=i+1; j<stat && buff[j]!='\n';++j);
            if(j<stat)buff[j]='\0';     
            else return;        //probably line is longer than 4096 or an error occurred.
         }
         else buff[stat]='\0';
      }
   }
   fprintf(stderr,"%s",buff+i);
}

#undef size

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

bool _____assert(bool cond,const char * msg){
  if(cond)return false;
  errno=____NON_STANDARD_ERROR; 
  strncpy(____msg,msg,____MSG_TAB_SIZE); 
  return true;
}
