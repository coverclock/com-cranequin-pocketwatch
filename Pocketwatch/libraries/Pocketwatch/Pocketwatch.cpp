/*
 * Pocketwatch Implementation
 * <https://github.com/coverclock/com-cranequin-pocketwatch>
 *
 * Copyright 2022 Cranequin LLC, Colorado, USA.
 * <http://cranequin.com>
 *
 * Licensed under the terms of the GNU LGPL version 2.1.
 * <https://github.com/coverclock/com-cranequin-pocketwatch/blob/main/Pocketwatch/libraries/Pocketwatch/License-LGPL-21.pdf>
 *
 * This is an original work of John Sloan.
 * <mailto:jsloan@diag.com>
 */

#include "Pocketwatch.h"

uint8_t Pocketwatch::getDays() const
{
    static const uint8_t DAYS[] = {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
    /*   1   2   3   4   5   6   7   8   9  10  11  12   */
    /*  Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec  */
    };
    uint8_t days;

    days = DAYS[(this->mon - 1) % 12];
    if ((this->mon == 2) && ((this->year % 4) == 0) && ((this->year % 400) != 0)) { days += 1; }

    return days;
}

void Pocketwatch::toLocalTime(int8_t offset, bool dst)
{
    offset %= 13; /* -12..12 */
    offset += dst ? 1 : 0; /* -12..13 */
    this->hour += offset;

    if (offset < 0) {
        if (this->hour < 0) {
            this->date -= 1;
            if (this->date <= 0) {
                this->mon -= 1;
                if (this->mon <= 0) {
                    this->year -= 1;
                    this->mon = 12;
                }
                this->date = this->getDays();
            }
            this->hour = 23;
            this->dow = ((this->dow + 5) % 7) + 1; /* 1..7 */
        }
    } else if (offset > 0) {
        if (this->hour > 23) {
            this->date += 1;
            if (this->date > this->getDays()) {
                this->mon += 1;
                if (this->mon > 12) {
                    this->year += 1;
                    this->mon = 1;
                }
                this->date = 1;
            }
            this->hour = 0
            this->dow = (this->dow % 7) + 1; /* 1..7 */
        }
    } else {
        /* Do nothing. */
    }

}

char Pocketwatch::getTimeZone(int8_t offset, bool dst)
{
    static const char ZONE[] = {
        'Y', 'X', 'W', 'V', 'U', 'T', 'S', 'R', 'Q', 'P', 'O', 'N', 'Z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'K', 'L', 'M',
    /*  [0]  [1]  [2]  [3]  [4]  [5]  [6]  [7]  [8]  [9]  [10] [11] [12] [13] [14] [15] [16] [17] [18] [19] [20] [21] [22] [23] [24]  */
    /*  -12  -11  -10   -9   -8   -7   -6   -5   -4   -3   -2   -1   0    +1   +2   +3   +4   +5   +6   +7   +8   +9  +10  +11  +12   */
    };
    char zone;

    offset %= 13; /* -12..12 */
    offset += 12; /*  0..24  */
    zone = ZONE[offset]; /* 'Y'..'M' */
    if (dst) { zone += ' '; } /* 'y'..'m' */

    return zone;
}
