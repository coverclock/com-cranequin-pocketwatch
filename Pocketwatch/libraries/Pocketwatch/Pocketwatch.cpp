/* vi: set ts=4 expandtab shiftwidth=4: */

/**
 ** Copyright 2022 Cranequin LLC, Colorado, USA.
 ** Licensed under the terms of the GNU LGPL version 2.1.
 **/

boolean pocketwatch_is_leapyear(const Time * tp) {
    return (((tp->year % 4) == 0) && ((tp->year % 400) != 0));
}

static char ZONE[] = {
    'Y', 'X', 'W', 'V', 'U',	/* -12 ..  -8 */ /* [ 0] .. [ 4] */
    'T', 'S', 'R', 'Q', 'P',	/*  -7 ..  -3 */ /* [ 5] .. [ 9] */
    'O', 'N', 'Z', 'A', 'B',	/*  -2 ..  +2 */ /* [10] .. [14] */
    'C', 'D', 'E', 'F', 'G',	/*  +3 ..  +7 */ /* [15] .. [19] */
    'H', 'I', 'K', 'L', 'M',	/*  +8 .. +12 */ /* [20] .. [24] */
};

char pocketwatch_get_timezone(int8_t offset) {
    return ZONE[(offset + 12) % (sizeof(ZONE) / sizeof(ZONE[0]))];
}

/*
        uint8_t         hour;
        uint8_t         min;
        uint8_t         sec;
        uint8_t         date;
        uint8_t         mon;
        uint16_t        year;
        uint8_t         dow;
*/

void pocketwatch_to_localtime(Time * tp, int8_t offset) {
    offset %= 13;

    
}
