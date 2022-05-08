# com-cranequin-pocketwatch
Modifications to the Universal Solder EverSet ES100-MOD Application Development Kit
# License
This repository contains source files from various developers and
organizations, and which are licensed under a variety of terms.
The murkiness of the licensing is why this project was done under
the auspices of Cranequin LLC (my closed source company) instead
of Digital Aggregates Corporation (my open source and consulting
company).
# Abstract
This repository contains source code from the Universal Solder web
site with changes I made so that the time zone can be adjusted
forwards and backwards using the S1 and S3 buttons, and to enable
or disable the use of Daylight Saving Time (DST) using the S2 button.
The local time will not be adjusted for DST until the clock receives
a valid decode of the WWVB signal, which contains a flag indicating
whether DST is in effect or not. The time zone and the use of DST
will have to be readminstered every time the clock powers up and
returns back to the defaults of UTC and no DST.

My changes are limited to modifications to the original Arduino
sketch, and the addition of the Pocketwatch Arduino library. The
DS3231 (for the real-time clock) and ES100 (for the radio receiver)
Arduino libraries are provided unchanged from what I downloaded
from the Universal Solder web site.

If you look at the Arduino sketch that runs in the AVR microcontroller,
and peruse my changes, you will see that you can make the default
time zone, and the default DST enable/disable state, anything you want.
So you can set it to your own time zone and DST usage so that it
powers up in the correct state. (I left mine set to UTC and no DST, because
I like having an excuse to play with the buttons.)

Included in this repository is are unit tests that can test
portions of the Pocketwatch library without any hardware. It should
work on virtually any Linux system. I used it on my desktop Mac.
It might even work on Windows in a variety of UNIX-like environments.
# Installation
* Connect 5V power to your assembled ES100-MOD ADK board.
* Connect a 5V (IMPORTANT NOT 3.3V) FTDI USB-to-TTL serial adapter (or similar) to the TX and RX pins (you will probably need to reverse RX and TX) and a GND pin (I used the one on the ICSP header). If your cable has a DTR output wire (mine doesn't), connect that to the RESET pin on the board.
* Bring up the Arduino IDE.
* In the Tools->Board dialog select "Arduino Uno".
* In the Tools->Port dialog select the serial port representing the enumerated FTDI USB-to-TTL serial adapter.
* Use the Sketch->Include Library->Add .ZIP Library dialog to import the directories of the DS3231, ES100, and Pocketwatch Arduino libraries under the ```libraries``` directory.
* Use the File->Open... dialog to open the Pocketwatch.ino Arduino sketch in the ```sketches``` directory.
* Compile the new libraries and sketch using the Check button.
* Providing that works, use the Right Arrow button to upload the resulting binary image to your ES100-MOD ADK board. If your cable doesn't have a DTR output wire (which the IDE uses to reset the board at just the right time), you will need to press and release the Reset button on the board at just the right time: not too soon (or the bootloader on the board will have already given up) and not too late (or the uploader in the IDE will have already given up); it might take some practice.
* That's it. The board comes up in almost no time at all after uploading or subsequent power up. Use the S1 and S3 buttons to non-persistently set your local time zone (which is indicated using the NATO nomenclature, e.g. Z or "Zulu" for UTC, T or "Tango" for my own MST), and the S2 button to non-persistently enable (or disable) the use of Daylight Saving Time (DST) if your state uses it (not all do).
* The correct local time will not be displayed until the board successfully receives and decodes a packet from the WWVB radio station in Fort Collins Colorado. This can take a while (minutes).
# Contact
John Sloan    
Cranequin LLC   
3440 Youngfield Street, Suite 209    
Wheat Ridge, Colorado 80033 USA    
<mailto:jsloan@cranequin.com>    
<http://www.cranequin.com>    
# Links
<https://universal-solder.ca/product/canaduino-application-development-kit-with-everset-es100-mod-wwvb-bpsk-atomic-clock-receiver-module/>
# Articles
Chip Overclock, "Pocketwatch", 2022-04-23, <https://coverclock.blogspot.com/2022/04/pocketwatch.html>
