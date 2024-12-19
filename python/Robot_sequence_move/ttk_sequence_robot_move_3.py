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

def reset_robot():
    print("reset")
    ser.write("3\n".encode())
    time.sleep(5)

def move_robot(a_value, b_value, c_value, d_value):
    print(f"move to A:{a_value} B:{b_value} C:{c_value} D:{d_value}")
    command = f"2a{a_value}b{b_value}c{c_value}d{d_value}e\n"
    ser.write(command.encode())

def update_slider_from_entry(entry_var, slider_var):
    try:
        value = int(entry_var.get())
        if 0 <= value <= 180:
            slider_var.set(value)
    except ValueError:
        pass

def save_positions():
    positions = [slider_vars[i].get() for i in range(len(slider_vars))]
    with open("robot_positions.txt", "a") as f:
        f.write(",".join(map(str, positions)) + "\n")
    update_position_listbox()

def load_positions(selected_position):
    try:
        with open("robot_positions.txt", "r") as f:
            lines = f.readlines()
            if 0 <= selected_position < len(lines):
                positions = list(map(int, lines[selected_position].split(',')))
                for i in range(len(positions)):
                    slider_vars[i].set(positions[i])
                    slider_entries[i].delete(0, tk.END)
                    slider_entries[i].insert(0, str(positions[i]))
            else:
                print("Invalid selection.")
    except FileNotFoundError:
        print("No saved positions found.")

def update_position_listbox():
    position_listbox.delete(0, tk.END)
    try:
        with open("robot_positions.txt", "r") as f:
            lines = f.readlines()
            for i, line in enumerate(lines):
                position_listbox.insert(i, f"Position {i+1}")
    except FileNotFoundError:
        print("No saved positions found.")

root = tk.Tk()
root.title("KG KAIROS Robot Arm Control")
root.geometry("600x480")

position_listbox_frame = ttk.Frame(root)
position_listbox_label = ttk.Label(position_listbox_frame, text="Saved Positions:")
position_listbox = tk.Listbox(position_listbox_frame, selectmode=tk.SINGLE)
position_listbox_label.pack(side='top')
position_listbox.pack(side='left')
position_listbox_frame.grid(column=0, row=18, columnspan=2, pady=10, padx=10, sticky='w')


slider_labels = ['BASE', 'SHOULDER', 'ELBOW', 'GRIPPER']
slider_vars = [tk.IntVar() for _ in slider_labels]
slider_entries = [None] * len(slider_labels)
slider_scales = [None] * len(slider_labels)
slider_labels_widgets = [None] * len(slider_labels)

for i, label in enumerate(slider_labels):
    ttk.Label(root, text=f"{label}:").grid(column=0, row=i*3, pady=10, sticky='w')

    slider_vars[i].set(90)
    slider_entries[i] = ttk.Entry(root, width=5)
    slider_entries[i].grid(column=1, row=i*3, pady=10, padx=(0, 10), sticky='w')
    
    slider_scales[i] = ttk.Scale(root, from_=0, to=180, orient="horizontal", variable=slider_vars[i], length=150)
    slider_scales[i].grid(column=2, row=i*3, pady=10, sticky='w')

    slider_labels_widgets[i] = ttk.Label(root, textvariable=slider_vars[i])
    slider_labels_widgets[i].grid(column=2, row=i*3+1, pady=5, sticky='w')

    update_cmd = lambda event, slider_var=slider_vars[i], entry_var=slider_entries[i]: update_slider_from_entry(entry_var, slider_var)
    slider_entries[i].bind("<FocusOut>", update_cmd)


m_robot_reset_btn = ttk.Frame(root)
robot_reset_btn = ttk.Button(m_robot_reset_btn, text="Reset Robot", command=reset_robot)
robot_reset_btn.pack(side='left', padx=10)
m_robot_reset_btn.grid(column=4, row=8, pady=10, sticky='w')

move_button = ttk.Button(root, text="Move Robot", command=lambda: move_robot(
    slider_vars[0].get(), slider_vars[1].get(), slider_vars[2].get(), slider_vars[3].get()
))
move_button.grid(column=2, row=8, pady=10, sticky='w')

save_button = ttk.Button(root, text="Save Positions", command=save_positions)
save_button.grid(column=0, row=15, pady=10, sticky='w')

load_button = ttk.Button(root, text="Load Selected Position", command=lambda: load_positions(position_listbox.curselection()[0]))
load_button.grid(column=2, row=18, pady=10, sticky='w')

root.mainloop()