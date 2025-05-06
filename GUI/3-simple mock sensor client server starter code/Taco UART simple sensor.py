# Author: Gage Baker
# Date: 05/06/2025

# Serial library:  https://pyserial.readthedocs.io/en/latest/shortintro.html 
import serial
import time # Time library   
# Socket library:  https://realpython.com/python-sockets/  
# See: Background, Socket API Overview, and TCP Sockets  
import socket   

import os  # import function for finding absolute path to this python script

# A little python magic to make it more convient for you to adjust where you want the data file to live
# Link for more info: https://towardsthecloud.com/get-relative-path-python 
absolute_path = os.path.dirname(__file__) # Absoult path to this python script
relative_path = "./"   # Path to sensor data file relative to this python script (./ means data file is in the same directory as this python script)
full_path = os.path.join(absolute_path, relative_path) # Full path to sensor data file
filename = 'sensor-scan.txt' # Name of file you want to store sensor data from your sensor scan command


cybot = serial.Serial('COM100', 115200)  # UART (Make sure you are using the correct COM port and Baud rate!!)

# Send some text: Either 1) Choose "Hello" or 2) have the user enter text to send
send_message = "Hello\n"                            # 1) Hard code message to "Hello", or
# send_message = input("Enter a message:") + '\n'   # 2) Have user enter text

cybot.write(send_message.encode()) # Convert String to bytes (i.e., encode), and send data to the server

print("Sent to server: " + send_message) 

# Send messges to server until user sends "quit"
while send_message != 'quit\n':

        # Check if a sensor scan command has been sent
        if (send_message == "M\n") or (send_message == "m\n"):

                print("Requested Sensor scan from Cybot:\n")
                rx_message = bytearray(1) # Initialize a byte array

                # Create or overwrite existing sensor scan data file
                file_object = open(full_path + filename,'w') # Open the file: file_object is just a variable for the file "handler" returned by open()

                while (rx_message.decode() != "END\n"): # Collect sensor data until "END" recieved
                        rx_message = cybot.readline()   # Wait for sensor response, readline expects message to end with "\n"
                        file_object.write(rx_message.decode())  # Write a line of sensor data to the file
                        print(rx_message.decode()) # Convert message from bytes to String (i.e., decode), then print

                file_object.close() # Important to close file once you are done with it!!                

        else:                
                print("Waiting for server reply\n")
                rx_message = cybot.readline()      # Wait for a message, readline expects message to end with "\n"
                print("Got a message from server: " + rx_message.decode() + "\n") # Convert message from bytes to String (i.e., decode)
        
        send_message = input("Enter a message (enter quit to exit):") + '\n' # Enter next message to send to server
        cybot.write(send_message.encode()) # Convert String to bytes (i.e., encode), and send data to the server
        
print("Client exiting, and closing file descriptor, and/or network socket\n")
time.sleep(2) # Sleep for 2 seconds
cybot.close() # Close file object associated with the socket or UART
#cybot_socket.close()  # Close the socket (NOTE: comment out if using UART interface, only use for network socket option)
