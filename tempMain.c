


char* checkNewMessages(char* newHash)
{
    if(usbReceivedData)
    {
        //some types of verification of valid data received
        // memcpy(hash. newHash, SHA256_DIGEST_LENGTH);
        return newHash;
    }
    else{
    	return NULL;
    }
}

char* checkNewDevices(char* guestHash)
{
    if(radioReceivedData)
    {
        //some types of verification of valid data received
        // memcpy(hash. radioReceivedData, SHA256_DIGEST_LENGTH);
        return guestHash;
    }
    else{
    	return NULL;
    }
}

void startAdvertising() {
	uBit.bleManager.advertiseEddystoneUid(UID_NAMESPACE, UID_INSTANCE, CALIBRATED_POWERS[tx_power_level-1], false);
	uBit.bleManager.setTransmitPower(20);
	uBit.display.scroll("ADV");
}

void stopAdvertising() {
    uBit.bleManager.stopAdvertising();
    uBit.display.scroll("OFF");
}

// Wait function to delay adding a block
void waitFor (unsigned int secs) {
	// Need to wait for 10 mins
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
}

int main()
{
	// Initialise microbit runtime environment
	uBit.init();
	uBit.display.scroll("ON");

	// power level of the broadcast beacon
	uint8_t tx_power_level = 10;
	// UIDD4 number
	char UID_NAMESPACE[] = {0x20,0x0b,0x2d,0xd4,0xf6,0xd1,0x1b,0x16,0x8a,0xdc}; // sha-1 hash of "com.bittysoftware" 
	// UID instance number
	char UID_INSTANCE[]  = {0x84, 0x81, 0x81, 0x72, 0x63, 0x36};
	// Powers that are calibrated for beacon
	const int8_t CALIBRATED_POWERS[] = {-49, -37, -33, -28, -25, -20, -15, -10};
	// Reserve a space in memory for newHashes
	char* newHash = malloc(sizeOf(char)*SHA256_DIGEST_LENGTH);
	// Time (used for delaying block) 
	time_t timeNow;
	int c,n,r, connected;
	char x;

	while(1)
	{
		// Create instance of usb connection
		MicroBitSerial serial(USBTX, USBRX);
		// Create thread for RX?
			// Add add to assigned place in memory
		startAdvertising()

		// listen for radio signal

			// Send ACK
			// Send SYN
			// Wait for ACK

		// Possibly change this function to be more appropriate?
		// https://lancaster-university.github.io/microbit-docs/ble/ble-connection-events/
		if(checkNewDevices(guestHash))
		{
			stopAdvertising()
			connected = 1;
			// Wait 10 minutes
			addBlock(n, newHash, time(timeNow), serialisationNumber);
			startAdvertising()

		}
		
		serial.baud(115200);
		while(1)
		{
		    x = serial.read(ASYNC);
		    if(x != MICROBIT_NO_DATA)
		      serial.send(x);
		}
		release_fiber();
		

		// Send completed block on USB

		// Send new block on radio
	}

}
/*
addblock(char* hash)
{
    malloc hashdata = (sizeOf(char)*SHA256_DIGEST_LENGTH);
    memcpy(hashdata, hash, SHA256_DIGEST_LENGTH);
    HashBlockFunction();
}
*/

/*
Sample code for beacon
char UID_NAMESPACE[] = {0x0E,0x67,0x47,0x04,0x42,0xD0,0x14,0x06,0xD5,0x83}; // sha-1 hash of "com.bittysoftware" 
char UID_INSTANCE[]  = {0x00 , 0x00, 0x00, 0x00, 0x00, 0x01};
const int8_t CALIBRATED_POWERS[] = {-49, -37, -33, -28, -25, -20, -15, -10};

uint8_t advertising = 0;
uint8_t tx_power_level = 6;

void startAdvertising() {
    uBit.bleManager.advertiseEddystoneUid(UID_NAMESPACE, UID_INSTANCE, CALIBRATED_POWERS[tx_power_level-1], false);
    uBit.bleManager.setTransmitPower(6);
    uBit.display.scroll("ADV");
    advertising = 1;
}
*/