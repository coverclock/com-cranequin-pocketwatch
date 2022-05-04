#ifndef _H_COM_CRANEQUIN_POCKETWATCH_
#define _H_COM_CRANEQUIN_POCKETWATCH_

/*
 * Pocketwatch Declarations
 * <https://github.com/coverclock/com-cranequin-pocketwatch>
 *
 * Copyright 2022 Cranequin LLC, Colorado, USA.
 * <http://cranequin.com>
 *
 * Licensed under the terms of the GNU LGPL version 2.1.
 * <https://github.com/coverclock/com-cranequin-pocketwatch/blob/main/Pocketwatch/libraries/Pocketwatch/License-LGPL-21.pdf>
 *
 * This is a work of John Sloan.
 * <mailto:jsloan@diag.com>
 */

#include <stdint.h>
#include <stdbool.h>

/*
 * Debounce a digital input.
 *
 * REFERENCES
 *
 * Jack Gannsle, "A Guide to Debouncing, or How To Debounce a Contact in Two
 * Easy Pages", 2008-06, http://www.ganssle.com/debouncing.htm
 *
 * Jack Gannsle, "A Guide to Debouncing - Part 2, or How To Debounce a Contact
 * in Two Easy Pages", 2008-06, http://www.ganssle.com/debouncing-pt2.htm
 *
 * Jack Ganssle, "My Favorite Software Debouncers", 2004-06-16,
 * http://www.embedded.com/electronics-blogs/break-points/4024981/My-favorite-software-debouncers
 *
 * Dr. Marty, "The Best Switch Debounce Routine Ever", 2009-05-20,
 * http://drmarty.blogspot.com/2009/05/best-switch-debounce-routine-ever.html
 *
 * Jack Ganssle, "A Guide to Debouncing", 2008-06,
 * http://www.eng.utah.edu/~cs5780/debouncing.pdf
 */
class Debounce {

public:

  uint8_t a : 1;
  uint8_t b : 1;
  uint8_t c : 1;
  uint8_t r : 1;
  uint8_t p : 1;

  /*
   * Constructor.
   */
  Debounce(int initial);

  /*
   * Given a digital input, debounce it and return the output.
   */
  int debounce(int input);

};

/*
 * Define a mechanism to adjust a time in UTC to a local time given a
 * signed integer hour time zone offset in the range -12..12. This algorithm
 * does not support half-hour offsets (which exist).
 */
class Localtime {

public:

  int16_t year;
  int8_t mon;  /* 1..12 */
  int8_t date; /* 1..31 */
  int8_t hour; /* 0..23 */
  int8_t dow;  /* 1..7 */

  /*
   * Constructor.
   */
  Localtime();

  /*
   * Given a Pocketwatch object, return the number of days in the month
   * for that year.
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
   * an offset of zero. If Daylight Saving Time is enabled and active for this
   * time zone, convert the time zone name to lower case to indicate it.
   */
  static char getTimeZone(int8_t offset, bool dst);

};

#endif
