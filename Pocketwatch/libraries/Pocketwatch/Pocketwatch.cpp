/*
 * Pocketwatch Implementations
 * <https://github.com/coverclock/com-cranequin-pocketwatch>
 *
 * Copyright 2022 Cranequin LLC, Colorado, USA.
 * <http://cranequin.com>
 *
 * Licensed under the terms of the GNU LGPL version 2.1.
 * <https://github.com/coverclock/com-cranequin-pocketwatch/blob/main/Pocketwatch/licenses/License-LGPL-21.pdf>
 *
 * This is a work of John Sloan.
 * <mailto:jsloan@diag.com>
 */

#include "Pocketwatch.h"

Debouncer::Debouncer(int initial)
{
  initial = !!initial;
  this->a = initial;
  this->b = initial;
  this->c = initial;
  this->r = initial;
  this->p = initial;
}

int Debouncer::debounce(int input)
{
  this->c = this->b;
  this->b = this->a;
  this->a = !!input;
  this->p = this->r;
  this->r = (this->p & (this->a | this->b | this->c)) | (this->a & this->b & this->c);

  return this->r;
}

Debouncer::Edge Debouncer::edge() const
{
  enum Edge result;

  if (!this->p) {
    if (!this->r) {
      result = IS_LOW;
    } else {
      result = IS_RISING;
    }
  } else {
    if (!this->r) {
      result = IS_FALLING;
    } else {
      result = IS_HIGH;
    }
  }

  return result;
}

Localtime::Localtime()
{
  this->year = 1970;
  this->mon = 1;
  this->date = 1;
  this->hour = 0;
  this->dow = 4;
}

bool Localtime::isLeapYear() const
{
  return (((this->year % 4) == 0) && (((this->year % 100) != 0) || ((this->year % 400) == 0)));
}

int8_t Localtime::getDays() const
{
  static const uint8_t DAYS[] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
  /*  1   2   3   4   5   6   7   8   9  10  11  12   */
  /* Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec  */
  };
  int8_t days;

  days = DAYS[(this->mon - 1) % 12];
  if ((this->mon == 2) && isLeapYear()) {
    days += 1;
  }

  return days;
}

void Localtime::toLocalTime(int8_t offset, bool dst)
{
  offset %= 13; /* -12..12 */
  offset += dst ? 1 : 0; /* -12..13 */

  if (offset == 0) {
    return;
  }

  this->hour += offset;
  if (this->hour < 0) {
    this->date -= 1;
    if (this->date <= 0) {
      this->mon -= 1;
      if (this->mon <= 0) {
        this->year -= 1;
        this->mon += 12;
      }
      this->date += this->getDays();
    }
    this->hour += 24;
    this->dow = ((this->dow + 5) % 7) + 1; /* 1..7 */
  } else if (this->hour >= 24) {
    this->date += 1;
    uint8_t days = this->getDays();
    if (this->date > days) {
      this->mon += 1;
      if (this->mon > 12) {
        this->year += 1;
        this->mon -= 12;
      }
      this->date -= days;
    }
    this->hour -= 24;
    this->dow = (this->dow % 7) + 1; /* 1..7 */
  } else {
    /* Do nothing. */
  }

}

char Localtime::getTimeZone(int8_t offset, bool dst)
{
  static const char ZONE[] = {
    'Y', 'X', 'W', 'V', 'U', 'T', 'S', 'R', 'Q', 'P', 'O', 'N', 'Z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'K', 'L', 'M',
/*  [0]  [1]  [2]  [3]  [4]  [5]  [6]  [7]  [8]  [9]  [10] [11] [12] [13] [14] [15] [16] [17] [18] [19] [20] [21] [22] [23] [24]  */
/*  -12  -11  -10   -9   -8   -7   -6   -5   -4   -3   -2   -1    0   +1   +2   +3   +4   +5   +6   +7   +8   +9  +10  +11  +12   */
  };
  char zone;

  offset %= 13; /* -12..12 */
  offset += 12; /*  0..24  */

  zone = ZONE[offset]; /* 'Y'..'M' */

  if (dst) {
    zone += ' '; /* 'y'..'m' */
  }

  return zone;
}
