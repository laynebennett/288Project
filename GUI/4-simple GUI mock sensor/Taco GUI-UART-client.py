# Author - Gage Baker
# Date: 5/3/2025
# Description: A python GUI made to show the radar 
#              and key presses of the cybot during program execution.

import tkinter as tk
from tkinter import messagebox
import serial
import serial.tools.list_ports
import threading
import os

class cybotGUI:
    def __init__(self, port='COM3', baud=115200):
        # Serial connection
        try:
            self.ser = serial.Serial(port, baud, timeout=1)
        except serial.SerialException as e:
            messagebox.showerror("Serial Port Error", f"Could not open port {port}.\n\n{e}")
            raise SystemExit  # Exit the program if serial connection fails

        self.root = tk.Tk()
        self.root.title("IR Radar View")

        # Create a main frame for layout consistency
        main_frame = tk.Frame(self.root)
        main_frame.pack()

        self.canvas = tk.Canvas(main_frame, width=400, height=300, bg='white')
        self.canvas.pack()

        self.active_command = None

        # Frame to hold movement buttons
        btn_frame = tk.Frame(main_frame)
        btn_frame.pack()

        # Movement buttons inside btn_frame
        self.btn_forward = tk.Button(btn_frame, text="Forward")
        self.btn_left = tk.Button(btn_frame, text="Left")
        self.btn_backward = tk.Button(btn_frame, text="Backward")
        self.btn_right = tk.Button(btn_frame, text="Right")
        self.btn_quit = tk.Button(btn_frame, text="Quit", command=self.on_close)

        self.btn_forward.grid(row=0, column=1)
        self.btn_left.grid(row=1, column=0)
        self.btn_backward.grid(row=2, column=1)
        self.btn_right.grid(row=1, column=2)
        self.btn_quit.grid(row=3, column=3, padx=10, pady=10, sticky="se")

        self.btn_forward.bind("<ButtonPress-1>", lambda event: self.send_command('w'))
        self.btn_forward.bind("<ButtonRelease-1>", lambda event: self.send_command('x'))

        self.btn_left.bind("<ButtonPress-1>", lambda event: self.send_command('a'))
        self.btn_left.bind("<ButtonRelease-1>", lambda event: self.send_command('x'))

        self.btn_backward.bind("<ButtonPress-1>", lambda event: self.send_command('s'))
        self.btn_backward.bind("<ButtonRelease-1>", lambda event: self.send_command('x'))

        self.btn_right.bind("<ButtonPress-1>", lambda event: self.send_command('d'))
        self.btn_right.bind("<ButtonRelease-1>", lambda event: self.send_command('x'))

        # Start serial reading thread
        self.running = True
        self.thread = threading.Thread(target=self.read_serial, daemon=True)
        self.thread.start()

        # Proper GUI shutdown
        self.root.protocol("WM_DELETE_WINDOW", self.on_close)
        self.root.mainloop()

    def send_command(self, cmd_char):
        try:
            self.ser.write(cmd_char.encode())
            self.highlight_button(cmd_char)
        except Exception as e:
            print("Failed to send command:", e)

    def highlight_button(self, cmd_char):
        self.clear_highlights()
        if cmd_char == 'w':
            self.btn_forward.config(bg='green')
        elif cmd_char == 'a':
            self.btn_left.config(bg='green')
        elif cmd_char == 's':
            self.btn_backward.config(bg='green')
        elif cmd_char == 'd':
            self.btn_right.config(bg='green')

    def clear_highlights(self):
        for btn in [self.btn_forward, self.btn_backward, self.btn_left, self.btn_right]:
            btn.config(bg='SystemButtonFace')

    def read_serial(self):
        while self.running:
            try:
                line = self.ser.readline().decode().strip()
                if line in ['w', 'a', 's', 'd']:
                    self.highlight_button(line)
                elif line == 'x':
                    self.clear_highlights()
            except Exception as e:
                print("Error reading serial:", e)

    def on_close(self):
        self.running = False
        self.ser.close()
        self.root.destroy()

cybotGUI(port='COM3')
