#ifndef ____TRY_H
#define ____TRY_H

/**
 * Made by Michał Janiec
 * 
 * using
 *      try systemcall()$;
 *
 * POSIX don't give uniform way to notify about errors so:
 * $ is best way for most cases but there are no guarantee that it works properly.
 * If function error code is different than 0 or -1 then it WONT be CAUGHT, by try and $.
 * Moreover POSIX let functions set errno even if its executes successfully so proper value maybe misinterpreted.
 * 
 * Also you can declare RELEASE before including this file to erase all error checks.
 * 
 **/

#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

#define null NULL

#define ____err ___err(__FILE__,__LINE__)
#define ____cond (____stat==-1 || ____stat==0) && errno!=EAGAIN && ____err
#define err {if(____err)return false;}
#define ____NON_STANDARD_ERROR 32000
#define ____MSG_TAB_SIZE 1024

#ifndef RELEASE
   extern __thread int ____stat;
   extern __thread bool ____stack_trace;
   extern __thread char ____msg[];
   #define ptry                 { ____stack_trace=false; ____stat=!(errno=(int)
   #define try                  { ____stack_trace=false; errno=0; ____stat=(int)(
   #define assert(cond,msg)		{ if(!(cond)){errno=____NON_STANDARD_ERROR; strncpy(___msg,msg,____MSG_TAB_SIZE);}
   #define $                    );if(____cond)return false; }
   #define $c                   );if(____cond);             }
   #define $e                   );if(____cond)exit(1);      }
   #define $$ 			printf("%s: %d\n",__FILE__, __LINE__);
   
#endif
#ifdef RELEASE  
   #define assert
   #define ptry    
   #define try
   #define $
   #define $c
   #define $e
   #define $$
#endif

void ___nth(int fd, int line);
bool ___err(char *file, int line);

#endif
