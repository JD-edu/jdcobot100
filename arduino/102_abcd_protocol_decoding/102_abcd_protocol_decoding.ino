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
int baseAngle = 90;
int shoulderAngle = 90;
int upperarmAngle = 90;
int forearmAngle = 90;

String baseAngle_str;
String shoulder_str;
String upperarm_str;
String forearm_str;
String inString;

void setup() {
   Serial.begin(115200);
}

void loop() {
  if(Serial.available()){
    inString = Serial.readStringUntil('\n');
    int first = inString.indexOf('a');
    int second = inString.indexOf('b');
    baseAngle_str = inString.substring(first+1, second);
    baseAngle = baseAngle_str.toInt();    
    Serial.print(baseAngle);
    Serial.print(" ");
    // angle 1
    first = inString.indexOf('b');
    second = inString.indexOf('c');
    shoulder_str = inString.substring(first+1, second);
    shoulderAngle = shoulder_str.toInt();
    Serial.print(shoulderAngle);
    Serial.print(" ");
    // angle 2
    first = inString.indexOf('c');
    second = inString.indexOf('d');
    upperarm_str = inString.substring(first+1, second);
    upperarmAngle = upperarm_str.toInt();
    Serial.print(upperarmAngle); 
    Serial.print(" ");
    // angle 3
    first = inString.indexOf('d');
    second = inString.indexOf('e');
    forearm_str = inString.substring(first+1, second);
    forearmAngle = forearm_str.toInt();
    Serial.print(forearmAngle);
    Serial.println(" ");
  }
}
