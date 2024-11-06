## jdcobot100 아두이노 코드 설명

### 101_servo_center

```c
#define pulse_us_min 500            // 최소 펄스 길이 ms = 1000 µs
#define pulse_us_max 2450           // 최대 펄스 길이 ms = 1000 µs
#define servo_freq 50               // 서보 주파수 (Hz)
#define servo_driver_bits 4096
```
서보 모터의 최소, 최대 펄스 길이, 주파수, PWM 드라이버의 분해능 등 상수를 정의합니다

```c
#define base 0
#define shoulder 1
#define upperarm 2
#define forearm 3
#define gripper 4
```
로봇 팔 관절 각 관절에 대한 상수

```c
double baseAngle = 90;
double shoulderAngle = 90;
double upperarmAngle = 90;
double forearmAngle = 90;
double gripperAngle = 90;
double motor_step = 0.5;
```
관절 초기 각도 설정(90도)
각도 등가 폭 설정(0.5도)

```c
int delay_time = 15;
int delay_time_setup = 0;
```
지연 시간을 위한 변수 설정

```c
double angle_list[5] = {baseAngle, shoulderAngle, upperarmAngle, forearmAngle, gripperAngle};
```
각 관절의 각도를 배열로 저장

```c
long period_us = 1000000 / servo_freq; // 주기 (µs)
long pulse_min = (pulse_us_min / (double)period_us) * servo_driver_bits;
long pulse_max = (pulse_us_max / (double)period_us) * servo_driver_bits;
```
펄스 관련 변수 정의

```c
void set_servo(uint8_t n, double angle) {
  double pulse = map(angle, 0, 180, pulse_min, pulse_max);
  pwm.setPWM(n, 0, pulse);
}
```
서보 제어 함수 정의
