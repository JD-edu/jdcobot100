## jdcobot100 아두이노 코드 설명
### 목차
[101_servo_center](#101_servo_center)

### 101_servo_center
1. 서보 드라이버 객체 생성
```c
Adafruit_PWMServoDriver pwm = adafruit_PWMServoDriver();
```

2. 서보 관련 설정 값 정의
```c
#define pulse_us_min 500            // 최소 펄스 길이 ms = 1000 µs
#define pulse_us_max 2450           // 최대 펄스 길이 ms = 1000 µs
#define servo_freq 50               // 서보 주파수 (Hz)
#define servo_driver_bits 4096
```
서보 모터의 최소•최대 펄스 길이, 주파수, PWM 드라이버의 분해능을 정의합니다.

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
double gripperAngle = 90;
double motor_step = 0.5;
```
관절 초기화를 위해기본 각도를 설정하고 각도 변화 단위(motor_step)를 지정합니다.

```c
int delay_time = 15;
int delay_time_setup = 0;
```
딜레이 시간을 설정하여 서보 모터가 부드럽게 움직이도록 합니다

```c
double angle_list[5] = {baseAngle, shoulderAngle, upperarmAngle, forearmAngle, gripperAngle};
```
모든 관절의 각도를 효율적으로 관리하기 위해 배열로 정의합니다.

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
6. setup함수
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

7. loop함수
```c
void loop() {
  for (int i = 0; i < 5; i++){
    set_servo(i, angle_list[i]);
    delay(delay_time);
  }
  delay(2000);
}
```
각 관절을 기본 각도로 설정합니다.
