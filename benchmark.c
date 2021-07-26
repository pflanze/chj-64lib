#include <stdio.h>
#include "tod.h"
#include "benchmark.h"

void benchmark_init() {
    tod_init();
}

int time_this(int(*f)(void*), void* ctx, uint16_t numiterations) {
    struct TOD start, end;
    int res;
    int32_t d;
    uint16_t i;

    getTOD(&start);
    for (i=0; i<numiterations; i++) {
        res = f(ctx);
    }
    getTOD(&end);
    printf("start: "); TOD_print(&start); printf("\r\n");
    printf("  end: "); TOD_print(&end); printf("\r\n");
#ifdef BENCHMARK_WARN
    printf(" start = %li ds\r\n", TOD_to_deciseconds(&start));
    printf("   end = %li ds\r\n", TOD_to_deciseconds(&end));
#endif
    d = TOD_diff(&start, &end);
    printf("duration: %li /10 sec", d);
    return res;
}

