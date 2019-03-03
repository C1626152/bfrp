import microbit
import radio

# turn radio on
radio.on()
# set channel, power to max, assign group
radio.config(channel=7, power=10, group=1)

tX = radio.send()
rX = radio.receive()
currentBlock = []
x = 0
# While loop to send beacon signal
while rX == False:


# Wrong, work out how to actually validate for a 16byte string
if type(rX) == bytes(16) && (rX == {0x20, 0x0b, 0x2d, 0xd4, 0xf6, 0xd1, 0x1b, 0x16, 0x8a ,0xdc, 0x84, 0x81, 0x81, 0x72, 0x63, 0x36}):
	tX(currentBlock[])
	# Needs code to validate
		if(rx == bytes(16) && (rx!={0x20, 0x0b, 0x2d, 0xd4, 0xf6, 0xd1, 0x1b, 0x16, 0x8a ,0xdc, 0x84, 0x81, 0x81, 0x72, 0x63, 0x36})):
			# store new block in dict obj
			x++
			currentBlock.update({(x):rX})

else:
	# Ensure that the script returns to sending signal code
	return


