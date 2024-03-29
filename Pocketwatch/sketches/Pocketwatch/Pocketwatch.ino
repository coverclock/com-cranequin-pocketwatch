/*
This is the demonstration code for the UNIVERSAL-SOLDER / Everset ES100 
Application Development Kit. It reads the decoded time stamp from 
the ES100MOD receiver module and shows several information on a 4x20
character display. There are no function assignments for unused GPIO,
analog inputs and the 3 push buttons included in this sketch.

Version: 1 (10/04/2020)

PLEASE FEEL FREE TO CONTRIBUTE TO THE DEVELOPMENT. CORRECTIONS AND
ADDITIONS ARE HIGHLY APPRECIATED. SEND YOUR COMMENTS OR CODE TO:
support@universal-solder.ca 

Please support us by purchasing products from UNIVERSAL-SOLDER.ca store!

Copyright (c) 2020 UNIVERSAL-SOLDER Electronics Ltd. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * POCKETWATCH
 * Repository: <https://github.com/coverclock/com-cranequin-pocketwatch>
 * Web Site: <http://cranequin.com>
 * Author: <mailto:jsloan@diag.com>
 * Configuration:
 * POCKETWATCH - define to enable Pockwatch changes.
 * VERBOSE - define to enable verbose serial output.
 * CONTINUOUS - define to resynchronize continuously.
 * OFFSET - define to be your desired time zone offset (e.g. -7 is MST).
 * DST - define to be your desired DST usage (e.g. true for MDT).
 */
#define POCKETWATCH
//#define VERBOSE
//#define CONTINUOUS
#define OFFSET (-7)
#define DST (true)

// include the library code:
#include <LiquidCrystal.h>
#include <DS3231.h>
#include <ES100.h>
#include <Wire.h>
#ifdef POCKETWATCH
#include <Pocketwatch.h>
#define S1 3
#define S2 6
#define S3 7
#endif


#define lcdRS 4
#define lcdEN 5
#define lcdD4 8
#define lcdD5 9
#define lcdD6 10
#define lcdD7 11
LiquidCrystal lcd(lcdRS, lcdEN, lcdD4, lcdD5, lcdD6, lcdD7);

DS3231 rtc(SDA, SCL);

#define es100Int 2
#define es100En 13
ES100 es100;
uint8_t     lp = 0;

unsigned long lastMillis = 0;
volatile unsigned long atomicMillis = 0;
unsigned long lastSyncMillis = 0;

volatile unsigned int interruptCnt = 0;
unsigned int lastinterruptCnt = 0;

boolean receiving = false;        // variable to determine if we are in receiving mode
boolean trigger = true;           // variable to trigger the reception
boolean continous = false;        // variable to tell the system to continously receive atomic time, if not it will happen every night at midnight
boolean validdecode = false;      // variable to rapidly know if the system had a valid decode done lately


Time t;
ES100DateTime d;
ES100Status0  status0;
ES100NextDst  nextDst;

#ifdef POCKETWATCH
Localtime l;
int8_t zoneOffset = OFFSET;
boolean dstInUse = DST;
boolean dstInEffect = false; /* Unknown until we get disciplined to WWVB. */
unsigned long nextMillis = 0;
unsigned long currMillis = 0;
Debouncer s1(!0); /* Active low. */
Debouncer s2(!0); /* Active low. */
Debouncer s3(!0); /* Active low. */
#endif

void atomic() {
  // Called procedure when we receive an interrupt from the ES100
  // Got a interrupt and store the currect millis for future use if we have a valid decode
  atomicMillis = millis();
  interruptCnt++;
}

char * getISODateStr() {
#ifdef POCKETWATCH
  static char result[21];
  boolean dst = dstInUse && dstInEffect;
  char zone = Localtime::getTimeZone(zoneOffset, false);
#else
  static char result[19];
#endif

  t=rtc.getTime();

#ifdef POCKETWATCH
  l.hour = t.hour;
  l.mon = t.mon;
  l.date = t.date;
  l.year = t.year;
  l.dow = t.dow;
#ifdef VERBOSE
  Serial.println(zoneOffset, DEC);
  Serial.println(dst, DEC);
  Serial.print(l.year, DEC);
  Serial.print('-');
  Serial.print(l.mon, DEC);
  Serial.print('-');
  Serial.print(l.date, DEC);
  Serial.print('T');
  Serial.print(l.hour, DEC);
  Serial.print('Z');
  Serial.println(l.dow, DEC);
#endif
  l.toLocalTime(zoneOffset, dst);
#ifdef VERBOSE
  Serial.print(l.year, DEC);
  Serial.print('-');
  Serial.print(l.mon, DEC);
  Serial.print('-');
  Serial.print(l.date, DEC);
  Serial.print('T');
  Serial.print(l.hour, DEC);
  Serial.print(zone);
  Serial.println(l.dow, DEC);
#endif
  t.hour = l.hour;
  t.mon = l.mon;
  t.date = l.date;
  t.year = l.year;
  t.dow = l.dow;
#endif

  result[0]=char((t.year / 1000)+48);
  result[1]=char(((t.year % 1000) / 100)+48);
  result[2]=char(((t.year % 100) / 10)+48);
  result[3]=char((t.year % 10)+48);
  result[4]=45;
  if (t.mon<10)
    result[5]=48;
  else
    result[5]=char((t.mon / 10)+48);
  result[6]=char((t.mon % 10)+48);
  result[7]=45;
  if (t.date<10)
    result[8]=48;
  else
    result[8]=char((t.date / 10)+48);
  result[9]=char((t.date % 10)+48);
  
  result[10]=84;

  if (t.hour<10)
    result[11]=48;
  else
    result[11]=char((t.hour / 10)+48);
  result[12]=char((t.hour % 10)+48);
  result[13]=58;
  if (t.min<10)
    result[14]=48;
  else
    result[14]=char((t.min / 10)+48);
  result[15]=char((t.min % 10)+48);
  result[16]=58;
  if (t.sec<10)
    result[17]=48;
  else
    result[17]=char((t.sec / 10)+48);
  result[18]=char((t.sec % 10)+48);
#ifdef POCKETWATCH
  result[19] = zone;
#else
  result[19]=90;
#endif
  result[20]=0;

  return result;
}

void displayDST() {
  lcd.print("DST ");
  switch (status0.dstState) {
    case B00:
#ifdef POCKETWATCH
      dstInEffect = false;
#endif
      lcd.print("is Not In Effect");
      break;
    case B10:
#ifdef POCKETWATCH
      dstInEffect = true;
#endif
      lcd.print("Begins Today");
      break;
    case B11:
#ifdef POCKETWATCH
      dstInEffect = true;
#endif
      lcd.print("is In Effect");
      break;
    case B01:
#ifdef POCKETWATCH
      dstInEffect = false;
#endif
      lcd.print("Ends Today");
      break;
  }
}

void displayNDST() {
  byte tmp = 0;
  lcd.print("NDST ");
  
  tmp = nextDst.month;
  if (tmp < 10) { lcd.print("0"); }
  lcd.print(tmp);
  lcd.print("/");

  tmp = nextDst.day;
  if (tmp < 10) { lcd.print("0"); }
  lcd.print(tmp);
  lcd.print(" ");

  tmp = nextDst.hour;
  if (tmp < 10) { lcd.print("0"); }
  lcd.print(tmp);
  lcd.print(":00");
}

void displayLeapSecond() {
  switch (status0.leapSecond) {
    case B00:
      lcd.print("No LS this month");
      break;
    case B10:
      lcd.print("Neg. LS this month");
      break;
    case B11:
      lcd.print("Pos. LS this month");
      break;
  }
}

void displayLastSync() {
  lcd.print("Last sync ");

  if (lastSyncMillis > 0) {
    int days =    (millis() - lastSyncMillis) / 86400000;
    int hours =   ((millis() - lastSyncMillis) % 86400000) / 3600000;
    int minutes = (((millis() - lastSyncMillis) % 86400000) % 3600000) / 60000;
    int seconds = ((((millis() - lastSyncMillis) % 86400000) % 3600000) % 60000) / 1000;

    if (days > 0) {
      if (days < 10) { lcd.print("0"); }
      lcd.print(days);
      lcd.print("d");
      if (hours < 10) { lcd.print("0"); }
      lcd.print(hours);
      lcd.print("h");
      if (minutes < 10) { lcd.print("0"); }
      lcd.print(minutes);
      lcd.print("m");
    } else {
      if (hours > 0) {
        if (hours < 10) { lcd.print("0"); }
        lcd.print(hours);
        lcd.print("h");
        if (minutes < 10) { lcd.print("0"); }
        lcd.print(minutes);
        lcd.print("m");
        if (seconds < 10) { lcd.print("0"); }
        lcd.print(seconds);
        lcd.print("s");
      } else {
        if (minutes > 0) {
          if (minutes < 10) { lcd.print("0"); }
          lcd.print(minutes);
          lcd.print("m");
          if (seconds < 10) { lcd.print("0"); }
          lcd.print(seconds);
          lcd.print("s");
        } else {
          if (seconds < 10) { lcd.print("0"); }
          lcd.print(seconds);
          lcd.print("s");
        }
      }
    }
  } else {
    lcd.print("never");
  }
}

void displayInterrupt() {
  lcd.print("Interrupt Count ");
  lcd.print(interruptCnt);
}

void displayAntenna() {
  lcd.print("Antenna used ");
  switch (status0.antenna) {
    case B0:
      lcd.print("1");
      break;
    case B1:
      lcd.print("2");
      break;
  }
}

void clearLine(unsigned int n) {
  while (n-- > 0)
    lcd.print(" ");
}

void showlcd() {
  lcd.setCursor(0,0);
  lcd.print(getISODateStr());

  if (validdecode) {
    // Scroll lines every 5 seconds.
    int lcdLine = (millis() / 5000 % 6) + 1;

    lcd.setCursor(0,1);
    clearLine(20);
    lcd.setCursor(0,1);
    switch (lcdLine) {
      case 1:
        displayInterrupt();
        break;
      case 2:
        displayLastSync();
        break;
      case 3:
        displayDST();
        break;
      case 4:
        displayNDST();
        break;
      case 5:
        displayLeapSecond();
        break;
      case 6:
        displayAntenna();
        break;
    }

    lcd.setCursor(0,2);
    clearLine(20);
    lcd.setCursor(0,2);
    switch (lcdLine) {
      case 6:
        displayInterrupt();
        break;
      case 1:
        displayLastSync();
        break;
      case 2:
        displayDST();
        break;
      case 3:
        displayNDST();
        break;
      case 4:
        displayLeapSecond();
        break;
      case 5:
        displayAntenna();
        break;
    }

    lcd.setCursor(0,3);
    clearLine(20);
    lcd.setCursor(0,3);
    switch (lcdLine) {
      case 5:
        displayInterrupt();
        break;
      case 6:
        displayLastSync();
        break;
      case 1:
        displayDST();
        break;
      case 2:
        displayNDST();
        break;
      case 3:
        displayLeapSecond();
        break;
      case 4:
        displayAntenna();
        break;
    }
  }
  else {
    lcd.setCursor(0,1);
    displayInterrupt();
    lcd.setCursor(0,2);
    clearLine(20);
    lcd.setCursor(0,3);
    clearLine(20);

  }
  // ToDo:
  //   Show rolling status of the following informations :
  //   Interrupt Count xxxx  /* Where x = 0 to 9999 */
  //   Last sync xxxxxx ago  /* Where x = 59s / 59m59s / 23h59m / 49d23h */
  //   DST xxxxxxxxxxxxxxxx  /* Day Light Saving : Where x = is Not In Effect / Begins Today / is In Effect / Ends Today */
  //   NDST xxxxxxxxxxxxxxx  /* Next DST : Where x = 2019-11-03T2h00 */
  //   xxxxxxxxxxxxxxxxxxxx  /* Leap Second : Where x = No LS this month / Neg. LS this month / Pos. LS this month */
  //   Antenna used x    /* Antenna Used for reception where x = 1 or 2 */

}

void setup() {
  Wire.begin();
  Serial.begin(9600);
  es100.begin(es100Int, es100En);
  lcd.begin(20, 4);
  lcd.clear();
  rtc.begin();

#ifdef POCKETWATCH
  pinMode(S1, INPUT_PULLUP);
  pinMode(S2, INPUT_PULLUP);
  pinMode(S3, INPUT_PULLUP);
#ifdef CONTINUOUS
  continous = true;
#endif
#endif

  attachInterrupt(digitalPinToInterrupt(es100Int), atomic, FALLING);
}

void loop() {
  if (!receiving && trigger) {
    interruptCnt = 0;
    
    es100.enable();
    es100.startRx();
    
    receiving = true;
    trigger = false;

    /* Important to set the interrupt counter AFTER the startRx because the es100 
     * confirm that the rx has started by triggering the interrupt. 
     * We can't disable interrupts because the wire library will stop working
     * so we initialize the counters after we start so we can ignore the first false 
     * trigger
     */
    lastinterruptCnt = 0;
    interruptCnt = 0;
  }

  if (lastinterruptCnt < interruptCnt) {
    Serial.print("ES100 Interrupt received... ");
  
    if (es100.getIRQStatus() == 0x01 && es100.getRxOk() == 0x01) {
      validdecode = true;
      Serial.println("Valid decode");
      // Update lastSyncMillis for lcd display
      lastSyncMillis = millis();
      // We received a valid decode
      d = es100.getDateTime();
      // Updating the RTC
      rtc.setDate(d.day, d.month, 2000+d.year);
      rtc.setTime(d.hour, d.minute, d.second + ((millis() - atomicMillis)/1000));

      // Get everything before disabling the chip.
      status0 = es100.getStatus0();
      nextDst = es100.getNextDst();
  
/* DEBUG */
      Serial.print("status0.rxOk = B");
      Serial.println(status0.rxOk, BIN);
      Serial.print("status0.antenna = B");
      Serial.println(status0.antenna, BIN);
      Serial.print("status0.leapSecond = B");
      Serial.println(status0.leapSecond, BIN);
      Serial.print("status0.dstState = B");
      Serial.println(status0.dstState, BIN);
      Serial.print("status0.tracking = B");
      Serial.println(status0.tracking, BIN);
/* END DENUG */
  
      if (!continous) {
        es100.stopRx();
        es100.disable();
        receiving = false;
      }
    }
    else {
      Serial.println("Invalid decode");
    }
    lastinterruptCnt = interruptCnt;
  }

#ifdef POCKETWATCH
    currMillis = millis();
    if (currMillis >= nextMillis) {
      s1.debounce(digitalRead(S1));
      if (s1.edge() == Debouncer::IS_RISING) {
        Serial.println("S1");
        zoneOffset += 1;
      }
      s2.debounce(digitalRead(S2));
      if (s2.edge() == Debouncer::IS_RISING) {
        Serial.println("S2");
        dstInUse = !dstInUse;
      }
      s3.debounce(digitalRead(S3));
      if (s3.edge() == Debouncer::IS_RISING) {
        Serial.println("S3");
        zoneOffset -= 1;
      }
      nextMillis = currMillis + 10;
    }
#endif
 
  if (lastMillis + 100 < millis()) {
    showlcd();

#ifdef POCKETWATCH
    // Set trigger to start reception at 2AM local time if we are not in continuous mode
    // (when DST is taken into and out of effect). Note that at this point, the Time
    // object has local time in it, not UTC.
    trigger = (!continous) && (!receiving) && (t.hour == 2) && (t.min == 0); 
#else
    // set the trigger to start reception at midnight (UTC-4) if we are not in continous mode.
    // 4am UTC is midnight for me, adjust to your need
    trigger = (!continous && !receiving && t.hour == 4 && t.min == 0); 
#endif
    
    lastMillis = millis();
  }
}
