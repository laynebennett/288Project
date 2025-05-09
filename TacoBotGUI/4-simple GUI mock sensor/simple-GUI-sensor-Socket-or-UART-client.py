import time
import socket
import tkinter as tk
from tkinter import ttk
import threading
import os
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import numpy as np
import math
from queue import Queue

# Globals
gui_send_message = "wait\n"
scan_lines_to_plot = []
window = None
canvas = None
ax = None
message_box = None
minimap_canvas = None
cybot_x, cybot_y = 250, 250
cybot_angle_deg = 90  # 90 is facing right
step_distance = 2  # mm per step forward
movement_threads = {}  # To track active movement per command
movement_flags = {}  # Control flags for movement threads
stop_queue = Queue()  # Queue to hold stop commands ("x")

def main():
    global window, canvas, ax, message_box, minimap_canvas

    window = tk.Tk()
    window.title("CyBot Control GUI")
    window.geometry("1000x650")

    # Layout
    main_frame = tk.Frame(window)
    main_frame.pack(fill=tk.BOTH, expand=True)

    # Left: Scanner plot
    left_frame = tk.Frame(main_frame)
    left_frame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

    fig, ax = plt.subplots(subplot_kw={'projection': 'polar'}, figsize=(6, 5))
    ax.set_title("CyBot Sensor Scan", fontsize=14)
    ax.set_rmax(50)
    ax.set_rticks([10, 20, 30, 40, 50])
    ax.set_rlabel_position(-22.5)

    canvas = FigureCanvasTkAgg(fig, master=left_frame)
    canvas.draw()
    canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)

    # Right: Controls and text
    right_frame = tk.Frame(main_frame)
    right_frame.pack(side=tk.RIGHT, fill=tk.BOTH)

    message_box_label = tk.Label(right_frame, text="Messages from CyBot:")
    message_box_label.pack(pady=(10, 0))

    message_box = tk.Text(right_frame, height=15, width=40, state=tk.DISABLED)
    message_box.pack(padx=10, pady=5)

    # Minimap
    minimap_label = tk.Label(right_frame, text="Minimap")
    minimap_label.pack(pady=(5, 0))

    minimap_canvas = tk.Canvas(right_frame, width=500, height=500, bg="white")
    minimap_canvas.pack()
    draw_minimap()

    # Controls
    control_frame = tk.Frame(right_frame)
    control_frame.pack(pady=10)
    create_control_button(control_frame, "↑", "w", 0, 1)
    create_control_button(control_frame, "←", "a", 1, 0)
    create_control_button(control_frame, "→", "d", 1, 2)
    create_control_button(control_frame, "↓", "s", 2, 1)

    utility_frame = tk.Frame(right_frame)
    utility_frame.pack(pady=10)

    tk.Button(utility_frame, text="Scan", width=10, command=send_scan).pack(pady=2)
    tk.Button(utility_frame, text="Quick Scan", width=10, command=send_quick_scan).pack(pady=2)
    tk.Button(utility_frame, text="Reset Bot", width=10, command=reset_minimap_bot).pack(pady=2)
    tk.Button(utility_frame, text="Quit", width=10, command=send_quit).pack(pady=2)

    threading.Thread(target=socket_thread, daemon=True).start()
    window.after(100, process_stop_queue)  # Start stop queue processor
    window.mainloop()

def create_control_button(parent, text, command_char, row, col):
    btn = tk.Button(parent, text=text, width=5, height=2)
    btn.grid(row=row, column=col)
    btn.bind("<ButtonPress-1>", lambda e: start_movement(command_char))
    btn.bind("<ButtonRelease-1>", lambda e: stop_movement(command_char))

def start_movement(command_char):
    if command_char not in movement_flags or not movement_flags[command_char]:
        movement_flags[command_char] = True
        thread = threading.Thread(target=movement_loop, args=(command_char,))
        movement_threads[command_char] = thread
        thread.start()

def stop_movement(command_char):
    movement_flags[command_char] = False
    stop_queue.put("x")  # Queue "x" for reliable stopping

def process_stop_queue():
    global gui_send_message
    if not stop_queue.empty() and gui_send_message == "wait\n":
        gui_send_message = stop_queue.get()
    window.after(100, process_stop_queue)

def movement_loop(command_char):
    while movement_flags.get(command_char, False):
        handle_command(command_char)
        time.sleep(0.2)

def send_quit():
    global gui_send_message
    gui_send_message = "quit\n"
    time.sleep(1)
    window.destroy()

def send_scan():
    global gui_send_message
    gui_send_message = "e"

def send_quick_scan():
    global gui_send_message
    gui_send_message = "q"

def send_command(command_char):
    global gui_send_message
    gui_send_message = command_char

def handle_command(command_char):
    update_minimap(command_char)
    send_command(command_char)

def update_minimap(command_char):
    global cybot_x, cybot_y, cybot_angle_deg

    if command_char == "w":
        rad = math.radians(cybot_angle_deg)
        cybot_x += step_distance * math.cos(rad)
        cybot_y -= step_distance * math.sin(rad)
    elif command_char == "s":
        rad = math.radians(cybot_angle_deg)
        cybot_x -= step_distance * math.cos(rad)
        cybot_y += step_distance * math.sin(rad)
    elif command_char == "a":
        cybot_angle_deg = (cybot_angle_deg + 5.69) % 360
    elif command_char == "d":
        cybot_angle_deg = (cybot_angle_deg - 5.69) % 360

    draw_minimap()

def reset_minimap_bot():
    global cybot_x, cybot_y, cybot_angle_deg
    cybot_x, cybot_y = 250, 250
    cybot_angle_deg = 90
    draw_minimap()

def draw_minimap():
    global minimap_canvas, cybot_x, cybot_y, cybot_angle_deg

    minimap_canvas.delete("all")
    for i in range(0, 501, 50):
        minimap_canvas.create_line(i, 0, i, 500, fill="#ddd")
        minimap_canvas.create_line(0, i, 500, i, fill="#ddd")

    bot_size = 10
    minimap_canvas.create_oval(cybot_x - bot_size, cybot_y - bot_size,
                               cybot_x + bot_size, cybot_y + bot_size,
                               fill="red")

    rad = math.radians(cybot_angle_deg)
    end_x = cybot_x + 15 * math.cos(rad)
    end_y = cybot_y - 15 * math.sin(rad)
    minimap_canvas.create_line(cybot_x, cybot_y, end_x, end_y, fill="blue", width=2)

def append_to_message_box(text):
    message_box.config(state=tk.NORMAL)
    message_box.insert(tk.END, text + "\n")
    message_box.see(tk.END)
    message_box.config(state=tk.DISABLED)

def socket_thread():
    global gui_send_message

    absolute_path = os.path.dirname(__file__)
    filename = os.path.join(absolute_path, 'sensor-scan.txt')

    HOST = "192.168.1.1"
    PORT = 288
    cybot_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    cybot_socket.connect((HOST, PORT))
    cybot = cybot_socket.makefile("rbw", buffering=0)

    send_message = "Hello\n"
    gui_send_message = "wait\n"
    cybot.write(send_message.encode())
    print("Sent to server: " + send_message)

    while send_message != 'quit\n':
        if send_message.strip() in ("e", "q") or decoded == "z":
            print("Requested Sensor scan from CyBot:\n")
            scan_lines = []

            with open(filename, 'w') as file_object:
                while True:
                    rx_message = cybot.readline()
                    decoded = rx_message.decode().strip()
                    if decoded == "SCAN_START":
                        break

                while True:
                    rx_message = cybot.readline()
                    decoded = rx_message.decode()
                    if decoded.strip() == "END":
                        break

                    print(decoded.strip())
                    file_object.write(decoded)
                    file_object.flush()
                    scan_lines.append(decoded)

            scan_lines_to_plot.clear()
            scan_lines_to_plot.extend(scan_lines)
            window.after(0, lambda: parse_and_plot_scan_data_polar(scan_lines_to_plot))
        else:
            rx_message = cybot.readline()
            decoded = rx_message.decode().strip()
            print("Received:", decoded)
            window.after(0, lambda msg=decoded: append_to_message_box(msg))

        while gui_send_message == "wait\n":
            time.sleep(.05)

        send_message = gui_send_message
        gui_send_message = "wait\n"
        cybot.write(send_message.encode())

    print("Client exiting...")
    time.sleep(2)
    cybot.close()
    cybot_socket.close()

def parse_and_plot_scan_data_polar(lines):
    global canvas, ax

    angles = []
    distances = []

    for line in lines:
        try:
            tokens = line.strip().split(",")
            if len(tokens) >= 2:
                angle = float(tokens[0])
                distance = float(tokens[1])
                if 0 < distance < 1000:
                    angles.append(angle)
                    distances.append(distance)
        except ValueError:
            continue

    if not angles or not distances:
        print("No valid data to plot.")
        return

    angles_rad = np.radians(angles)
    distances_np = np.array(distances)

    ax.clear()
    ax.set_title("CyBot Sensor Scan", fontsize=14)
    ax.set_rmax(50)
    ax.set_rmin(0)
    ax.set_rticks([10, 20, 30, 40, 50])
    ax.set_rlabel_position(-22.5)
    ax.plot(angles_rad, distances_np, color='blue', linewidth=2.0, marker='o', markersize=4)
    ax.set_rmax(50)
    ax.set_rmin(0)
    canvas.draw()

# Start GUI
main()
