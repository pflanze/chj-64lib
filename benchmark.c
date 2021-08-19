#include <stdio.h>
#include <inttypes.h>
#include "tod.h"
#include "benchmark.h"

void benchmark_init() {
    tod_init();
}

int32_t time_this(void(*f)(void*), void* ctx, uint16_t numiterations) {
    struct TOD start, end;
    uint16_t i;

    getTOD(&start);
    for (i=0; i<numiterations; i++) {
        f(ctx);
    }
    getTOD(&end);
    printf("start: "); TOD_print(&start); printf("\r\n");
    printf("  end: "); TOD_print(&end);
#ifdef BENCHMARK_WARN
    printf("\r\n");
    printf(" start = %" PRIi32 " ds\n", TOD_to_deciseconds(&start));
    printf("   end = %" PRIi32 " ds\n", TOD_to_deciseconds(&end));
#endif
    {
        int32_t d = TOD_diff(&start, &end);
#ifdef BENCHMARK_DECISECONDS
        d = TOD_diff(&start, &end);
        printf(" = %" PRIi32 "/10 sec", d);
#else
        int32_t d_s = d / 10;
        int32_t d_ds = d % 10;
        printf(" = %" PRIi32 ".%" PRIi32 " sec", d_s, d_ds);
#endif
        return d;
    }
}

