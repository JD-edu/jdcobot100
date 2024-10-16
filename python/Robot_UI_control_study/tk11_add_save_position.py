import tkinter as tk
from tkinter import ttk
import serial 
import threading 
import sys 
import glob
import time

def select_serial():
    selected_port = var.get()
    print(selected_port)

def start_serial():
    seq.port = var.get()
    seq.open()
    print("serial port opened:", seq.port)

def stop_serial():
    seq.close()
    print("serial port stopped:", seq.port)

def slide_handler_base(event, idx):
    global angles
    angles[idx] = slides[idx].get()
    print(angles[idx])
    entry_boxes[idx].delete(0, 'end')
    entry_boxes[idx].insert(0, str(int(angles[idx])))

def run_robot():
    global angles
    cmd = '2a'+str(int(angles[0]))+'b'+str(int(angles[1]))+'c'+str(int(angles[2]))+'d'+str(int(angles[3]))+'e'+str(int(angles[4]))+'f\n'
    print(cmd)
    seq.write(cmd.encode())
    print(cmd.encode())

def stop_robot():
    cmd = '1abcdef'
    seq.write(cmd.encode())
    while True:
        if seq.in_waiting > 0:
            data = seq.readline().decode('utf-8').strip()
            for i in range(slide_num):
                angles[i] = data[data.find(chr(97+i))+1:data.find(chr(98+i))]
                entry_boxes[i].delete(0, 'end')
                entry_boxes[i].insert(0, str(int(angles[i])))
                slides[i].set(angles[i])
            print(angles)
            break

def reset_robot():
    cmd = '3abcdef\n'
    seq.write(cmd.encode())
    print(cmd.encode())
    for i in range(slide_num):
        entry_boxes[i].delete(0, 'end')
        entry_boxes[i].insert(0, '90')
        slides[i].set(90)

def save_positions():
    positions = [int(round(slides[i].get(), 0)) for i in range(len(slides))]
    with open("robot_positions.txt", "a") as f:
        f.write(",".join(map(str, positions)) + "\n")
    update_position_listbox()

def delete_positions(selected_position):
    try:
        with open("robot_positions.txt", "r") as f:
            lines = f.readlines()
        
        if 0 <= selected_position < len(lines):
            del lines[selected_position]
            with open("robot_positions.txt", "w") as f:
                for line in lines:
                    f.write(line)
            print("Position at line", selected_position, "deleted.")
            update_position_listbox()
        else:
            print("Invalid selection.")
    except FileNotFoundError:
        print("No saved positions found.")

def load_positions(selected_position):
    try:
        with open("robot_positions.txt", "r") as f:
            lines = f.readlines()
            if 0 <= selected_position < len(lines):
                positions = list(map(int, lines[selected_position].split(',')))
                for i in range(len(positions)):
                    slides[i].set(positions[i])
                    entry_boxes[i].delete(0, tk.END)
                    entry_boxes[i].insert(0, str(positions[i]))
            else:
                print("Invalid selection.")
    except FileNotFoundError:
        print("No saved positions found.")

def update_position_listbox():
    listbox.delete(0, tk.END)
    try:
        with open("robot_positions.txt", "r") as f:
            lines = f.readlines()
            for i, line in enumerate(lines):
                listbox.insert(i, f"Position {i+1}")
    except FileNotFoundError:
        print("No saved positions found.")

def update_option_menu():
    global dropdown
    global serial_list
    menu = dropdown["menu"]
    menu.delete(0, "end")
    for string in serial_list:
        menu.add_command(label=string, command=lambda value=string: var.set(value))

def startTimer(iTimeSec, isRepeated):
    timer_thread1 = threading.Timer(iTimeSec, timerCallBack, [iTimeSec, isRepeated])
    timer_thread1.daemon = True
    timer_thread1.start()

def timerCallBack(iTimeSec, isRepeated):
    global serial_list
    print("start timer")
    result = serial_ports()
    serial_list = result
    print(serial_list)
    update_option_menu()
    start_serial_btn.configure(state='enable')
    stop_serial_btn.configure(state='enable')
    dropdown.configure(state='enable')
    if isRepeated == True:
        timer_thread1 = threading.Timer(iTimeSec, timerCallBack, [iTimeSec, isRepeated])
        timer_thread1.daemon = True
        timer_thread1.start()

def serial_ports():   
    if sys.platform.startswith('win'):   
        ports = ['COM%s' % (i + 1) for i in range(256)]   
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):   
        # this excludes your current terminal "/dev/tty"   
        ports = glob.glob('/dev/tty[A-Za-z]*')   
    elif sys.platform.startswith('darwin'):   
        ports = glob.glob('/dev/tty.*')   
    else:   
        raise EnvironmentError('Unsupported platform')   
    
    result = []   
    for port in ports:   
        try:   
            s = serial.Serial(port)   
            s.close()   
            result.append(port)
        except (OSError, serial.SerialException):   
            pass   
    return result


serial_list = None

print("started...")
angles = [90, 90, 90, 90, 90]

startTimer(1, False)
serial_list = ['시리얼 포트를 선택하세요.']

root = tk.Tk()
root.title('JDcobot 100 Control')

m_serial_select = ttk.Frame(root)
var = tk.StringVar()
m_serial_select.pack()

'''
list1 = [1, 2, 3]
이 리스트를 '*'를 사용하여 펼치면 다음과 같은 튜플이 반환됩니다.
print(*list1)
(1, 2, 3)
'''

# Add option menu
dropdown = ttk.OptionMenu(m_serial_select, var, serial_list[0], *serial_list, command=select_serial)
dropdown.pack()
dropdown.configure(state='normal')

# Add buttons using frames
m_serial_start_btn = ttk.Frame(root)
start_serial_btn = ttk.Button(m_serial_start_btn, text="Start serial", command=start_serial)
start_serial_btn.pack(side='left', padx=10)

m_serial_stop_btn = ttk.Frame(root)  
stop_serial_btn = ttk.Button(m_serial_stop_btn, text="Stop serial", command=stop_serial)
stop_serial_btn.pack(side='left', padx=10)

start_serial_btn.configure(state='disable')
stop_serial_btn.configure(state='disable')

slide_num = len(angles)        # num of slides
m_link_frames = []
entry_boxes = []
slides = []

for i in range(slide_num):
    m_link_frame = ttk.Frame(root)
    m_link_frames.append(m_link_frame)
    
    link_val = tk.StringVar()
    ttk.Label(m_link_frame, text=f'Link {i}: ', font='Helvetica 10 bold').pack(side='left')
    link_val_box = ttk.Entry(m_link_frame, width=6, textvariable=link_val)
    link_val_box.pack(side='left', padx=0, pady=5)
    entry_boxes.append(link_val_box)
    
    m_slide_frame = ttk.Frame(root)
    slide = ttk.Scale(m_slide_frame, length=100, from_=0, to=180, orient="vertical")
    slide.set(angles[i])
    slide.pack(side='left', padx=0, pady=5)
    slides.append(slide)
for i in range(slide_num):
    entry_boxes[i].insert(0,'90')

# Add robot run stop buttons
m_robot_run_btn = ttk.Frame(root)
robot_run_btn = ttk.Button(m_robot_run_btn, text="run robot", command=run_robot)
robot_run_btn.pack(side='left', padx=10)

m_robot_stop_btn = ttk.Frame(root)
robot_stop_btn = ttk.Button(m_robot_stop_btn, text="stop robot", command=stop_robot)
robot_stop_btn.pack(side='left', padx=10)

m_robot_reset_btn = ttk.Frame(root)
robot_reset_btn = ttk.Button(m_robot_reset_btn, text="reset robot", command=reset_robot)
robot_reset_btn.pack(side='left', padx=10)

# Grid layout
m_serial_select.grid(column=1, row=0, columnspan=3, padx=10, pady=10, sticky='w')
m_serial_start_btn.grid(column=3, row=0, padx=10, pady=5, sticky='w')
m_serial_stop_btn.grid(column=4, row=0, padx=10, pady=5, sticky='w')

for i, m_link_frame in enumerate(m_link_frames):
    m_link_frame.grid(column=i+1, row=2, padx=10, pady=5, sticky='w')
    m_slide_frame = slides[i].master
    m_slide_frame.grid(column=i+1, row=6, padx=15, pady=5, sticky='w')

m_robot_reset_btn.grid(column=slide_num-2, row=8, padx=10, pady=5, sticky='w')
m_robot_run_btn.grid(column=slide_num-1, row=8, padx=10, pady=5, sticky='w')
m_robot_stop_btn.grid(column=slide_num, row=8, padx=10, pady=5, sticky='w')

# Set the command for each slide after they are all created
for i, slide in enumerate(slides):
    slide.configure(command=lambda event, idx=i: slide_handler_base(event, idx))

# Serial port 
seq = serial.Serial(
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
)

# Add listbox
m_listbox_frame = ttk.Frame(root)
m_listbox_frame.grid(column=1, row=18, columnspan=3, padx=10, pady=10, sticky='w')

listbox = tk.Listbox(m_listbox_frame, width=30, height=7)
listbox.pack(padx=10, pady=10)


m_save_load_but_frame = ttk.Frame(root)
m_save_load_but_frame.grid(column=3, row=18, padx=10, pady=10, sticky='w')

save_btn = ttk.Button(m_save_load_but_frame, text="Save Positions", command=save_positions)
save_btn.grid(column=1, row=1, padx=10, pady=10, sticky='w')

delete_btn = ttk.Button(m_save_load_but_frame, text="delet Positions", command=lambda: delete_positions(listbox.curselection()[0]))
delete_btn.grid(column=1, row=2, padx=10, pady=10, sticky='w')

load_btn = ttk.Button(m_save_load_but_frame, text="Load Positions", command=lambda: load_positions(listbox.curselection()[0]))
load_btn.grid(column=1, row=3, padx=10, pady=10, sticky='w')

m_trajectory_frame = ttk.Frame(root)
m_trajectory_frame.grid(column=4, row=18, padx=10, pady=10, sticky='n')
trajectory_btn = ttk.Button(m_trajectory_frame, text="execute\ntrajectory", command=save_positions)
trajectory_btn.grid(column=1, row=1, padx=10, pady=10, sticky='w')



root.mainloop()
