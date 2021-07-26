#include <stdio.h>
#include "tod.h"
#include "benchmark.h"

void benchmark_init() {
    tod_init();
}

int time_this(int(*f)(void*), void* ctx, uint16_t numiterations) {
    struct TOD start, end;
    int res;
    uint16_t i;

    getTOD(&start);
    for (i=0; i<numiterations; i++) {
        res = f(ctx);
    }
    getTOD(&end);
    printf("start: "); TOD_print(&start); printf("\r\n");
    printf("  end: "); TOD_print(&end);
#ifdef BENCHMARK_WARN
    printf("\r\n");
    printf(" start = %li ds\r\n", TOD_to_deciseconds(&start));
    printf("   end = %li ds\r\n", TOD_to_deciseconds(&end));
#endif
    {
        int32_t d = TOD_diff(&start, &end);
#ifdef BENCHMARK_DECISECONDS
        d = TOD_diff(&start, &end);
        printf(" = %li/10 sec", d);
#else
        int32_t d_s = d / 10;
        int32_t d_ds = d % 10;
        printf(" = %li.%li sec", d_s, d_ds);
#endif
    }
    return res;
}

