Huzzah Weather Display
================

I modified andydoro's version to:

* Use the Adafruit 1.54" 128x64 OLED graphic display https://www.adafruit.com/products/2720
    (because the 1.3" was out of stock)
* Also, switched to hardware SPI because the 1.5" overview page recommended it
* Show the forecast.io summary on each panel (now, today, tomorrow)
* Do word wrap on the summary
* Use the Adafruit_GFX library

This version requires the latests (unreleased) version of Adafruit_GFX library
because word wrapping makes use of the new function getTextBounds.  Also, we
need a fix to that library so that it counts the last line's width.

I also added stl's for my 3d printed case.

For a tutorial and more information see https://learn.adafruit.com/huzzah-weather-display
 
Modified from https://github.com/squix78/esp8266-projects/tree/master/arduino-ide/weather-station-v2 by Daniel Eichhorn to work with Adafruit hardware.

The weather icons are from http://www.alessioatzeni.com/meteocons/ Alessio Attzeni is accepting donations for his work.
 
 
Hardware:

- Huzzah ESP8266 Breakout https://www.adafruit.com/product/2471
- Monochrome 1.3" 128x64 OLED graphic display https://www.adafruit.com/products/938
- USB micro-B breakout https://www.adafruit.com/products/1833
 
 
Acknowledgements:
  - Thanks Dano for lasercut enclosure! https://github.com/danowall
 

