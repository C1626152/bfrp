import usb
from usb import Queue
import json
import urllib2
from hashlib import *
import random


"""
To Do:
	Function to store blockchain
	Function to read blockchain
	function to package as JSON
	function to send manipulate JSON and display on .html page locally
	main() function
"""
currentData = []
currentHash = ()
newUserHash = ''
hashOut = ''

# Writes data into JSON object
def prepare(data):
	with open(packedData, 'w') as i:
		json.dump(data, i)

# Send data to html page
def display(data, hashData):
	print("""<!DOCTYPE html>
	<html>
	<head>
	<title>Found Data</title>
	</head>
	<body>
		<span>
			<h1>Found Data:</h1>
			<div>
				<h3>Block Data</h3>""")
	# Print block data
	for i in data:
		print(i)

	print("""		</div>
			<div>
				<h3>Hashes</h3>""")
	# Print hashes of blocks
	for k in hashData:
		print(k)

	print("""		</div>
		</span>
	</body>
	</html>""")

def writeSessionObj():
	newSessionObj = raw_input("Input session objectives for this session:\n")
	sessionObjectives = hashIt(newSessionObj)

def writeUserHash():
	first = raw_input("Input Users Name")
	second = str(randint(1000000000,9999999999))
	newUserHash = hashIt((first+second))

def hashIt(data):
	hashOut = str(hashlib.sha256(data))

def writeToMicrobit(data):
	# Need to find out how to write data to microbit with python
	
	# Treat this like a document?
	"""import microbit
	import radio

	# turn radio on
	radio.on()
	# set channel, power to max, assign group
	radio.config(channel=7, power=10, group=1)

	tX = radio.send()
	rX = radio.receive()
	dict currentBlock = []

	# While loop to send beacon signal
	while rX == False:
		tX = "SYN"
	if  rX == "ACK":
		# Placeholder in use here
		tX = '%s' % (data)
	# Wrong, work out how to actually validate for a 32bit string
	elif type(rX) == str && len(rX) == 32:
		# Needs code to validate
		# store new block in dict obj
		currentBlock = rX
	else:
		# Ensure that the script returns to sending signal code
		return"""


def main():

	while True:
		choice = input("Select Option:\n 1 - Write session objectives\n 2 - Read block data\n0 - Exit")
		if choice == 1:
			writeSessionObj()
			usb.send(sessionObjectives)
		elif choice == 2:
			display(data=currentData, hashData=currentHash)
"""
		elif choice == 3:
			writeUserHash()
			print("New user hash is: \n", newUserHash)
			writeToMicrobit(newUserHash)
"""
		elif choice == 0:
			break


"""
TO DO:
Include method of writing hashcode to visitor microbit
"""




