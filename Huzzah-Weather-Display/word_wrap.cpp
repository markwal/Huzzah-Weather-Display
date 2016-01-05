/**The MIT License (MIT)

Copyright (c) 2016 by Mark Walker

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

*/

/*******************************************************************
 * Word Wrapper
 *******************************************************************/

#include <Adafruit_GFX.h>
#include "word_wrap.h"

// word's longer than MAX_WORD_LEN are assumed to need a whole line to themselves
#define MAX_WORD_LEN 256

// side effect: leaves display.setTextWrap(false) since there is currently no public
// function to discover the setting prior to this call
void drawTextWordWrapped(Adafruit_GFX &display, int16_t x, int16_t y, int16_t width, String string)
{
    char word[MAX_WORD_LEN];
    int16_t left = x;
    int16_t right = x + width;
    int16_t x1 = 0, y1 = 0;
    uint16_t word_width = 0, word_height = 0;

    display.setTextWrap(false);

    // make sure word_height is set in case we start with newlines
    display.getTextBounds("M", 0, 0, &x1, &y1, &word_width, &word_height);

    const char *pch = string.c_str();
    char *pword = word;
    while (*pch) {
        while (*pch == '\n') {
            x = left;
            y += word_height;
            pch++;
            Serial.println("\\n");
        }

        pword = word;
        int ich = 0;
        while (*pch == ' ' && ich < sizeof(word)) {
            word[ich++] = *pch++;
        }
        while (*pch && *pch != ' ' && *pch != '\n' && ich < sizeof(word)) {
            word[ich++] = *pch++;
        }
        word[ich] = 0;

        display.getTextBounds(pword, 0, 0, &x1, &y1, &word_width, &word_height);
        if (x > left && x + word_width >= right) {
            x = left;
            y += word_height;

            // since we wrapped, eat the leading spaces.
            while (*pword == ' ')
                pword++;
            display.getTextBounds(pword, 0, 0, &x1, &y1, &word_width, &word_height);
        }

        display.setCursor(x, y);
        display.print(pword);
        x += word_width;
    }
}
