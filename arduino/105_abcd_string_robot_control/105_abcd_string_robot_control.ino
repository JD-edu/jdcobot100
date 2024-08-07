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

  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0, 1);
  u8x8.print("          ");
  u8x8.setCursor(0, 2);
  u8x8.print("jdcobot 100");
  u8x8.setCursor(0, 3);
  u8x8.print("standby");
  u8x8.setCursor(0, 4);
  u8x8.print("          ");

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
    if (cmd == '1') {
      clear_oled();
      u8x8.drawString(0, 0, "read angles");
      serial_print();
  
    } else if (cmd == '2') {
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
      
      serial_print();

      stop_flag = false;
      while (!stop_flag) {
        bool done = true;
        done &= moveServo(base, baseAngle, motor_step);
        done &= moveServo(shoulder, shoulderAngle, motor_step);
        done &= moveServo(upperarm, upperarmAngle, motor_step);
        done &= moveServo(forearm, forearmAngle, motor_step);
        done &= moveServo(gripper, gripperAngle, motor_step);
        if (done) break;
        delay(delay_time);
      }
      display_angles();
    } else if (cmd == '3') {
      resetServos();
      serial_print();

    } else if (cmd == '4') {
      clear_oled();
      u8x8.drawString(0, 0, "stop servo motors");
      stop_flag = true;
      serial_print();
    }
  }
}

bool moveServo(uint8_t servo_num, double targetAngle, double step) {
  if (stop_flag) {
    return true; // 정지 명령이 들어오면 현재 상태 유지
  }

  double currentAngle = angle_list[servo_num];
  if (currentAngle < targetAngle) {
    set_servo(servo_num, currentAngle + step);
    angle_list[servo_num] = currentAngle + step; // 현재 각도 업데이트
    return false;
  } else if (currentAngle > targetAngle) {
    set_servo(servo_num, currentAngle - step);
    angle_list[servo_num] = currentAngle - step; // 현재 각도 업데이트
    return false;
  }
  return true;
}

void resetServos() {
  bool done;
  do {
    done = true;
    done &= moveServo(base, 90, motor_step);
    done &= moveServo(shoulder, 92, motor_step);
    done &= moveServo(upperarm, 90, motor_step);
    done &= moveServo(forearm, 90, motor_step);
    done &= moveServo(gripper, 90, motor_step);
    delay(delay_time);
  } while (!done);
}

void display_angles() {
  clear_oled();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0, 0, ("sig: " + inString.substring(0, 10)).c_str());
  u8x8.drawString(0, 1, ("     " + inString.substring(10)).c_str());
  u8x8.drawString(0, 2, ("base:      " + String((int)baseAngle)).c_str());
  u8x8.drawString(0, 3, ("shoulder:  " + String((int)shoulderAngle)).c_str());
  u8x8.drawString(0, 4, ("upperArm:  " + String((int)upperarmAngle)).c_str());
  u8x8.drawString(0, 5, ("foreArm:   " + String((int)forearmAngle)).c_str());
  u8x8.drawString(0, 6, ("gripper:   " + String((int)gripperAngle)).c_str());
  delay(100);
}

void clear_oled() {
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  for (int i = 0; i < 7; i++) {
    u8x8.drawString(0, i, "                   ");
  }
  delay(100);
}

void set_servo(uint8_t n, double angle) {
  double pulse = map(angle, 0, 180, pulse_min, pulse_max);
  pwm.setPWM(n, 0, pulse);
}

void serial_print() {
  Serial.print("a");
  Serial.print(baseAngle);
  Serial.print(" b");
  Serial.print(shoulderAngle);
  Serial.print(" c");
  Serial.print(upperarmAngle);
  Serial.print(" d");
  Serial.print(forearmAngle);
  Serial.print(" e");
  Serial.print(gripperAngle);
  Serial.println(" f");
}
