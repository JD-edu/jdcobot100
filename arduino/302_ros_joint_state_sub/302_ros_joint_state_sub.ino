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

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <U8x8lib.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

#define pulse_us_min 500            // 최소 펄스 길이 ms = 1000 µs
#define pulse_us_max 2450           // 최대 펄스 길이 ms = 1000 µs
#define servo_freq 50               // 서보 주파수 (Hz)
#define servo_driver_bits 4096

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define base 0
#define shoulder 1
#define upperarm 2
#define forearm 3
#define gripper 4

double baseAngle = 90;
double shoulderAngle = 90;
double upperarmAngle = 90;
double forearmAngle = 90;
double gripperAngle = 90;
double motor_step = 0.5;


int delay_time = 15;
int delay_time_setup = 0;

double angle_list[5] = {baseAngle, shoulderAngle, upperarmAngle, forearmAngle, gripperAngle};

long period_us = 1000000 / servo_freq; // 주기 (µs)
long pulse_min = (pulse_us_min / (double)period_us) * servo_driver_bits;
long pulse_max = (pulse_us_max / (double)period_us) * servo_driver_bits;

bool stop_flag = false;

String base_str;
String shoulder_str;
String upperarm_str;
String forearm_str;
String gripper_str;
String inString;

void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(servo_freq);
  delay(10);
}

void loop() {
  delay(100);
  if (Serial.available() > 0) {
    inString = Serial.readStringUntil('\n');
    char cmd = inString[0];
    
    
  
   
      // 각도 값 파싱
      baseAngle = inString.substring(inString.indexOf('a') + 1, inString.indexOf('b')).toInt();
      shoulderAngle = inString.substring(inString.indexOf('b') + 1, inString.indexOf('c')).toInt();
      upperarmAngle = inString.substring(inString.indexOf('c') + 1, inString.indexOf('d')).toInt();
      forearmAngle = inString.substring(inString.indexOf('d') + 1, inString.indexOf('e')).toInt();
      gripperAngle = inString.substring(inString.indexOf('e') + 1, inString.indexOf('f')).toInt();
      
      baseAngle = constrain(baseAngle,0,180);
      shoulderAngle = constrain(shoulderAngle,0,180);
      upperarmAngle = constrain(upperarmAngle,0,180);
      forearmAngle = constrain(forearmAngle,0,180);
      gripperAngle = constrain(gripperAngle,0,180);

      set_servo(base, baseAngle);
      set_servo(shoulder, shoulderAngle);
      set_servo(upperarm, upperarmAngle);
      set_servo(forearm, forearmAngle);
  
  }
}

void set_servo(uint8_t n, double angle) {
  double pulse = map(angle, 0, 180, pulse_min, pulse_max);
  pwm.setPWM(n, 0, pulse);
}
