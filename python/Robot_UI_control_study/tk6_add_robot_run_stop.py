'''
MIT License

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
SOFTWARE.
'''

import tkinter as tk
from tkinter import ttk

def select_serial(selection):
    print(selection)

def start_serial():
    print('start serial')

def stop_serial():
    print('stop servo')

def slide_handler_0(selection):
    print(selection)

def run_robot():
    pass

def stop_robot():
    pass

serial_list = ['시리얼 포트를 선택하세요.']

root = tk.Tk()
root.title('KG-KAIROS Robot Control')
#root.geometry('600x480')

m_serial_select = ttk.Frame(root)
var = tk.StringVar()
m_serial_select.pack()

'''
list1 = [1, 2, 3]
이 리스트를 '*'를 사용하여 펼치면 다음과 같은 튜플이 반환됩니다.
print(*list1)
(1, 2, 3)
'''
# add option menu
dropdown = ttk.OptionMenu(m_serial_select, var, serial_list[0], *serial_list, command = select_serial)
dropdown.pack()
dropdown.configure(state='normal')

# add button using frame 
m_serial_start_btn = ttk.Frame(root)
m_serial_stop_btn = ttk.Frame(root)

start_serial_btn = ttk.Button(m_serial_start_btn, text="Start serial", command=start_serial)
start_serial_btn.pack(side='left',padx=10) 
        
stop_serial_btn = ttk.Button(m_serial_stop_btn, text="Stop serial", command=stop_serial)
stop_serial_btn.pack(side='left',padx=10)

start_serial_btn.configure(state='enable')
stop_serial_btn.configure(state='enable')

# add servo angle label using frame
m_link0 = ttk.Frame(root)       # link 0  

link0 = tk.StringVar()
ttk.Label(m_link0,text='Link 0: ',font='Helvetica 10 bold').pack(side='left')
port1 = ttk.Entry(m_link0,width=6, textvariable = link0)
port1.insert('end','80')
port1.pack(side='left',padx=0,pady=5)

# add slider using 
angle_0 = 0
m_slide_0 = ttk.Frame(root)  
m_slide_0_track = ttk.Scale(m_slide_0, length = 200,  from_= -90, to = 90, orient ="vertical")
m_slide_0_track.bind("<ButtonRelease-1>", slide_handler_0)
m_slide_0_track.set(angle_0)
m_slide_0_track.pack(side='left',padx=0,pady=5)

# add robot run stop button
m_robot_run_btn = ttk.Frame(root)
m_robot_stop_btn = ttk.Frame(root)
robot_run_btn = ttk.Button(m_robot_run_btn, text="run robot", command=run_robot)
robot_run_btn.pack(side='left', padx=10)
robot_stop_btn = ttk.Button(m_robot_stop_btn, text="stop robot", command=stop_robot)
robot_stop_btn.pack(side='left', padx=10)

    
# grid layout 
m_serial_select.grid(column=1,row=0,columnspan=3,padx=10,pady=10,sticky='w')
m_serial_start_btn.grid(column=1,row=1,padx=10,pady=5,sticky='w')
m_serial_stop_btn.grid(column=2,row=1,padx=10,pady=5,sticky='w')
m_link0.grid(column=1,row=2,padx=10,pady=5,sticky='w')
m_slide_0.grid(column=1,row=6,padx=15,pady=5,sticky='w')
m_robot_run_btn.grid(column=1, row=8,padx=10,pady=5,sticky='w')
m_robot_stop_btn.grid(column=2, row=8,padx=10,pady=5,sticky='w')
       
       
root.mainloop()


