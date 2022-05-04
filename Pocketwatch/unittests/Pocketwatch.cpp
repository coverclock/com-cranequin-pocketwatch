/*
 * Pocketwatch Unit Tests
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

#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "../libraries/Pocketwatch/Pocketwatch.h"
#include "../libraries/Pocketwatch/Pocketwatch.cpp"

static int test1(int16_t y0, int8_t m0, int8_t n1)
{
  Localtime ll;
  int8_t nx;

  ll.year = y0;
  ll.mon = m0;

  nx = ll.getDays();

  fprintf(stderr, "test1: INP %04d-%02d OUT %d EXP %d\n",
    y0, m0,
    nx,
    n1);

  return (nx == n1);
}

static int test2(int8_t offset, bool dst, char z1)
{
  char zx;

  zx = Localtime::getTimeZone(offset, dst);

  fprintf(stderr, "test2: INP %d %d OUT '%c' EXP '%c'\n",
    offset, dst,
    zx,
    z1);

  return (zx == z1);
}

static int test3(int16_t y0, int8_t m0, int8_t d0, int8_t h0, int8_t w0, int8_t offset, bool dst, int16_t y1, int8_t m1, int8_t d1, int8_t h1, int8_t w1)
{
  Localtime ll;

  ll.year = y0;
  ll.mon = m0;
  ll.date = d0;
  ll.hour = h0;
  ll.dow = w0;

  ll.toLocalTime(offset, dst);

  fprintf(stderr, "test3: INP %04d-%02d-%02dT%02d%c%d OUT %04d-%02d-%02dT%02d%c%d EXP %04d-%02d-%02dT%2d%c%d\n",
    y0, m0, d0, h0, ll.getTimeZone(0, false), w0,
    ll.year, ll.mon, ll.date, ll.hour, ll.getTimeZone(offset, dst), ll.dow,
    y1, m1, d1, h1, ll.getTimeZone(offset, dst), w1);

  return ((ll.year == y1) && (ll.mon == m1) && (ll.date == d1) && (ll.hour == h1) && (ll.dow == w1));
}

int main(int argc, char ** argv)
{
  {
    assert(test1(2000,  1, 31));
    assert(test1(2000,  2, 28));
    assert(test1(2000,  3, 31));
    assert(test1(2000,  4, 30));
    assert(test1(2000,  5, 31));
    assert(test1(2000,  6, 30));
    assert(test1(2000,  7, 31));
    assert(test1(2000,  8, 31));
    assert(test1(2000,  9, 30));
    assert(test1(2000, 10, 31));
    assert(test1(2000, 11, 30));
    assert(test1(2000, 12, 31));

    assert(test1(2001,  1, 31));
    assert(test1(2001,  2, 28));
    assert(test1(2001,  3, 31));
    assert(test1(2001,  4, 30));
    assert(test1(2001,  5, 31));
    assert(test1(2001,  6, 30));
    assert(test1(2001,  7, 31));
    assert(test1(2001,  8, 31));
    assert(test1(2001,  9, 30));
    assert(test1(2001, 10, 31));
    assert(test1(2001, 11, 30));
    assert(test1(2001, 12, 31));

    assert(test1(2004,  1, 31));
    assert(test1(2004,  2, 29));
    assert(test1(2004,  3, 31));
    assert(test1(2004,  4, 30));
    assert(test1(2004,  5, 31));
    assert(test1(2004,  6, 30));
    assert(test1(2004,  7, 31));
    assert(test1(2004,  8, 31));
    assert(test1(2004,  9, 30));
    assert(test1(2004, 10, 31));
    assert(test1(2004, 11, 30));
    assert(test1(2004, 12, 31));
  }

  {
    assert(test2(-12, false, 'Y'));
    assert(test2(-11, false, 'X'));
    assert(test2(-10, false, 'W'));
    assert(test2( -9, false, 'V'));
    assert(test2( -8, false, 'U'));
    assert(test2( -7, false, 'T'));
    assert(test2( -6, false, 'S'));
    assert(test2( -5, false, 'R'));
    assert(test2( -4, false, 'Q'));
    assert(test2( -3, false, 'P'));
    assert(test2( -2, false, 'O'));
    assert(test2( -1, false, 'N'));
    assert(test2(  0, false, 'Z'));
    assert(test2(  1, false, 'A'));
    assert(test2(  2, false, 'B'));
    assert(test2(  3, false, 'C'));
    assert(test2(  4, false, 'D'));
    assert(test2(  5, false, 'E'));
    assert(test2(  6, false, 'F'));
    assert(test2(  7, false, 'G'));
    assert(test2(  8, false, 'H'));
    assert(test2(  9, false, 'I'));
    /*                       'J' */
    assert(test2( 10, false, 'K'));
    assert(test2( 11, false, 'L'));
    assert(test2( 12, false, 'M'));

    assert(test2(-12,  true, 'y'));
    assert(test2(-11,  true, 'x'));
    assert(test2(-10,  true, 'w'));
    assert(test2( -9,  true, 'v'));
    assert(test2( -8,  true, 'u'));
    assert(test2( -7,  true, 't'));
    assert(test2( -6,  true, 's'));
    assert(test2( -5,  true, 'r'));
    assert(test2( -4,  true, 'q'));
    assert(test2( -3,  true, 'p'));
    assert(test2( -2,  true, 'o'));
    assert(test2( -1,  true, 'n'));
    assert(test2(  0,  true, 'z'));
    assert(test2(  1,  true, 'a'));
    assert(test2(  2,  true, 'b'));
    assert(test2(  3,  true, 'c'));
    assert(test2(  4,  true, 'd'));
    assert(test2(  5,  true, 'e'));
    assert(test2(  6,  true, 'f'));
    assert(test2(  7,  true, 'g'));
    assert(test2(  8,  true, 'h'));
    assert(test2(  9,  true, 'i'));
    /*                       'J' */
    assert(test2( 10,  true, 'k'));
    assert(test2( 11,  true, 'l'));
    assert(test2( 12,  true, 'm'));
  }

  return 0;
}
