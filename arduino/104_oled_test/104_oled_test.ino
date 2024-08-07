/*MIT License

Copyright (c) 2024 JD edu. http://jdedu.kr author: conner.jeong@gmail.com
     
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
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN TH
SOFTWARE.*/
#include <U8x8lib.h>  // install U8g2 library

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

String upperarm_str = "world";

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE); 

void clear_oled(){
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0,0,"                   ");
  u8x8.drawString(0,1,"                   ");
  u8x8.drawString(0,2,"                   ");
  u8x8.drawString(0,3,"                   ");
  delay(100);
}

void setup() {
  Serial.begin(115200);
  u8x8.begin();
  clear_oled();
}

void loop() {
  clear_oled();
  delay(1000);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0,0,"hello world");
  u8x8.drawString(0,1,upperarm_str.c_str());
  delay(1000);

}
