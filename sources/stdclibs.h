#ifndef _STDCLIBS_
#define _STDCLIBS_
/*
   General headers from standard C libraries
*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#if __STDC_VERSION__ >= 199901L
    /* C99 code */
    #define C99
#endif


#ifdef C99
    /*My code in C99 format*/
    #include <stdbool.h>
#else
    typedef enum { false, true } bool;
#endif

#define MAX(A,B) ((A) > (B) ? A : B)

#endif