#ifndef ____TRY_H
#define ____TRY_H

#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

#define null NULL


#ifndef RELEASE
  #define ____err (errno!=0 && _____err(__FILE__,__LINE__))
  #define ____cond (____stat==-1 || ____stat==0) && errno!=EAGAIN && ____err

  #define ____NON_STANDARD_ERROR 32000
  #define ____MSG_TAB_SIZE 1024
  
  extern __thread int ____stat;
  extern __thread bool ____stack_trace;
  extern __thread char ____msg[];
  
  void _____nth(int fd, int line);
  bool _____assert(bool cond, const char* msg);
  bool _____err(char *file, int line);
  
  #define ptry                 { ____stack_trace=false; ____stat=!(errno=(int)
  #define try                  { ____stack_trace=false; errno=0; ____stat=(int)(
  #define assert(cond,msg)     { if((!(cond)) && (_____assert(cond,msg)) && ____err)return false; }
  #define assertv(cond,msg)    { if((!(cond)) && (_____assert(cond,msg)) && ____err)return;       }
  #define assertc(cond,msg)    { if((!(cond)) && (_____assert(cond,msg)) && ____err);             }
  #define asserte(cond,msg)    { if((!(cond)) && (_____assert(cond,msg)) && ____err)exit(0);      } 
  #define $                    );if(____cond)return false; }
  #define $v                   );if(____cond)return false; }
  #define $c                   );if(____cond);             }
  #define $e                   );if(____cond)exit(1);      }
  #define $$                   printf("%s: %d\n",__FILE__, __LINE__);
#endif
  
  
#ifdef RELEASE  
  #define assert
  #define assertv
  #define assertc
  #define asserte
  #define ptry    
  #define try
  #define $
  #define $v
  #define $c
  #define $e
  #define $$
#endif


#endif
