
checkNewMessages(char* newHash)
{
    if(usbReceivedData)
    {
        //some types of verification of valid data received
        memcpy(hash. newHash, SHA256_DIGEST_LENGTH);
    }
}

checkNewDevices(char* guestHash)
{
    if(radioReceivedData)
    {
        //some types of verification of valid data received
        memcpy(hash. radioReceivedData, SHA256_DIGEST_LENGTH);
    }
}

int main()
{
	char* newHash = malloc(sizeOf(char)*SHA256_DIGEST_LENGTH);
	time_t timeNow;
	int c,n,r;

	while(1)
	{
		// listen for radio signal

			// Send ACK
			// Send SYN
			// Wait for ACK

		// listen for USB data
			// Add add to assigned place in memory
		if(checkNewMessages(newHash))
		{
			addBlock(n, newHash, time(timeNow), serialisationNumber);
		}

		// Wait 10 minutes

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