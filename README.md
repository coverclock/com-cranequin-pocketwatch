# com-cranequin-pocketwatch
Modifications to the Universal Solder EverSet ES100-MOD Application Development Kit
# Abstract
This repository contains source code from the Universal Solder web site with changes I made so that the time zone can
be adjusted forwards and backwards using the S1 and S3 buttons, and to enable or disable the use of Daylight Saving Time (DST)
using the S2 button. The local time will not be adjusted for DST until the clock receives a valid decode of the WWVB signal,
which contains a flag indicating whether DST is in effect or not. The time zone and the use of DST will have to be readminstered
every time the clock powers up and returns back to the defaults of UTC and no DST.
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
