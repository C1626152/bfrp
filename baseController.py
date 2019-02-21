import usb
from usb import Queue
import json
import urllib2
from hashlib import *
import random
import unittest
import Webbrowser
import os

"""
To Do:
	Function to store blockchain
	Function to read blockchain
	function to send manipulate JSON
	main() function
"""
# stores all current data
currentData = []
# Stores the current hash value
currentHash = ()
# Stores users hash number (For programming visiting device)
newUserHash = ''
# For hash output
hashOut = ''
# Hashes stored here with objectives as key value pairs
hashList = []

# ======================================================================
# Web page functionality built using code from https://anh.cs.luc.edu/python/hands-on/3.1/handsonHtml/webtemplates.html
# Web page template
wpgMessage = """<!DOCTYPE html>
<html>
<head>
<title>Found Data</title>
<style>
	span {
		background-color:SlateBlue;
		border:2px solid SlateBlue;
		display: block;
		margin: 5px;
		padding: 10px;
	}
	h3{
			font-family: "Times New Roman", Times, serif;
			color: Gray;
	}
	 p {
			font-family: "Calibri", Arial, serif;
	}
</style>
</head>
<body>
	<span>
		<h1>Found Data</h1>
		<div>
			<h3>Block Data:</h3>
				<p>Current data block contents: {blockData}</p>

		</div>
		<div>
			<h3>Hashes:</h3>
				<p>Current hash: {wpgHashes}</p>
		</div>
	<div>
		<h3> Session Objectives:</h3>
		<p>{wpgHashList}</p>
	</span>
</body>
</html>"""
# ======================================================================

wpg = open(results.html, w)
# Writes data into JSON object
# Currently unused (need to work out for C side of things)
def prepare(data):
	with open(packedData, 'w') as i:
		json.dump(data, i)

# Send data to html page and display page
def display(data, hashData, hashlist):
	blockData = Data
	wpgHashes = hashData
	wpgHashList = hashlist
	contents = wpgMessage.format(**locals())
	browseLocal(contents)

# Prepares web page for display
def browseLocal(webpageText = wpgMessage, filename='currentDataWPG.html'):
    '''Starts a webbrowser on a local file containing the text
    with given filename.'''
    strToFile(webpageText, filename)
    webbrowser.open("file:///" + os.path.abspath(filename))

# Writes string to a file
def strToFile(data, filename):
	outFile = open(filename, w)
	outFile.write(data)
	outFile.close()

# Write the session objectives and return as a hashnumber
# Also adds unhashed objectives to a dict obj
def writeSessionObj():
	tempObj = ''
	newSessionObj = raw_input("\nInput session objectives for this session:\n")
	tempObj = hashIt(newSessionObj)
	hashList[tempObj] = newSessionObj

# This hash function is theoretically insecure in this manner of use
def writeUserHash():
	first = raw_input("Input Users Name")
	second = str(randint(1000000000,9999999999))
	newUserHash = hashIt((first+second))

def hashIt(data):
	hashOut = str(hashlib.sha256(data))

# Future functionality is intended to modify this script and allow rewriting of the microbit on the fly
# def writeToMicrobit(data):
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

	try:
		out
	except:
		print("\nUSB Connection error!\n")

	while True:
		choice = input("Select Option:\n 1 - Write session objectives\n 2 - Read block data\n 3 - ###UNUSED CURRENTLY### 0 - Exit")
		if choice == 1:
			writeSessionObj()
			usb.send(sessionObjectives)
		elif choice == 2:
			try:
				display(data = currentData, hashData = currentHash, hashlist = hashList)
			except:
				print("\nError displaying data! D:\n")
"""
		elif choice == 3:
			writeUserHash()
			print("New user hash is: \n", newUserHash)
			writeToMicrobit(newUserHash)
"""
		# UNEXPECTED INDENT ERROR!?
		elif choice == 0:
			break

		if usb.receieve() == True:
			for i in usb.incoming:
			i = usb.incoming.dequeue(i)
			currentData.append(i)

"""
TO DO:
Include method of writing hashcode to visitor microbit
"""
# ============================================UNIT TESTS==============================================
class unitTests(unittest.TestCase):
	"""docstring for writeSessio"""
	def testHash(self):
		self.assertEqual(writeUserHash(this), 1EB79602411EF02CF6FE117897015FFF89F80FACE4ECCD50425C45149B148408)




