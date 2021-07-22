#include <stdio.h>
#include "tod.h"
#include "util.h"
#include "test.h"

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
    // how the h* do I get the data atomically?
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
            if (retries) {
                WARN("getTOD: %i retry/ies", retries);
            }
            return retries;
        }
        retries++;
    }
}

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
    return (t->sectenths +
            10 * (TOD_sec(t) +
                  60 * (TOD_min(t) +
                        60 * TOD_hours_european(t))));
}

static const int32_t day_deciseconds = 24*60*60*10;
// (XX wonder if that works out. No skips nothing?)

// This assumes that time stamp b is always taken after a. So if the
// difference is negative, it must have wrapped around.
int32_t TOD_diff(const struct TOD *a, const struct TOD *b) {
    int32_t d = TOD_to_deciseconds(b) - TOD_to_deciseconds(a);
    return d >= 0 ? d : d + day_deciseconds;
    //return d;
}

TEST(t1) {
    struct TOD t1, t2, t3, t;

    t1.sectenths = 9;
    t1.sec_BCD = 0x59;
    t1.min_BCD = 0x02;
    t1.hours_BCD_AMPM = 0x11;

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
    t.hours_BCD_AMPM = (0x11 | 0*128);
    ASSERT_EQ_(int32_t, TOD_to_deciseconds(&t), 423615);
    
    /* ASSERT_EQ_(int32_t, TOD_to_deciseconds(&t1), 397799); */
    /* ASSERT_EQ_(int32_t, TOD_to_deciseconds(&t2), 829800); */
    /* ASSERT_EQ_(int32_t, TOD_to_deciseconds(&t3), 829790); */
    
    ASSERT_EQ_(int32_t, TOD_diff(&t2, &t3), 863990);
    ASSERT_EQ_(int32_t, TOD_diff(&t3, &t2), 10);
    // XX...
};

