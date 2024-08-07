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
#include<Servo.h>

Servo base;
Servo shoulder;
Servo upperarm;
Servo forearm;
Servo gripper;

int baseAngle = 90;
int shoulderAngle = 90;
int upperarmAngle = 90;
int forearmAngle = 90;
int gripperAngle = 90;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  base.attach(3);
  base.write(baseAngle);
  shoulder.attach(5);
  shoulder.write(shoulderAngle);
  forearm.attach(6);
  forearm.write(forearmAngle);
  upperarm.attach(9);
  upperarm.write(upperarmAngle);

}

void loop() {
  base.write(90);
  shoulder.write(90);
  forearm.write(90);
  upperarm.write(90);
  delay(2000);
}
