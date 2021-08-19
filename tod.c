#include <stdio.h>
#include <chj-ctest/test.h>
#include "tod.h"
#include "util.h"

#ifdef __C64__

void __fastcall__ tod_calibrate(); // tod_init_s.s

static bool tod_init_done = 0;

void tod_init() {
    if (tod_init_done) return;
    tod_calibrate();
    tod_init_done = 1;
}

#define TOD_TENTHS() PEEK(0xDC08)
#define TOD_SECONDS() PEEK(0xDC09)
#define TOD_MINUTES() PEEK(0xDC0A)
#define TOD_HOURS() PEEK(0xDC0B)

unsigned char getTOD(struct TOD *out) {
    // how do I get the data atomically?
    unsigned char retries=0;
    while (1) {
        out->hours_BCD_AMPM = TOD_HOURS();
        out->min_BCD = TOD_MINUTES();
        out->sec_BCD = TOD_SECONDS();
        out->sectenths = TOD_TENTHS();
        if (
            (out->sectenths == TOD_TENTHS()) &&
            (out->sec_BCD == TOD_SECONDS()) &&
            (out->min_BCD == TOD_MINUTES()) &&
            (out->hours_BCD_AMPM == TOD_HOURS())
            ) {
#ifdef TOD_WARN
            if (retries) {
                WARN_("getTOD: %i retry/ies", retries);
            }
#endif
            return retries;
        }
        retries++;
    }
}

#else /* not __C64__ */

#include <sys/time.h>
#include <time.h>
#include <errno.h>

void tod_init() {
    // noop
}

static uint8_t int_to_BCD(uint8_t v) {
    uint8_t d0 = v % 10;
    v = v / 10;
    uint8_t d1 = v % 10;
    v = v / 10;
    if (v) DIE_("int_to_BCD: remainder after 2 digits: %i", v);
    return d1*16 + d0;
}

static void t_int_to_BCD(struct TestStatistics *__test_stats, uint8_t x) {
    ASSERT_EQ(BCD_to_int(int_to_BCD(x)), x);
}

/* uint8_t BCD_to_int(uint8_t bcd); */

TEST(int_to_BCD) {
    ASSERT_EQ(int_to_BCD(28), 0x28);
    t_int_to_BCD(__test_stats, 4);
    t_int_to_BCD(__test_stats, 40);
    t_int_to_BCD(__test_stats, 99);
}

static uint8_t int_to_BCD_AMPM(uint8_t v) {
    return int_to_BCD(v); //XXX finish
}

unsigned char getTOD(struct TOD *out) {
    struct timeval tv;
    struct timezone tz;
    if (gettimeofday(&tv, &tz)) {
        perror("gettimeofday");
        DIE("could not getTOD");
    }
    out->sectenths = tv.tv_usec / 100000;
    time_t tval = tv.tv_sec; /* sgh C lib */
    struct tm t;
    if (! gmtime_r(&tval, &t)) DIE("gmtime_r failed (can this even happen?)");
    out->sec_BCD = int_to_BCD(t.tm_sec);
    out->min_BCD = int_to_BCD(t.tm_min);
    out->hours_BCD_AMPM = int_to_BCD_AMPM(t.tm_hour); //XXX
    return 0;
}

#endif /* __C64__ */


uint8_t BCD_to_int(uint8_t bcd) {
    return ((bcd & 0xF0) >> 4) * 10 + (bcd & 0x0F);
}

void TOD_print(const struct TOD *t) {
    printf("%02i:%02i:%02i.%01i %s",
           TOD_hours_american(t),
           TOD_min(t),
           TOD_sec(t),
           TOD_dsec(t),
           TOD_is_PM(t) ? "pm" : "am");
}

int32_t TOD_to_deciseconds(const struct TOD *t) {
    return ((int32_t)TOD_dsec(t) +
            10 * ((int32_t)TOD_sec(t) +
                  60 * ((int32_t)TOD_min(t) +
                        60 * (int32_t)TOD_hours_european(t))));
}

static const int32_t day_deciseconds = 24*60*60*10;
// (XX wonder if that works out. No skips nothing?)

// This assumes that time stamp b is always taken after a. So if the
// difference is negative, it must have wrapped around.
int32_t TOD_diff(const struct TOD *a, const struct TOD *b) {
    int32_t d = TOD_to_deciseconds(b) - TOD_to_deciseconds(a);
    return d >= 0 ? d : d + day_deciseconds;
}

TEST(t1) {
    struct TOD t1, t2, t3, t;

    t1.sectenths = 9;
    t1.sec_BCD = 0x59;
    t1.min_BCD = 0x02;
    t1.hours_BCD_AMPM = 0x11;
    // calc> 11*60*60*10 + (2*60 + 59)*10 + 9
    // 397799

    t2.sectenths = 7;
    t2.sec_BCD = 0x59;
    t2.min_BCD = 0x02;
    t2.hours_BCD_AMPM = (0x11 | 128);

    t3.sectenths = 7;
    t3.sec_BCD = 0x58;
    t3.min_BCD = 0x02;
    t3.hours_BCD_AMPM = (0x11 | 128);

    ASSERT_EQ(TOD_hours_american(&t1), 11);
    ASSERT_EQ(TOD_hours_american(&t2), 11);
    ASSERT_EQ(TOD_hours_european(&t1), 11);
    ASSERT_EQ(TOD_hours_european(&t2), 23);

    t.sectenths = 5;
    t.sec_BCD = 0x00;
    t.min_BCD = 0x00;
    t.hours_BCD_AMPM = (0x00 | 0*128);
    ASSERT_EQ_(int32_t, TOD_to_deciseconds(&t), 5);

    t.sec_BCD = 0x01;
    ASSERT_EQ_(int32_t, TOD_to_deciseconds(&t), 15);

    t.min_BCD = 0x01;
    ASSERT_EQ_(int32_t, TOD_to_deciseconds(&t), 615);
    t.min_BCD = 0x11;
    ASSERT_EQ_(int32_t, TOD_to_deciseconds(&t), 6615);
    t.min_BCD = 0x46;
    ASSERT_EQ_(int32_t, TOD_to_deciseconds(&t), 27615);

    t.hours_BCD_AMPM = (0x01 | 0*128);
    ASSERT_EQ_(int32_t, TOD_to_deciseconds(&t), 63615);
    // Then from here on it was failing, aha, 16 bit unsigned limit to
    // result. Added `(int32_t)` everywhere inside TOD_to_deciseconds
    // and now it's fine:
    t.hours_BCD_AMPM = (0x02 | 0*128);
    ASSERT_EQ_(int32_t, TOD_to_deciseconds(&t), 99615);
    t.hours_BCD_AMPM = (0x09 | 0*128);
    ASSERT_EQ_(int32_t, TOD_to_deciseconds(&t), 351615);
    t.hours_BCD_AMPM = (0x11 | 0*128);
    ASSERT_EQ_(int32_t, TOD_to_deciseconds(&t), 423615);
    t.hours_BCD_AMPM = (0x23 | 0*128);
    ASSERT_EQ_(int32_t, TOD_to_deciseconds(&t), 855615);
    t.hours_BCD_AMPM = (0x11 | 1*128);
    ASSERT_EQ_(int32_t, TOD_to_deciseconds(&t), 855615);
    
    ASSERT_EQ_(int32_t, TOD_to_deciseconds(&t1), 397799);
    ASSERT_EQ_(int32_t, TOD_to_deciseconds(&t2), 829797);
    ASSERT_EQ_(int32_t, TOD_to_deciseconds(&t3), 829787);
    
    ASSERT_EQ_(int32_t, TOD_diff(&t2, &t3), 863990);
    ASSERT_EQ_(int32_t, TOD_diff(&t3, &t2), 10);
}

