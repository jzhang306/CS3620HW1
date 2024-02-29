# This python file creates a GUI for the process monitor to display the information.


import tkinter as tk
from tkinter import scrolledtext
import subprocess # The module that runs the C executable file.


def refresh_process_list():
    text_area.delete('1.0',tk.END)# clear the text.

    process = subprocess.Popen(['./Monitor'],stdout = subprocess.PIPE, stderr=subprocess.PIPE)
    output, error = process.communicate()

    if process.returncode == 0:
        text_area.insert(tk.INSERT, output.decode('utf-8'))
    else:
        text_area.insert(tk.INSERT,"Error running process monitor:\n"+error.decode)

#construct tkinter gui in a scrolled text way.
app = tk.Tk()
app.title("Process Monitor GUI")

text_area = scrolledtext.ScrolledText(app, wrap= tk.WORD, width=100, height = 25)
text_area.pack(pady= 10)

refresh_button = tk.Button(app, text = "Refresh", command = refresh_process_list)
refresh_button.pack(pady=5)

refresh_process_list()

app.mainloop()


