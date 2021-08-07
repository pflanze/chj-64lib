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
#  define fastcall __fastcall__
#  define IS_C64 1
// XX better? enum? ?
typedef unsigned char bool;
#else
#  define fastcall
#  define IS_C64 0
#endif


#define WARN(msg, ...) printf(msg "\r\n", __VA_ARGS__)
#define DIE(msg, ...) do { printf(msg "\r\n", __VA_ARGS__); exit(1); } while(0)
#define WARN_(msg) printf(msg "\r\n")
#define DIE_(msg) do { printf(msg "\r\n"); exit(1); } while(0)



#define IS_ODD(x) ((x) & 1)
#define IS_EVEN(x) (! IS_ODD(x))


#endif
