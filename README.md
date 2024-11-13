## jdCobot100 빠르게 시작하기 
먼저 jdCobot100을 조립해야 합니다. 조립은 doc/assembly.md를 참고해주세요.   
여기서는 jdCobot100을 빠르게 동작시키고 경험해 볼 수 있습니다.   
아래의 가이드를 따라서 회로 연결, 아두이노 프로그래밍, 파이썬 프로그램 실행하여 jdCobot100을 동작시켜 볼 수 있습니다. 

### 1. 회로 연결
1. 코드의 관절 번호에 맞게 실드에 연결합니다.
아두이노에서 pwm 신호를 받기 위한 것으로, pwm와 전원선의 방향을 잘 확인하고 넣어주세요.
```c
base 0
shoulder 1
upperarm 2
forearm 3
gripper 4
```
![배선확대](https://github.com/user-attachments/assets/81a5f847-b898-4994-8014-f777342b8fbf)

2. 실드의 전원 방향에 유의하며 전원을 연결한다.
![전원부확대](https://github.com/user-attachments/assets/2fb5f88f-a785-4694-a420-75843c9c7b6d)

### 2. 아두이노
1. 노트북에 아두이노를 연결한다.
2. <arduino/101_servo_center.ino>를 아두이노 보드에 업로드한다.
![1서보정렬](https://github.com/user-attachments/assets/cf242a52-391c-43b0-a671-91dd86a2449c)
PWM 신호로 서보 모터를 제어하기 위한 설정 및 초기화를 하고 초기 위치로 서보 모터를 움직이는 코드입니다.   
jdcobot100을 본격적으로 작동시키기 전 서보모터의 기본 위치에 맞게 조립이 잘 되었는지 확인하고 서보모터를 정렬합니다.   
3. <arduino/302_ros_joint_state_sub.ino>를 아두이노 보드에 업로드한다.
![2시리얼가동](https://github.com/user-attachments/assets/e2f4c5cb-4a22-46ac-8115-79b0b2efe81f)
시리얼 입력을 통해 서보 모터의 각도를 파싱하고, 서보 모터를 해당 각도로 제어하는 코드입니다.

```c
baseAngle = inString.substring(inString.indexOf('a') + 1, inString.indexOf('b')).toInt();
shoulderAngle = inString.substring(inString.indexOf('b') + 1, inString.indexOf('c')).toInt();
upperarmAngle = inString.substring(inString.indexOf('c') + 1, inString.indexOf('d')).toInt();
forearmAngle = inString.substring(inString.indexOf('d') + 1, inString.indexOf('e')).toInt();
gripperAngle = inString.substring(inString.indexOf('e') + 1, inString.indexOf('f')).toInt();
```
코드 업로드 후, 시리얼모니터에 a90b90c90d90e90f와 같은 문자열을 입력해 각 서보 모터(base, shoulder, upperarm, forearm, gripper)를 제어할 수 있습니다.

### 3. 파이썬
GUI를 통해 로봇암을 제어하기 위해 파이썬 파일을 실행합니다. Tkinter GUI와 시리얼 통신을 사용해 직관적인 인터페이스로 쉽게 제어할 수 있습니다.
1. <python/Robot_UI_control_study/tk12_add_trajectory.py>를 실행한다.
![1python실행화면](https://github.com/user-attachments/assets/bdf9aa81-94f0-40cb-ae99-004f05eb935c)
2. 아두이노가 연결된 포트와 시리얼을 연결한다.
![2포트선택](https://github.com/user-attachments/assets/8a5a43e8-628a-4b6d-ba2f-6be6bc5938f0)
![3시리얼시작](https://github.com/user-attachments/assets/a3ffae57-e431-4f71-8fee-d33aab5ecca1)
3. 로봇 조인트값을 원하는 값으로 변경한 후 run을 눌러 로봇의 상태를 변화시킨다.
![4로봇각도조절](https://github.com/user-attachments/assets/4acf09d6-28b6-42c4-a648-e0cbbe093e97)
![5각도보내기](https://github.com/user-attachments/assets/5d1a5ca1-3916-46b4-ba21-cbff260b9225)
