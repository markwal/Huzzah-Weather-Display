/**The MIT License (MIT)

Copyright (c) 2015 by Daniel Eichhorn

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

See more at http://blog.squix.ch
*/

#include <SPI.h>
#include <Wire.h>
#include <Ticker.h>
#include <ESP8266WiFi.h>
#include <Adafruit_GFX.h>

#define SSD1306_128_64
#include <Adafruit_SSD1306.h>

#include "icons.h"

#include "WeatherClient.h"
#include "frame_flipper.h"

#include <Fonts/FreeSans9pt7b.h>

#define OLED_MOSI  13
#define OLED_CLK   14
#define OLED_DC    2
#define OLED_CS    5
#define OLED_RESET 4

// software SPI
// Uncomment the following line for software SPI (and comment out the others)
//Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

// hardware SPI
// hardware SPI on the Huzzah ESP8266 is: MOSI 13, CLK 14, DC 2, CS 5, RESET 4
// Uncomment the following line for hardware SPI (and comment out the others)
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

// I2C
// Uncomment the following lines for i2c (and comment out the others)
// Note: currently Adafruit's SSD1306 doesn't support specifying SDA and SCL
// so only works with the default's for your hardware
// I think the default on the Huzzah ESP8266 is: SDA 4 SCL 5
// so you probably need to move OLED_RESET to somewhere else also
// #define I2C 0x3D
// Adafruit_SSD1306 display(OLED_RESET)

#define WIFISSID "YOUR_WIFI_SSID"
#define PASSWORD "YOUR_WIFI_PASSWORD"

#define FORECASTAPIKEY "YOUR_FORECAST_API_KEY"
#define DOMAINNAME "YOUR_DOMAIN_NAME"
#define PORT 80

// New York City
#define LATITUDE 40.71
#define LONGITUDE -74

FrameFlipper flipper(&display);
WeatherClient weather;
Ticker ticker;

// this array keeps function pointers to all frames
// frames are the single views that slide from right to left
void drawFrame1(int x, int y);
void drawFrame2(int x, int y);
void drawFrame3(int x, int y);
void drawSpinner(int count, int active);
void (*frameCallbacks[])(int x, int y) = {drawFrame1, drawFrame2, drawFrame3};

// how many frames are there?
int frameCount = 3;
// on frame is currently displayed
int currentFrame = 0;

// your network SSID (name)
char ssid[] = WIFISSID;

// your network password
char pass[] = PASSWORD;

// Go to forecast.io and register for an API KEY
String forecastApiKey = FORECASTAPIKEY;

// website domain name
String webDomain = DOMAINNAME;

// Coordinates of the place you want
// weather information for
double latitude = LATITUDE;
double longitude = LONGITUDE;

// flag changed in the ticker function every 10 minutes
bool readyForWeatherUpdate = true;


void setReadyForWeatherUpdate() {
  readyForWeatherUpdate = true;
}

const uint8_t *getIconFromString(String icon) {
  //"clear-day, clear-night, rain, snow, sleet, wind, fog, cloudy, partly-cloudy-day, or partly-cloudy-night"
  if (icon == "clear-day") {
    return clear_day_bits;
  } else if (icon == "clear-night") {
    return clear_night_bits;
  } else if (icon == "rain") {
    return rain_bits;
  } else if (icon == "snow") {
    return snow_bits;
  } else if (icon == "sleet") {
    return sleet_bits;
  } else if (icon == "wind") {
    return wind_bits;
  } else if (icon == "fog") {
    return fog_bits;
  } else if (icon == "cloudy") {
    return cloudy_bits;
  } else if (icon == "partly-cloudy-day") {
    return partly_cloudy_day_bits;
  } else if (icon == "partly-cloudy-night") {
    return partly_cloudy_night_bits;
  }
  return cloudy_bits;
}

void drawFrame1(int x, int y) {
  display.drawXBitmap(x, 5 + y, getIconFromString(weather.getCurrentIcon()), 50, 50, WHITE);
  display.setTextSize(1);
  display.setCursor(55 + x, 0 + y);
  display.setTextLeftMargin(55 + x);
  display.setTextRightMargin(128 + x);
  display.println("Now");
  display.setFont(&FreeSans9pt7b);
  display.println(String(weather.getCurrentTemp()) + "F");
  display.setFont();
  display.println(String(weather.getCurrentHumidity()) + "% humidity");
  display.println(String(weather.getCurrentSummary()));
}

void drawFrame2(int x, int y) {
  display.drawXBitmap(x, 5 + y, getIconFromString(weather.getIconToday()), 50, 50, WHITE);
  display.setTextSize(1);
  display.setCursor(55 + x, 0 + y);
  display.setTextLeftMargin(55 + x);
  display.setTextRightMargin(128 + x);
  display.println("Today");
  display.println(String(weather.getMaxTempToday()) + "F/" + String(weather.getMinTempToday()) + "F");
  display.setCursor(55 + x, display.getCursorY() + 4);
  display.println(String(weather.getSummaryToday()));
}

void drawFrame3(int x, int y) {
  display.drawXBitmap(x, 5 + y, getIconFromString(weather.getIconTomorrow()), 50, 50, WHITE);
  display.setTextSize(1);
  display.setCursor(55 + x, 0 + y);
  display.setTextLeftMargin(55 + x);\
  display.setTextRightMargin(128 + x);
  display.println("Tomorrow");
  display.println(String(weather.getMaxTempTomorrow()) + "F/" + String(weather.getMinTempTomorrow()) + "F");
  display.setCursor(55 + x, display.getCursorY() + 4);
  display.println(String(weather.getSummaryTomorrow()));
}

void drawSpinner(int count, int active) {
  for (int i = 0; i < count; i++) {
    const uint8_t *xbm;
    if (active == i) {
      xbm = active_bits;
    } else {
      xbm = inactive_bits;
    }
    display.drawXBitmap(64 - (12 * count / 2) + 12 * i, 56, xbm, 8, 8, WHITE);
  }
}

void setup() {

#ifdef I2C
  display.begin(SSD1306_SWITCHCAPVCC, I2C);
#else
  display.begin(SSD1306_SWITCHCAPVCC); // generate high voltage from the 3.3v line
  display.display();

  Serial.begin(115200);
  delay(500);
  Serial.println();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  // set the drawing functions
  flipper.setFrameCallbacks(3, frameCallbacks);
  // how many ticks does a slide of frame take?
  flipper.setFrameTransitionTicks(100);
  flipper.setFrameWaitTicks(800);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

    display.clearDisplay();
    display.drawXBitmap(34, 10, WiFi_Logo_bits, 60, 36, WHITE);
    display.fillRect(10, 10, 108, 44, INVERSE);
    drawSpinner(3, counter % 3);
    display.display();

    counter++;
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // update the weather information every 10 mintues only
  // forecast.io only allows 1000 calls per day
  ticker.attach(60 * 10, setReadyForWeatherUpdate);

}

void loop() {

  if (readyForWeatherUpdate && flipper.getFrameState() == flipper.FRAME_STATE_FIX) {
    readyForWeatherUpdate = false;
    weather.updateWeatherData(webDomain, PORT, forecastApiKey, latitude, longitude);
  }

  display.clearDisplay();
  flipper.nextFrameTick();
  display.display();
}

