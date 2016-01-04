/*********************************************************************
 * Frame Flipper
 *
 * Adaptation of the frame transition portion of Daniel Eichorn's SSD1306
 * library to work with Adafruit's GFX/SSD1306 libraries.
 *********************************************************************/

/**The MIT License (MIT)

Copyright (c) 2015 by Daniel Eichhorn
Portions copyright (c) 2016 by Mark Walker

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

class FrameFlipper {

private:
   Adafruit_SSD1306 *myDisplay;
   int myFrameState = 0;
   int myFrameTick = 0;
   int myCurrentFrame = 0;
   int myFrameCount = 0;
   int myFrameWaitTicks = 800;
   int myFrameTransitionTicks = 250;
   int myIndicators = false;
   int myColor = WHITE;
   void (**myFrameCallbacks)(int x, int y);

   
public:
   FrameFlipper(Adafruit_SSD1306 *display);
   void setFrameCallbacks(int frameCount, void (*frameCallbacks[])(int x, int y));
   void nextFrameTick(void);
   void drawIndicators(int frameCount, int activeFrame);
   void setFrameWaitTicks(int frameWaitTicks);
   void setFrameTransitionTicks(int frameTransitionTicks);
   void setIndicators(int indicators);
   int getFrameState();

   const int FRAME_STATE_FIX = 0;
   const int FRAME_STATE_TRANSITION = 1;

};
