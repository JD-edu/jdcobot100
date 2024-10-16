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

def uga():
    pass

serial_list = ['시리얼 포트를 선택하세요.']

root = tk.Tk()
root.title('JDcobot 100 Control')
root.geometry('600x480')

m_serial_select = ttk.Frame(root)
var = tk.StringVar()
m_serial_select.pack()

'''
list1 = [1, 2, 3]
이 리스트를 '*'를 사용하여 펼치면 다음과 같은 튜플이 반환됩니다.
print(*list1)
(1, 2, 3)
'''
dropdown = ttk.OptionMenu(m_serial_select, var, serial_list[0], *serial_list, command = uga)
dropdown.pack()
dropdown.configure(state='normal')


root.mainloop()


