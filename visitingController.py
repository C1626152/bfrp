import microbit
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
	tX = "someHashcode"
# Wrong, work out how to actually validate for a 32bit string
elif type(rX) == str && len(rX) == 32:
	# Needs code to validate
	# store new block in dict obj
	currentBlock = rX
else:
	# Ensure that the script returns to sending signal code
	return


