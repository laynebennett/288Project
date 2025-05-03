# Author - Gage Baker
# Date: 5/3/2025
# Description: A python GUI made to show the radar 
#              and key presses of the cybot during program execution.

import tkinter as tk
from tkinter import messagebox # THIS IS HUGE
import serial
import serial.tools.list_ports # NEEDED FOR HUGENESS
import threading
import os

# NOTES:
# I do not know what our port is, there is a command that can be run to figure it out.
# I have not made the radar for this code yet, not sure if I have to since its coded 
# in a different file but I ran out of time. 

# This code uses tkinter to make the buttons, make colors on the buttons change when 
# pressed and not pressed, the thread should be able to read from the microcontroller
# for IR distance data, I would go from there but I genuinely suggest using Chat for
# incorporating the radar already made with test values into this GUI in tkinter and 
# read values during execution instead.

class cybotGUI:
    def __init__(self, port='COM3', baud=115200):
        # Serial connection
        self.ser = serial.Serial(port, baud, timeout=1)

        # This is what makes the IR

        self.root = tk.Tk() # This sets up the GUI panel
        self.root.title("IR Radar View")
        self.canvas = tk.Canvas(self.root, width=400, height=300, bg='white') # This creates the object
        self.canvas.pack()

        # Setting a frame for the movement buttons

        self.active_command = None
        btn_frame = tk.Frame(self.root)
        btn_frame.pack()

        # Makes all the buttons and organizes them in a grid

        self.btn_forward = tk.Button(root, text="Forward", command=lambda: self.send_command('w'))
        self.btn_left = tk.Button(root, text="Left", command=lambda: self.send_command('a'))
        self.btn_backward = tk.Button(root, text="Backward", command=lambda: self.send_command('s'))
        self.btn_right = tk.Button(root, text="Right", command=lambda: self.send_command('d'))

        self.btn_forward.grid(row=0, column=1)
        self.btn_left.grid(row=1, column=0)
        self.btn_backward.grid(row=1, column=1)
        self.btn_right.grid(row=1, column=2)

        # Creating a thread to read from the microcontroller (I HAVE NO CLUE)

        self.running = True
        self.thread = threading.Thread(target=self.read_serial, daemon=True)
        self.thread.start()

        # Start GUI loop

        self.root.protocol("WM_DELETE_WINDOW", self.on_close)
        self.root.mainloop()

    # Send data (echo) to microcontroller for movement
    def send_command(self, cmd_char):
        try:
            self.ser.write(cmd_char.encode())
            self.highlight_button(cmd_char)
        except Exception as e:
            print("Failed to send command:", e)

    # Shows which buttons are being pressed
    def highlight_button():

        self.clear_highlights()

        if cmd_char == 'w':
            self.btn_forward.config(bg='green')
        elif cmd_char == 'a':
            self.btn_left.config(bg='green')
        elif cmd_char == 's':
            self.btn_backward.config(bg='green')
        elif cmd_char == 'd':
            self.btn_right.config(bg='green')

    # Clears the highlights in case of sent 'x' (no keys being pressed, which stops movement) or new button press
    def clear_highlights(self):
        for btn in [self.btn_forward, self.btn_backward, self.btn_left, self.btn_right]
           btn.config(bg='SystemButtonFace') # This resets backgrounds to original

    # Reads the buttons through the echo
    def read_serial(self):
        while self.running:
            try:
                line = self.ser.readLine().decode().strip()
                if line in ['w', 'a', 's', 'd']:
                    self.highlight_button(line)
                elif line == 'x':
                    self.clear_highlights()
            except Exception as e:
                print("Error reading serial:", e)

    # Terminate program when done
    def on_close(self):
        self.running = False
        self.ser.close()
        self.root.destroy()

# cybotGUI(port='COM3') Aparently this line starts the GUI, considering the port is correct and is connected to the cybot. 









