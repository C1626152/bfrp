import serial

out = serial.Serial('/dev/ttyUSB0', 9600)

# Python script to handle data from RPi to microbit and back again
# To do:
"""
Function to read from USB (specific USB port)
Function to write to USB (specific port)
Function to package data correctly for USB
Main() function - Should act as an API
"""

# Queue taken from:
# http://interactivepython.org/courselib/static/pythonds/BasicDS/ImplementingaQueueinPython.html
class Queue:
    def __init__(self):
        self.items = []

    def isEmpty(self):
        return self.items == []

    def enqueue(self, item):
        self.items.insert(0,item)

    def dequeue(self):
        return self.items.pop()

    def size(self):
        return len(self.items)

prePack = Queue()
# Function to send data over USB ()
def send(data):
	# serial must send data as strings
	while True:
		# Convert data to string
		data = str(data)
		# add data to queue
		prePack.enqueue(data)
		# Use something similar to: "'{0}F{1}S{2}T'.format(first_data, second_data, third_data)" to package data
		# 
		# 
		# out.write(someVarStoringData)

# Function to receive data over USB (New blocks)
def recieve(data):
	# Think about how data is going to be sent via USB (probs as strings)
	# Must split extra data?