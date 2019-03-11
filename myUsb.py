import serial
import usb.core 
import usb.util

# Follow this: https://www.youtube.com/watch?v=xH_y05pIDTo

out = serial.Serial(port='/dev/ttyAMA0', baudrate=115200, timeout=1)

prePack = Queue()
incoming = Queue()

# Python script to handle data from RPi to microbit and back again
# To do:
"""
Function to read from USB (specific USB port)
Function to write to USB (specific port)
Function to package data correctly for USB
Main() function - Should act as an API
"""

# Queue class taken from:
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
    
    def to_list(self):
        with self.mutex:
            return list(self.queue)


# Function to send data over USB ()
def send(data):
	# serial must send data as strings
	while True:
		if isinstance(data, str):
			continue
		else:
			# Convert data to string
			data = str(data)
		# add data to queue
		prePack.enqueue(data)
		# Currently data is stored like so: prePack[data1, data2, data3[data3a, data3b, data3c]]
		# Use something similar to: "'{0}F{1}S{2}T'.format(first_data, second_data, third_data)" to package data?
		# Some consideration for packet contents and flags must be given so as to successfully
		# decide between which pieces of information to handle with USB receive

	# This is outside of loop so it gets sent
	# out.write(someVarStoringData)
	if prePack.isEmpty() == False:
		for x in prePack:
			out.write(x)

# Function to receive data over USB (New blocks)
def recieve(data):
	# Think about how data is going to be sent via USB (probs as strings)
	while True:
		incoming.enqueue(str(out.readline()))

	# split data if needed else return as a string
	# Must split extra data?


# Use following to test port numbers:
# python -m serial.tools.list_ports

# Use this to access port in terminal for testing:
# python -m serial.tools.miniterm <port_name>

# Reading for USB:
# http://www.brainboxes.com/faq/items/what-is-rts--cts-hardware-flow-control-
# https://pyserial.readthedocs.io/en/latest/pyserial_api.html





