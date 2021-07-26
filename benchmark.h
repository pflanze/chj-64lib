
void benchmark_init();

/* Runs f(ctx), then prints the start and end times and the time difference.
   Returns the total real time taken in deciseconds. */
int32_t time_this(void(*f)(void*), void* ctx, uint16_t numiterations);

