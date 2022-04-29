/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 ** Copyright 2022 Cranequin LLC, Colorado, USA.
 ** Licensed under the terms of the GNU LGPL version 2.1.
 **/

static uint8_t DAYS[] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
/*    1   2   3   4   5   6   7   8   9  10  11  12   */
/*  Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec   */
};

void pocketwatch_to_localtime(Time * tp, int8_t offset, uint8_t dst)
{
    int16_t yy;
    int8_t mm;
    int8_t dd;
    int8_t hh;
    int8_t ll;

    offset %= 13;
    offset += dst;

    yy = tp->year;
    mm = tp->mon;
    dd = tp->date;
    hh = tp->hour;

    if (offset < 0) {
        hh += offset;
        if (hh < 0) {
            dd -= 1;
            if (dd <= 0) {
                mm -= 1;
                if (mm <= 0) {
                    yy -= 1;
                    mm = 12;
                }
                ll = DAYS[mm - 1] + (((mm == 2) && ((yy % 4) == 0) && ((yy % 400) != 0)) ? 1 : 0);
                dd = ll;
            }
            hh = 23;
            tp->dow = ((tp->dow + 7) % 7) + 1;
        }
    } else if (offset > 0) {
        hh += offset;
        if (hh > 23) {
            dd += 1;
            ll = DAYS[mm - 1] + (((mm == 2) && ((yy % 4) == 0) && ((yy % 400) != 0)) ? 1 : 0);
            if (dd > ll) {
                mm += 1;
                if (mm > 12) {
                    yy += 1;
                    mm = 1;
                }
                dd = 1;
            }
            hh = 0
            tp->dow = (tp->dow % 7) + 1;
        }
    } else {
        /* Do nothing. */
    }

}

static char ZONE[] = {
    'Y', 'X', 'W', 'V', 'U', 'T', 'S', 'R', 'Q', 'P', 'O', 'N', 'Z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'K', 'L', 'M',
/*  -12  -11  -10   -9   -8   -7   -6   -5   -4   -3   -2   -1   0    +1   +2   +3   +4   +5   +6   +7   +8   +9  +10  +11  +12   */
};

char pocketwatch_get_timezone(int8_t offset)
{
    return ZONE[(offset + 12) % (sizeof(ZONE) / sizeof(ZONE[0]))];
}
