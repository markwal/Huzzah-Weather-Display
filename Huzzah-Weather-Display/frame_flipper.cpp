/**The MIT License (MIT)

Copyright (c) 2015 by Daniel Eichhorn
Portions Copyright (c) 2015 by Mark Walker

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

Credits for parts of this code go to Mike Rankin. Thank you so much for sharing!
*/

/*******************************************************************
 * Frame Flipper
 *
 * Adaptation of the frame transition portion of Daniel Eichhorn's SSD1306
 * library to work with Adafruit's GFX/SSD1306 libraries
 *******************************************************************/

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "frame_flipper.h"


const uint8_t active_bits[] PROGMEM = {
   0x00, 0x18, 0x3c, 0x7e, 0x7e, 0x3c, 0x18, 0x00 };

const uint8_t inactive_bits[] PROGMEM = {
   0x00, 0x18, 0x24, 0x42, 0x42, 0x24, 0x18, 0x00 };


FrameFlipper::FrameFlipper(Adafruit_SSD1306 *display)
{
  myDisplay = display;
}


void FrameFlipper::nextFrameTick() {
  myFrameTick++;
  if (myFrameTick == myFrameWaitTicks && myFrameState == 0 || myFrameTick == myFrameTransitionTicks && myFrameState == 1) {
    myFrameState = (myFrameState + 1) %  2;
    if (myFrameState == FRAME_STATE_FIX) {
      myCurrentFrame = (myCurrentFrame + 1) % myFrameCount;
    }
    myFrameTick = 0;
  }
  if (myIndicators)
    drawIndicators(myFrameCount, myCurrentFrame);

  int16_t x, w;
  switch (myFrameState) {
    case 0:
      (*myFrameCallbacks[myCurrentFrame])(0, 0);
      break;
    case 1:
      w = myDisplay->width();
      x = -w * myFrameTick / myFrameTransitionTicks;
      myDisplay->setTextRightMargin(x + w);
      (*myFrameCallbacks[myCurrentFrame])(x, 0);
      x += w;
      myDisplay->setTextRightMargin(x + w);
      (*myFrameCallbacks[(myCurrentFrame + 1) % myFrameCount])(x, 0);
      break;
  }

}


void FrameFlipper::drawIndicators(int frameCount, int activeFrame) {
  for (int i = 0; i < frameCount; i++) {
    const uint8_t *xbm;
    if (activeFrame == i) {
      xbm = active_bits;
    } else {
      xbm = inactive_bits;
    }
    int16_t c = myDisplay->width() / 2;
    myDisplay->drawBitmap(c - (12 * frameCount / 2) + 12 * i, c - 8, xbm, 8, 8, WHITE);
  }
}


void FrameFlipper::setFrameCallbacks(int frameCount, void (*frameCallbacks[])(int x, int y)) {
  myFrameCount = frameCount;
  myFrameCallbacks = frameCallbacks;
}


void FrameFlipper::setFrameWaitTicks(int frameWaitTicks) {
  myFrameWaitTicks = frameWaitTicks;
}


void FrameFlipper::setFrameTransitionTicks(int frameTransitionTicks) {
  myFrameTransitionTicks = frameTransitionTicks;
}


void FrameFlipper::setIndicators(int indicators) {
  myIndicators = indicators;
}

int FrameFlipper::getFrameState() {
  return myFrameState;
}
