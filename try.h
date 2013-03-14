/**
 * Michał Janiec
 * 
 * Library aim is to mimic modern languages error handling in C/Linux environment.
 * 
 * It offers constructions:
 * 
 * try syscall() $;
 * assert(cond,"message");
 * 
 * It tries to mimic following Java code, accordingly:
 * try{ syscall() } catch(Exception e){ e.PrintStackTrace(); }
 * 
 * above constructions covers 80% of uses, but library offers quiet variety modifications:
 * 
 *   * ptry instead of try 
 *        to handle pthread_* functions. 
 *        It is because of different way of reporting errors in this function family
 *   * $c instead of $  
 *        It only reports error and continues execution;
 *        It's close to Java: catch(Exception e){e.PrintMessage(); } 
 *        Stack trace can't be printed because of technical reasons
 *   * $v instead of $
 *        It has to be used in void-returning functions (technical reason)
 *   * $e 
 *        Mimics following code: catch(Exception e){e.PrintMessage(); exit(); }
 * 
 *   * assertc - as assert but do not "throw an exception" and only print message instead
 *   * assertv - as assert but to use in void-returning functions
 *   * asserte - print message and immediately exit program
 * 
 * 
 *  All error checks can be removed by defining macro TRY_RELEASE
 * Be carreful because asserts would be completely removed
 * If you want to keep side effects of asserts you can use TRY_RELEASE_SIDE_EFFECTS
 */


#ifndef ____TRY_H
#define ____TRY_H

#define null NULL

#ifndef TRY_RELEASE
#ifndef TRY_RELEASE_SIDE_EFFECTS
  #include <stdbool.h>
  
  #define ____err (errno!=0 && _____err(__FILE__,__LINE__))
  #define ____cond (____stat==-1 || ____stat==0) && errno!=EAGAIN && ____err
  
  extern __thread int ____stat;
  extern __thread bool ____stack_trace;
  extern __thread char ____msg[];
  
  void _____nth(int fd, int line);
  bool _____assert(bool cond,...);
  bool _____err(char *file, int line);
  
  #define ptry                 { ____stack_trace=false; ____stat=!(errno=(int)
  #define try                  { ____stack_trace=false; errno=0; ____stat=(int)(
  #define assert(cond,...)     { bool ___cond_value=cond; if((!___cond_value) && (_____assert(___cond_value,__VA_ARGS__)) && ____err)return false; }
  #define assertv(cond,...)    { bool ___cond_value=cond; if((!___cond_value) && (_____assert(___cond_value,__VA_ARGS__)) && ____err)return;       }
  #define assertc(cond,...)    { bool ___cond_value=cond; if((!___cond_value) && (_____assert(___cond_value,__VA_ARGS__)) && ____err);             }
  #define asserte(cond,...)    { bool ___cond_value=cond; if((!___cond_value) && (_____assert(___cond_value,__VA_ARGS__)) && ____err)exit(0);      } 
  #define $                    );if(____cond)return false; }
  #define $v                   );if(____cond)return false; }
  #define $c                   );if(____cond);             }
  #define $e                   );if(____cond)exit(1);      }
  #define $$                   printf("%s: %d\n",__FILE__, __LINE__);
#endif
#endif
  
  
#ifdef TRY_RELEASE  
  #define assert(cond,msg)
  #define assertv(cond,msg)
  #define assertc(cond,msg)
  #define asserte(cond,msg)
  #define ptry    
  #define try
  #define $
  #define $v
  #define $c
  #define $e
  #define $$
#endif
  
#ifdef TRY_RELEASE_SIDE_EFFECTS
  #define assert(cond,msg)  (cond);
  #define assertv(cond,msg) (cond);
  #define assertc(cond,msg) (cond);
  #define asserte(cond,msg) (cond);
  #define ptry    
  #define try
  #define $
  #define $v
  #define $c
  #define $e
  #define $$
#endif


#endif
