## jdcobot100 아두이노 코드 설명
### 목차
[101_servo_center](#101_servo_center) 관절 초기화   
[102_abcd_protocol_decoding](#102_abcd_protocol_decoding) 시리얼 통신 확인   
[103_robotarm_servo_test.ino](#103_robotarm_servo_testino) 모터 각도 변화 및 병렬 제어 테스트   
[104_oled_test.ino](#104_oled_testino) SSD1306 OLED 테스트   
[105_abcd_string_robot_control.ino](#105_abcd_string_robot_controlino) 관절 제어, OLED에 현재 상태 출력   
[302_ros_joint_state_sub.ino](#302_ros_joint_state_subino) 시리얼 통신으로 로봇암 제어   

### 101_servo_center

JDCobot100의 각 관절을 기본 상태로 설정하기 위한 코드입니다.   
서보 정렬, 동작 테스트 등에 사용하시길 바랍니다.   

1. 라이브러리 추가 및 서보 드라이버 객체 생성
```c
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = adafruit_PWMServoDriver();
```
  JDCobot100은 서보 모터 제어를 위해 Adafruit의 라이브러리를 사용합니다.    
  [여기](https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library.git)에서 라이브러리를 다운받은 후 사용해주세요.   

2. 서보 관련 설정 값 정의
```c
#define pulse_us_min 500            // 최소 펄스 길이 ms = 1000 µs
#define pulse_us_max 2450           // 최대 펄스 길이 ms = 1000 µs
#define servo_freq 50               // 서보 주파수 (Hz)
#define servo_driver_bits 4096
```
  서보 모터의 최소•최대 펄스 길이, 작동 주파수, 드라이버의 분해능을 정의하여 정확한 모터 제어가 가능하도록 설정합니다.   

```c
#define base 0
#define shoulder 1
#define upperarm 2
#define forearm 3
#define gripper 4
```
  각 관절에서 사용하는 서보 모터의 포트 번호를 정의합니다.   

3. 각도 및 딜레이 설정

```c
double baseAngle = 90;
double shoulderAngle = 90;
double upperarmAngle = 90;
double forearmAngle = 90;
double gripperAngle = 180;
double motor_step = 0.5;
```
  각 관절의 기본 각도를 설정하고 각도 변화 단위(motor_step)를 지정합니다.   

```c
int delay_time = 15;
int delay_time_setup = 0;
```
  딜레이 시간을 설정하여 서보 모터가 부드럽게 움직이도록 합니다.   

```c
double angle_list[5] = {baseAngle, shoulderAngle, upperarmAngle, forearmAngle, gripperAngle};
```
  각 관절의 초기 각도를 배열로 정의하여 효율적으로 관리합니다.   

4. 펄스 계산

```c
long period_us = 1000000 / servo_freq; // 주기 (µs)
```
  주기를 계산합니다.   

```c
long pulse_min = (pulse_us_min / (double)period_us) * servo_driver_bits;
long pulse_max = (pulse_us_max / (double)period_us) * servo_driver_bits;
```
  서보가 올바른 범위 내에서 움직이기 위해 펄스 폭을 드라이버 분해능(4096)에 맞게 변환해야 합니다.   
  변환을 위해 펄스 폭의 최소•최대값을 계산합니다.   

5. 서보 모터 제어 함수
```c
void set_servo(uint8_t n, double angle) {
  double pulse = map(angle, 0, 180, pulse_min, pulse_max);
  pwm.setPWM(n, 0, pulse);
}
```   
6. setup
```c
void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(servo_freq);
  delay(10);
 
}
```
  시리얼 통신 설정, 서보 드라이버 초기화, 드라이버의 오실레이터 주파수 설정, 서보 모터의 주파수 설정 등 초기화를 진행합니다.   

7. loop
```c
void loop() {
  for (int i = 0; i < 5; i++){
    set_servo(i, angle_list[i]);
    delay(delay_time);
  }
  delay(2000);
}
```
  각 관절을 초기 각도로 설정합니다.   


### 102_abcd_protocol_decoding

이 코드는 시리얼로 `a90b90c90d90e` 형태의 데이터를 받아 각 관절의 각도를 설정하고, 이를 시리얼 출력으로 확인할 수 있는 코드입니다.   
시리얼 통신을 확인 및 테스트 시 사용할 수 있습니다.   

1. 변수 선언 및 초기화
```c
int baseAngle = 90;
int shoulderAngle = 90;
int upperarmAngle = 90;
int forearmAngle = 90;
```
  각 관절의 기본 각도를 90도로 설정합니다.   

```c
String baseAngle_str;
String shoulder_str;
String upperarm_str;
String forearm_str;
String inString;
```
  시리얼로 받은 문자열 데이터를 관절 별로 나누어 저장하기 위한 String 변수를 선언합니다.   

2. setup
```c
void setup() {
   Serial.begin(115200);
}
```
  시리얼 통신을 설정합니다.   

3. loop
```c
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
```
  시리얼 입력이 있을 때 데이터를 한 줄 단위(`\n` 기준)로 읽어옵니다.   
  `a[Base]b[Shoulder]c[Upperarm]d[Forearm]e` 형식으로 값을 읽어 정수로 변환한 후, 각도를 설정하고 결과를 시리얼로 출력합니다.      

### 103_robotarm_servo_test.ino

모터를 90도에서 120도까지 천천히 변화시키는 코드입니다.   
모터의 각도 변화 및 병렬 제어를 간단히 테스트할 때 사용할 수 있습니다.   

1. 라이브러리 포함 및 변수 선언
```c
#include <Servo.h>
```
  서보 모터 제어를 위해 Servo 라이브러리를 사용합니다.   

```c
Servo base;
Servo shoulder;
Servo forearm;
Servo upperarm;
int baseAngle = 90;
int shoulderAngle = 90;
int forearmAngle = 90;
int upperarmAngle = 90;
```
  관절을 제어하기 위해 각 관절에 대해 Servo 객체를 생성하고 기본 각도를 90도로 설정합니다.   

2. `servoParallelControl`함수
```c
int servoParallelControl (int thePos, Servo theServo, int speed){
    int startPos = theServo.read();       //read the current position of the servo we are working with.
    int newPos = startPos;                // newPos holds the position of the servo as it moves
    
    //define where the pos is with respect to the command
    // if the current position is less that the desired move the position up
    if (startPos < (thePos)){
       newPos = newPos + 1;               
       theServo.write(newPos);
       delay(speed);
       return 0;                          // Tell primary program that servo has not reached its position     
    }
    // Else if the current position is greater than the desired move the servo down
    else if (newPos > (thePos)){
      newPos = newPos - 1;
      theServo.write(newPos);
      delay(speed);
      return 0;  
    }  
    // If the servo is +-5 within the desired range then tell the main program that the servo has reached the desired position.
    else {
        return 1;
    }  
}
```
  서보를 원하는 각도(`thePos`)까지 천천히 이동시키기 위한 함수입니다.   
  현재 각도와 목표 각도를 비교하며 서보를 1도씩 움직입니다.   

3. setup

```c
void setup() {
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
```
  시리얼 통신을 연결하고 서보 모터를 해당 핀에 연결(`attach`)하고 기본 위치로 설정합니다.   

4. loop
```c
void loop() {
      // this value tells when all the joints have reached thier positions
  while(done == 0){     // Loop until all joints have reached thier positions                      && ready == 1
    //move the servo to the desired position
    //This block of code uses "Functions" to make is more condensed.
    status1 = servoParallelControl(120, base, 20);
    status2 = servoParallelControl(120, shoulder, 20);
    status3 = servoParallelControl(120, forearm, 20);
    status4 = servoParallelControl(120, upperarm, 20);         
    // Check whether all the joints have reached their positions
    if (status1 == 1 && status2 == 1 && status3 == 1 && status4 == 1  ){
      done = 1; //When done =1 then the loop will stop
    }   
  }// end of while
}
```
  각 관절 서보를 120도로 이동시킵니다.
  모든 서보가 목표 위치에 도달하면 루프를 종료합니다.  

### 104_oled_test.ino

SSD1306 128x64 OLED 디스플레이의 기본 기능을 테스트하기 위한 코드로, 텍스트 출력 및 화면 초기화를 반복합니다.   

1. 라이브러리 포함 및 상수 정의
```c
#include <U8x8lib.h>  // install U8g2 library

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
```
  OLED 디스플레이 제어를 위해 U8x8lib.h 라이브러리를 사용합니다.   
  디스플레이 크기는 기본 설정(128x64)으로 사용합니다.    

2. 전역 변수 및 객체 생성
```c
String upperarm_str = "world";

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE); 
```
  `upperarm_str`변수에 world를 저장하여 디스플레이에 출력될 값으로 사용합니다.   
  디스플레이와 I2C 통신을 초기화하기 위해 객체를 생성합니다.   

3. clear_oled()
```c
void clear_oled(){
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0,0,"                   ");
  u8x8.drawString(0,1,"                   ");
  u8x8.drawString(0,2,"                   ");
  u8x8.drawString(0,3,"                   ");
  delay(100);
}
```
  OLED 화면을 지우는 함수로, 이전에 출력된 문자열을 공백 문자열로 덮어씁니다.   

4. setup
```c
void setup() {
  Serial.begin(115200);
  u8x8.begin();
  clear_oled();
}
```
  시리얼 통신을 초기화하고 OLED 디스플레이를 시작하고 화면을 초기화합니다.   

5. loop
```c
void loop() {
  clear_oled();
  delay(1000);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0,0,"hello world");
  u8x8.drawString(0,1,upperarm_str.c_str());
  delay(1000);

}
```
  화면 초기화 후 첫번째 줄에 `hello world`, 두번째 줄에 `upperarm_str` 변수의 값을 출력하는 것을 반복합니다.   

### 105_abcd_string_robot_control.ino

시리얼 입력을 통해 각 관절의 각도를 설정하거나 초기화 하고, OLED 디스플레이에 현재 상태를 출력합니다.

1. 라이브러리 추가 및 객체 생성
```c
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <U8x8lib.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);
```
  JDCobot100은 [Adafruit의 라이브러리](https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library.git)와 U8x8lib.h를 사용합니다.    

2. 서보 관련 설정값 정리
```c
#define pulse_us_min 500
#define pulse_us_max 2450
#define servo_freq 50
#define servo_driver_bits 4096
```
  서보의 최소•최대 펄스, 주파수, 서보 모터의 분해능을 정의합니다.   

3. 디스플레이 관련 설정값 정리
```c
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
```
  사용할 디스플레이 크기를 설정합니다.   

4. 서보 모터 관련 
```c
#define base 0
#define shoulder 1
#define upperarm 2
#define forearm 3
#define gripper 4
```
  각 관절에서 사용하는 서보 모터의 포트 번호를 정의합니다.   

5. 서보 모터 관련 상수 및 변수 정의
```c
double baseAngle = 90;
double shoulderAngle = 90;
double upperarmAngle = 90;
double forearmAngle = 90;
double gripperAngle = 180;
double motor_step = 0.5;
```
  각 관절의 기본 각도를 설정하고 각도 변화 단위(motor_step)를 지정합니다.   

```c
int delay_time = 15;
int delay_time_setup = 0;
```
  딜레이 시간을 설정하여 서보 모터가 부드럽게 움직이도록 합니다.   

```c
double angle_list[5] = {baseAngle, shoulderAngle, upperarmAngle, forearmAngle, gripperAngle};
```
  각 관절의 초기 각도를 배열로 정의하여 효율적으로 관리합니다.   
 
```c
long pulse_min = (pulse_us_min / (double)period_us) * servo_driver_bits;
long pulse_max = (pulse_us_max / (double)period_us) * servo_driver_bits;
```
  서보가 올바른 범위 내에서 움직이기 위해 펄스 폭을 드라이버 분해능(4096)에 맞게 변환해야 합니다.   
  변환을 위해 펄스 폭의 최소•최대값을 계산합니다. 

```c
String base_str;
String shoulder_str;
String upperarm_str;
String forearm_str;
String gripper_str;
String inString;
```
  시리얼로 받은 문자열 데이터를 관절 별로 나누어 저장하기 위한 String 변수를 선언합니다.   

6. setup
```c
void setup() {
  // 시리얼 통신을 설정합니다.
  Serial.begin(115200);

  // OLED 디스플레이 초기화
  u8x8.begin();
  u8x8.setPowerSave(0); //절전 모드 해제
  u8x8.setFont(u8x8_font_chroma48medium8_r); //폰트 설정

  // OLED 초기 메시지 출력
  u8x8.setCursor(0, 1);
  u8x8.print("          ");
  u8x8.setCursor(0, 2);
  u8x8.print("jdcobot 100");
  u8x8.setCursor(0, 3);
  u8x8.print("standby");
  u8x8.setCursor(0, 4);
  u8x8.print("          ");

  // 서보 PWM 초기화
  pwm.begin();
  pwm.setOscillatorFrequency(27000000); //오실레이터 주파수를 27MHz로 설정
  pwm.setPWMFreq(servo_freq); //서보 제어를 위한 PWM 주파수 설정
  delay(10);
}
```

7. loop
```c
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
```
  시리얼에 `2a90b90c90d90e180f` 형식의 입력이 들어오면 아래와 같이 값을 저장합니다.
  ```c
  cmd = 2
  baseAngle = 90
  shoulderAngle = 90
  upperarmAngle = 90
  forearmAngle = 90
  gripperAngle = 180
  ``` 
  이때 cmd 값에 따라 서로 다른 작업을 수행합니다.
  `1`: OLED 디스플레이를 초기화하고 디스플레이에 'read angles'를 출력합니다.
  `2`: 각 관절을 목표 각도가 되도록 움직입니다.
  `3`: 모든 서보를 기본 각도로 복귀시킵니다.
  `4`: 동작 중인 서보 모터를 정지하고 OLED에 'stop servo motors' 표시한다.

8. moveServo
```c
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
```
  서보 모터를 목표 각도로 천천히 이동시키는 함수입니다.
  `step`만큼씩 각도를 조정합니다.

  매개변수
    servo_num: 이동시킬 서보 모터의 번호
    targetAngle: 서보 모터가 도달해야 할 목표 각도
    step: 한 번의 호출마다 이동한 각도 단계

9. resetServos
```c
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
```
  서보 모터를 기본 각도로 이동시킵니다   

10. display_angles
```c
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
```
  OLED 디스플레이에 서보 모터의 현재 각도를 출력합니다.   

11. clear_oled
```c
void clear_oled() {
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  for (int i = 0; i < 7; i++) {
    u8x8.drawString(0, i, "                   ");
  }
  delay(100);
}
```
  OLED 디스플레이를 지웁니다.   

12. set_servo
```c
void set_servo(uint8_t n, double angle) {
  double pulse = map(angle, 0, 180, pulse_min, pulse_max);
  pwm.setPWM(n, 0, pulse);
}
```
  특정 서보 모터를 원하는 각도로 설정합니다.   

13. serial_print
```c
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
```
  현재 각도를 시리얼로 출력합니다.

### 302_ros_joint_state_sub.ino
   
시리얼 통신으로 각도 정보를 받아 로봇암을 제어하기 위한 코드입니다.   
1. 라이브러리 포함 및 객체 생성
```c
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <U8x8lib.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);
```

2. 상수 정의 및 초기값 설정

```c
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
```
3. setup
```c
void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(servo_freq);
  delay(10);
}
```
시리얼 통신을 주파수 115200으로 설정하고 서보 모터의 PWM을 초기화 한 후 서보 모터 제어를 위한 주파수를 설정합니다.   
4. loop
```c
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
```
  시리얼에 `a90b90c90d90e180f` 형식의 입력이 들어오면
  아래와 같이 목표 각도 값을 저장하고 모터를 움직입니다.   
  ```c
  baseAngle = 90
  shoulderAngle = 90
  upperarmAngle = 90
  forearmAngle = 90
  gripperAngle = 180
  ```

5. set_servo
```c
void set_servo(uint8_t n, double angle) {
  double pulse = map(angle, 0, 180, pulse_min, pulse_max);
  pwm.setPWM(n, 0, pulse);
}
```
  각도를 펄스로 변환하여 지정된 서보 모터에 전달합니다.   