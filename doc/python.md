## jdcobot100 파이썬 코드 설명
### 목차
[Robot_sequence_move](#robot_sequence_move)   
[ttk_sequence_robot_move_1.py](#ttk_sequence_robot_move_1py) GUI로 제어하기 - 슬라이더를 통한 실시간 제어 및 동작 리스트 기록   
[ttk_sequence_robot_move_2.py](#ttk_sequence_robot_move_2py) GUI로 제어하기 - 버튼을 통한 슬라이더 값 전송 및 초기화   
[ttk_sequence_robot_move_3.py](#ttk_sequence_robot_move_3py)

### Robot_sequence_move
#### ttk_sequence_robot_move_1.py
   
GUI를 통해 직관적으로 로봇 암의 각도를 설정하고 기록된 동작을 실행시킬 수 있습니다.
   
1. 라이브러리 가져오기 및 시리얼 초기화
```c
import serial
import tkinter as tk
from tkinter import ttk
import time

# 시리얼 통신 설정
ser = serial.Serial('COM37', 115200, timeout=1)  # COM 포트 및 보레이트 설정
```
`com37`: 아두이노가 연결된 포트
`115200`: 통신 속도 설정   

2. GUI 초기 설정
```c
# GUI 설정
root = tk.Tk()
root.title("Robot Arm Control")

# 윈도우 크기 조절
root.geometry("800x600")

# 캔버스 생성
canvas = tk.Canvas(root)
canvas.pack(side=tk.LEFT, expand=True, fill=tk.BOTH, padx=20, pady=20)  # 여백 설정

# 프레임 생성
frame = tk.Frame(canvas)
canvas.create_window((0, 0), window=frame, anchor='nw')  # 캔버스에 프레임 추가
```
tkinter를 이용해 GUI를 설정합니다.
`canvas`: 스크롤 가능 영역을 담당
`frame`: GUI 요소를 담는 컨테이너   

3. 각도 설정 및 전송 함수
```c
# 각도 설정 함수
def set_angles():
    base_angle = base_scale.get()  # Base 각도를 스케일에서 가져옴
    shoulder_angle = shoulder_scale.get()  # Shoulder 각도를 스케일에서 가져옴
    upperarm_angle = upperarm_scale.get()  # Upperarm 각도를 스케일에서 가져옴
    forearm_angle = forearm_scale.get()  # Forearm 각도를 스케일에서 가져옴
    # 각도 값을 시리얼 통신을 통해 Arduino로 전송
    angles = "2a" + str(base_angle) + "b" + str(shoulder_angle) + "c" + str(upperarm_angle) + "d" + str(forearm_angle) + "e\n"
    ser.write(angles.encode())  # 문자열을 바이트로 변환하여 시리얼 통신으로 전송
```
슬라이더에서 입력받은 각도를 조합해 문자열로 변환 후, 시리얼 통신으로 아두이노에서 전송합니다.   

4. 동작 기록 및 실행
```c
# 동작 기록 리스트
recorded_actions = []

# 동작 기록 함수
def record_action():
    base_angle = base_scale.get()  # Base 각도를 스케일에서 가져옴
    shoulder_angle = shoulder_scale.get()  # Shoulder 각도를 스케일에서 가져옴
    upperarm_angle = upperarm_scale.get()  # Upperarm 각도를 스케일에서 가져옴
    forearm_angle = forearm_scale.get()  # Forearm 각도를 스케일에서 가져옴
    recorded_actions.append((base_angle, shoulder_angle, upperarm_angle, forearm_angle))
    for i in recorded_actions:
        print(i)
    update_action_list()  # 동작 기록이 추가될 때마다 리스트 업데이트

# 동작 실행 함수
def execute_actions():
    for action in recorded_actions:
        base_angle, shoulder_angle, upperarm_angle, forearm_angle = action
        print(base_angle, shoulder_angle, upperarm_angle, forearm_angle)
        angles = "2a" + str(base_angle) + "b" + str(shoulder_angle) + "c" + str(upperarm_angle) + "d" + str(forearm_angle) + "e\n"
        ser.write(angles.encode())
        time.sleep(3)
```
`record_action`: 현재 각도 저장
`execute_actions`: 저장된 동작을 순차적으로 실행하며, 각 동작마다 3초 대기   

5. 동작 리스트 업데이트
```c
# 동작 리스트 업데이트 함수
def update_action_list():
    action_listbox.delete(0, tk.END)
    for i, action in enumerate(recorded_actions):
        action_listbox.insert(tk.END, f"Action {i+1}")

# 선택된 동작 표시 함수
def show_selected_action(evt):
    selected_index = action_listbox.curselection()
    if selected_index:
        selected_index = int(selected_index[0])
        base_angle, shoulder_angle, upperarm_angle, forearm_angle = recorded_actions[selected_index]
        selected_action_label.config(text=f"Base: {base_angle}, Shoulder: {shoulder_angle}, Upperarm: {upperarm_angle}, Forearm: {forearm_angle}")
```
`update_action_list`: 동작 리스트를 리스트박스에 표시
`show_selected_action`: 선택된 동작의 세부 정보를 표시   

6. GUI 구성 요소
```c
# GUI 구성 요소 생성
base_label = tk.Label(frame, text="Base Angle:")  # Base 각도 입력 레이블
base_label.grid(row=0, column=0)  # 그리드 배치

base_scale = ttk.Scale(frame, from_=0, to=180, orient=tk.HORIZONTAL, length=300)  # Base 각도 입력 스케일
base_scale.grid(row=0, column=1)  # 그리드 배치

shoulder_label = tk.Label(frame, text="Shoulder Angle:")  # Shoulder 각도 입력 레이블
shoulder_label.grid(row=1, column=0)  # 그리드 배치

shoulder_scale = ttk.Scale(frame, from_=0, to=180, orient=tk.HORIZONTAL, length=300)  # Shoulder 각도 입력 스케일
shoulder_scale.grid(row=1, column=1)  # 그리드 배치

upperarm_label = tk.Label(frame, text="Upperarm Angle:")  # Upperarm 각도 입력 레이블
upperarm_label.grid(row=2, column=0)  # 그리드 배치

upperarm_scale = ttk.Scale(frame, from_=0, to=180, orient=tk.HORIZONTAL, length=300)  # Upperarm 각도 입력 스케일
upperarm_scale.grid(row=2, column=1)  # 그리드 배치

forearm_label = tk.Label(frame, text="Forearm Angle:")  # Forearm 각도 입력 레이블
forearm_label.grid(row=3, column=0)  # 그리드 배치

forearm_scale = ttk.Scale(frame, from_=0, to=180, orient=tk.HORIZONTAL, length=300)  # Forearm 각도 입력 스케일
forearm_scale.grid(row=3, column=1)  # 그리드 배치

set_button = tk.Button(frame, text="Set Angles", command=set_angles)  # 각도 설정 버튼
set_button.grid(row=4, columnspan=2)  # 그리드 배치

record_button = tk.Button(frame, text="Record Action", command=record_action)  # 동작 기록 버튼
record_button.grid(row=5, columnspan=2)  # 그리드 배치

execute_button = tk.Button(frame, text="Execute Actions", command=execute_actions)  # 동작 실행 버튼
execute_button.grid(row=6, columnspan=2)  # 그리드 배치
```
슬라이더를 통해 각 관절의 각도를 설정할 수 있도록 버튼 배치   

7. 동작 리스트 박스와 선택된 동작 표시
```c
# 동작 리스트박스 생성
action_listbox = tk.Listbox(frame)
action_listbox.grid(row=7, columnspan=2, padx=5, pady=5)
action_listbox.bind('<<ListboxSelect>>', show_selected_action)

# 선택된 동작을 표시할 레이블
selected_action_label = tk.Label(frame, text="")
selected_action_label.grid(row=8, columnspan=2)
```
기록된 로봇 팔 동작을 리스트박스에 표시하고, 선택한 동작의 세부 정보를 레이블에 보여줍니다.   

8. 스크롤바와 메인 루프
```c
# 스크롤바 설정
vscrollbar = tk.Scrollbar(root, orient=tk.VERTICAL, command=canvas.yview)
vscrollbar.pack(side=tk.RIGHT, fill=tk.Y)
canvas.configure(yscrollcommand=vscrollbar.set)

hscrollbar = tk.Scrollbar(root, orient=tk.HORIZONTAL, command=canvas.xview)
hscrollbar.pack(side=tk.BOTTOM, fill=tk.X)
canvas.configure(xscrollcommand=hscrollbar.set)

canvas.bind('<Configure>', lambda e: canvas.configure(scrollregion=canvas.bbox('all')))

root.mainloop()  # GUI 루프 실행
```
스크롤바와 GUI를 최종 실행합니다.

#### ttk_sequence_robot_move_2.py
   
슬라이더와 입력창을 통해 로봇 암의 관절 각도를 설정하고 시리얼 통신을 통해 동작을 전송하거나 초기화합니다.
   
1. 라이브러리 가져오기 및 시리얼 통신 설정
```c
import tkinter as tk
from tkinter import ttk
import serial
```
`tkinter`: GUI 구성
`serial`: 로봇암과 시리얼 통신   

```c
seq = serial.Serial(
        baudrate=115200,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=1
    )


seq.port = "COM37"
seq.open()
```
시리얼 통신 초기화
`com37` 포트 사용, 통신 속도 `115200`으로 설정   

2. GUI 초기화 및 윈도우 설정
```c
root = tk.Tk()
root.title('KG-KAIROS Robot Control')
root.geometry('600x480')
```
tkinter를 사용하여 메인 윈도우를 생성   
제목`title` 및 크기`geometry`를 설정합니다.   

3. 슬라이더와 입력 창을 통한 각도 설정
```c
angle_0 = 90
angle_1 = 90
angle_2 = 90
angle_3 = 90

# add servo value  frame
m_link0 = ttk.Frame(root)
m_link1 = ttk.Frame(root)
m_link2 = ttk.Frame(root)
m_link3 = ttk.Frame(root)      # link 0  

port1 = ttk.Entry(m_link0,width=6, textvariable = link0)
port2 = ttk.Entry(m_link1,width=6, textvariable = link1)
port3 = ttk.Entry(m_link2,width=6, textvariable = link2)
port4 = ttk.Entry(m_link3,width=6, textvariable = link3)

port1.insert('end','80')
port2.insert('end','80')
port3.insert('end','80')
port4.insert('end','80')
```
기본값은 90으로 초기화합니다.   
각 관절의 각도를 나타내는 입력창을 생성합니다.   

4. 슬라이더 값 변경 처리
```c
def slide_handler_base(event):
    global angle_0, angle_1, angle_2, angle_3
    
    print(angle_0)
    print(angle_1)
    print(angle_2)
    print(angle_3)
    port1.delete(0, 'end')
    port1.insert(0, str(int(angle_0)))
    port2.delete(0, 'end')
    port2.insert(0, str(int(angle_1)))
    port3.delete(0, 'end')
    port3.insert(0, str(int(angle_2)))
    port4.delete(0, 'end')
    port4.insert(0, str(int(angle_3)))
```
슬라이더 값이 변경되는 것을 처리하기 위한 함수입니다.   
슬라이더 값 변경 시 입력창에 실시간으로 업데이트됩니다.   
현재 각도 값이 출력되어 디버깅에도 활용할 수 있습니다.   

5. 로봇암 동작 및 초기화 함수
```c
def move_robot():
    global angle_0
    global angle_1
    global angle_2
    global angle_3
    print("robot move")
    seq.write(("2a" + str(angle_0) + "b" + str(angle_1) + "c" + str(angle_2) + "d" + str(angle_3) + "e" + "\n").encode())
```
설정된 각도를 시리얼 통신으로 전송하여 로봇암을 제어합니다.   
    
```c
def reset_robot():
    global angle_0
    global angle_1
    global angle_2
    global angle_3
    print("reset robot")
    seq.write("3\n".encode())
    port1.delete(0, 'end')
    port1.insert(0, '90')
    port2.delete(0, 'end')
    port2.insert(0, '90')
    port3.delete(0, 'end')
    port3.insert(0, '90')
    port4.delete(0, 'end')
    port4.insert(0, '90')
    angle_0 = 90
    angle_1 = 90
    angle_2 = 90
    angle_3 = 90
    m_slide_0_track.set(angle_0)
    m_slide_1_track.set(angle_1)
    m_slide_2_track.set(angle_2)
    m_slide_3_track.set(angle_3)
```
로봇 암을 초기 상태로 복원하며, 슬라이더와 입력창 값도 초기화됩니다   

6. GUI 버튼 및 슬라이더 생성
```c
m_robot_move_btn = ttk.Frame(root)
m_robot_reset_btn = ttk.Frame(root)

robot_move_btn = ttk.Button(m_robot_move_btn, text="move robot", command=move_robot)
robot_move_btn.pack(side='left',padx=10) 
        
robot_reset_btn = ttk.Button(m_robot_reset_btn, text="reset robot", command=reset_robot)
robot_reset_btn.pack(side='left',padx=10)
```
`Move Robot`: 설정된 각도로 로봇을 움직이는 버튼   
`Reset Robot`: 로봇과 GUI를 초기 상태로 복원하는 버튼   

```c
m_slide_0 = ttk.Frame(root)  
m_slide_0_track = ttk.Scale(m_slide_0, length = 200,  from_= 0, to = 180, orient ="vertical", command=slide_handler_base)
m_slide_0_track.set(angle_0)
m_slide_0_track.pack(side='left',padx=0,pady=5)

m_slide_1 = ttk.Frame(root)  
m_slide_1_track = ttk.Scale(m_slide_1, length = 200,  from_= 0, to = 180, orient ="vertical", command=slide_handler_base)
m_slide_1_track.set(angle_1)
m_slide_1_track.pack(side='left',padx=0,pady=5)

m_slide_2 = ttk.Frame(root)  
m_slide_2_track = ttk.Scale(m_slide_2, length = 200,  from_= 0, to = 180, orient ="vertical", command=slide_handler_base)
m_slide_2_track.set(angle_2)
m_slide_2_track.pack(side='left',padx=0,pady=5)

m_slide_3 = ttk.Frame(root)  
m_slide_3_track = ttk.Scale(m_slide_3, length = 200,  from_= 0, to = 180, orient ="vertical", command=slide_handler_base)
m_slide_3_track.set(angle_3)
m_slide_3_track.pack(side='left',padx=0,pady=5)
```
관절 각도를 조정하기 위한 슬라이더 배치   
7. GUI 레이아웃 배치 및 실행
```c
m_robot_move_btn.grid(column=1,row=0,columnspan=3,padx=10,pady=10,sticky='w')
m_robot_reset_btn.grid(column=2,row=0,padx=10,pady=5,sticky='w')
m_link0.grid(column=1,row=1,padx=10,pady=5,sticky='w')
m_link1.grid(column=2,row=1,padx=10,pady=5,sticky='w')
m_link2.grid(column=3,row=1,padx=10,pady=5,sticky='w')
m_link3.grid(column=4,row=1,padx=10,pady=5,sticky='w')
m_slide_0.grid(column=1,row=6,padx=15,pady=5,sticky='w')
m_slide_1.grid(column=2,row=6,padx=15,pady=5,sticky='w')
m_slide_2.grid(column=3,row=6,padx=15,pady=5,sticky='w')
m_slide_3.grid(column=4,row=6,padx=15,pady=5,sticky='w')

root.mainloop()
```
버튼과 슬라이더를 그리드 레이아웃에 배치   
`root.mainloop()`로 GUI 실행   

#### ttk_sequence_robot_move_3.py
   
로봇 팔의 관절 각도를 설정하고 저장된 위치를 불러와 효율적으로 제어합니다.   

1. 라이브러리 불러오기 및 시리얼 통신 설정
```c
import serial
import tkinter as tk
from tkinter import ttk
import time

ser = serial.Serial(
    port="COM37",
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
)
```
포트`com37`, 보드 레이트`115200`로 시리얼 통신 설정   
`tkinter`로 GUI 구현   

2. 