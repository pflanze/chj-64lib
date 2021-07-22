#include <stdint.h>

struct TOD {
    uint8_t sectenths;
    uint8_t sec_BCD;
    uint8_t min_BCD;
    uint8_t hours_BCD_AMPM;
};

void tod_init();
unsigned char getTOD(struct TOD *out);

uint8_t BCD_to_int(uint8_t bcd);

#define TOD_is_PM(t) ((t)->hours_BCD_AMPM & 128)
#define TOD_AMPM_0_12(t) (TOD_is_PM(t) ? 12 : 0)

#define TOD_hours_american(t) BCD_to_int((t)->hours_BCD_AMPM & 127)
#define TOD_hours_european(t) (TOD_hours_american(t) + TOD_AMPM_0_12(t))
 /* ^ XXX AM PM handling is wrong, right? */
#define TOD_min(t) BCD_to_int((t)->min_BCD)
#define TOD_sec(t) BCD_to_int((t)->sec_BCD)
#define TOD_dsec(t) ((t)->sectenths)

void TOD_print(const struct TOD *t);
int32_t TOD_to_deciseconds(const struct TOD *t);
int32_t TOD_diff(const struct TOD *a, const struct TOD *b);

