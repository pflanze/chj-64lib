#ifndef _UTIL_H
#define _UTIL_H

#include <stdio.h>
#include <stdlib.h>

/*
static void poke(unsigned int addr, unsigned char val) {
    *((unsigned char*)addr) = val;
}
static unsigned char peek(unsigned int addr) {
    return *((unsigned char*)addr);
}
*/

#ifdef __C64__

#define POKE(addr, val) *((unsigned char*)(addr)) = val
#define PEEK(addr) (*((unsigned char*)(addr)))
#define BYTE(addr) (*((unsigned char*)(addr)))

#define PTR(addr) ((void*)(addr))

#endif


#ifdef __C64__

# define ABORT() exit(1)
# define fastcall __fastcall__
# define IS_C64 1
# include "_64lib_bool.h"
# define UNUSED
# define NORETURN


#else
/* not C64 */

#include <stdlib.h>

# define ABORT() abort()
# define fastcall
# define IS_C64 0

# ifndef __cplusplus
#  include "_64lib_bool.h"
# endif

# ifdef __GNUC__ /* GCC */
#  define UNUSED __attribute__ ((unused))
#  define NORETURN __attribute__((__noreturn__))
# else
#warning "unknown compiler"
#  define UNUSED 
// #  include <stdnoreturn.h>
#  define NORETURN _Noreturn
# endif


#endif


#define WARN_(msg, ...) printf(msg "\r\n", __VA_ARGS__)
#define DIE_(msg, ...) do { printf(msg "\r\n", __VA_ARGS__); ABORT(); } while(0)
#define WARN(msg) printf(msg "\r\n")
#define DIE(msg) do { printf(msg "\r\n"); ABORT(); } while(0)



#define IS_ODD(x) ((x) & 1)
#define IS_EVEN(x) (! IS_ODD(x))


#endif
