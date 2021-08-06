#ifndef _UTIL_H
#define _UTIL_H

#include <stdio.h>

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

#define WARN(msg, ...) printf(msg "\r\n", __VA_ARGS__)
#define DIE(msg, ...) do { printf(msg "\r\n", __VA_ARGS__); exit(1); } while(0)
#define WARN_(msg) printf(msg "\r\n")
#define DIE_(msg) do { printf(msg "\r\n"); exit(1); } while(0)

// XX better? enum? ?
typedef unsigned char bool;


#endif
