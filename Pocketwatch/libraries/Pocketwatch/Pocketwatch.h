#ifndef _H_COM_CRANEQUIN_POCKETWATCH_
#define _H_COM_CRANEQUIN_POCKETWATCH_

/*
 * Pocketwatch Declaration
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

#include <stdint.h>
#include <stdbool.h>

/*
 * Define a mechanism to adjust a time in UTC to a local time.
 */
class Pocketwatch {

public:

    int8_t hour; /* 0..23 */
    int8_t date; /* 1..31 */
    int8_t mon;  /* 1..12 */
    int8_t year;
    int8_t dow;  /* 1..7 */

    /*
     * Given the year and month, return the number of days in the month.
     */
    int8_t getDays() const;

    /*
     * Given a Pocketwatch object in UTC, a signed time zone offset in hours,
     * and a flag indicating whether Daylight Saving Time is active, return the
     * object adjusted to the local time.
     */
    void toLocalTime(int8_t offset, bool dst);

    /*
     * Given a signed time zone offset in hours, return a time zone name using
     * the single letter NATO nomenclature, where 'Z' for Zulu is UTC which has
     * an offset of zero.
     */
    static char getTimeZone(int8_t offset, bool dst);

};

#endif
