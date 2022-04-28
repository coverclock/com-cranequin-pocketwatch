/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_CRANEQUIN_POCKETWATCH_
#define _H_COM_CRANEQUIN_POCKETWATCH_

/**
 ** Copyright 2022 Cranequin LLC, Colorado, USA.
 ** Licensed under the terms of the GNU LGPL version 2.1.
 **/

#include <DS3231.h>

#define POCKETWATCH (!0)

boolean pocketwatch_is_leapyear(const Time * tp);

char pocketwatch_get_timezone(int8_t offset);

void pocketwatch_to_localtime(Time * tp, int8_t offset);

#endif
