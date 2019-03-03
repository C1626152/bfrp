#include "openssl/sha.h"
#include "stdio.h"
#include "time.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"

#include "MicroBit.h"
// https://lancaster-university.github.io/microbit-docs/ubit/io/
// or
// https://lancaster-university.github.io/microbit-docs/ubit/serial/#example
// Probably this one ^^^
// Need to begin including fibre


// Create block structure
struct block
{

	// Must add Hashcode1, Hashcode2, currentTime, sessionObjectives, serialisationNumber
	unsigned char prevHash[SHA256_DIGEST_LENGTH];
	int data;
	struct block *link;
	unsigned char* ownHashID;
	unsigned char* guestHashID;
	time_t currentTime;
	unsigned char* newHash;
	int serialisationNumber;


// *head is the header pointing to the next ref in the chain?
}*head;
// Adds new block
void addBlock(int data, unsigned char* newHash, time_t currentTime, int serialisationNumber);
// verifys chain integrity
void verifyChain();
// Compares hashes for validity checking
int hashCompare(unsigned char*,unsigned char*);
// Prints hash numbers
void hashPrinter();
// Turns unsigned char hashes into strings for SHA256 alg to interpret
unsigned char* toString(struct block);
// Print one block
// void printBlock();
// Check for new messages
void checkNewMessages(char* newHash);
// Check for new devices within broadcast range
void checkNewDevices(char* guestHash);
// Begin beacon
void startAdvertising();
// Kill beacon
void stopAdvertising();
// Manage connections
void connected(int k);
// wait to add block to chain
unsigned int sleep(unsigned int seconds);


// power level of the broadcast beacon
uint8_t tx_power_level = 10;
// UIDD4 number
char UID_NAMESPACE[] = {0x20,0x0b,0x2d,0xd4,0xf6,0xd1,0x1b,0x16,0x8a,0xdc}; // sha-1 hash of "com.bittysoftware" 
// UID instance number
char UID_INSTANCE[]  = {0x84, 0x81, 0x81, 0x72, 0x63, 0x36};
// Powers that are calibrated for beacon
const int8_t CALIBRATED_POWERS[] = {-49, -37, -33, -28, -25, -20, -15, -10};


// add a block to the chain
// Need some help discussing the workings of this
void addBlock(int data, unsigned char* sessionObjectives, time_t currentTime, int serialisationNumber)
{
	// If head is empty generate genesis block
	if(head == NULL)
	{
		// Allocate an area of memory the size of a block
		// head=malloc returns an address in memory
		// head now equals a memory address
		head = malloc(sizeof(struct block));
		// Hash "" and the size of "" and store in previous hash location(retrived from head)
		SHA256((unsigned char*)"", sizeof(""), head->prevHash);
		// head should now read block data
		head->data = data;
		return;
	}
	struct block *currentBlock = head;
	// While there is a previous block (i.e blockchain is valid) do:
	while(currentBlock->link)
		// link between blocks
		currentBlock = currentBlock->link;
	// New block created in memory with malloc
	struct block *newBlock = malloc(sizeof(struct block));
	// creates area in memory for hashes to be stored for blocks
	char* hashData = malloc(sizeof(char)*SHA256_DIGEST_LENGTH);
	// Copy data from memory
	memcpy(hashData, sessionObjectives, SHA256_DIGEST_LENGTH);
	// New block gets linked to old block
	currentBlock->link = newBlock;
	// New blocks data is now at this memory address
	// Add own data here
	newBlock->data = data;
	// Add extra data:
	/*
	time_t currentTime;
	unsigned char sessionObjectives;
	int serialisationNumber;
	*/
	// Add session objectives
	newBlock->newHash = (unsigned char*) hashData;
	newBlock->serialisationNumber = serialisationNumber;

	// Generate hash of current block and old block?
	SHA256(toString(*currentBlock), sizeof(*currentBlock), newBlock->prevHash);
}

// Possible to remove this(?)
void verifyChain()
{
	// If empty, say so
	if(head == NULL)
	{
		printf("Blockchain is empty, add some blocks!\n");
		return;
	}
	struct block *curr = head->link, *prev=head;
	int count = 1;
	while(curr)
	{
		printf("%d\t[%d]\t\n", count++, curr->data);
		hashPrinter(SHA256(toString(*prev), sizeof(*prev), NULL) ,SHA256_DIGEST_LENGTH);
		printf(" - ");
		hashPrinter(curr->prevHash, SHA256_DIGEST_LENGTH);
		if(hashCompare(SHA256(toString(*prev), sizeof(*prev), NULL), curr->prevHash))
			printf("Verified!\n");
		else
			printf("Verification Failed! :(\n");
		prev = curr;
		curr = curr->link;
	}
}

//What is this for?
unsigned char* toString(struct block b)
{
	// Creates space in memory the size of next block
	unsigned char *str = malloc(sizeof(unsigned char)*sizeof(b));
	// Copy it into a string
	memcpy(str, &b, sizeof(b));
	return str;
	// This is so SHA256 can accept any data fed to it rather than rewrite the SHA256 alg
}

// Print hash of block (by loc)
void hashPrinter(unsigned char hash[], int length)
{
	for(int i = 0; i<length; i++)
		printf("%02x", hash[i]);
}

// Compare hash numbers
int hashCompare(unsigned char *str1, unsigned char *str2)
{
	for(int i = 0; i<SHA256_DIGEST_LENGTH; i++)
		if(str1[i]!=str2[i])
			return 0;
		return 1;
}

// Print block and contents??
// Modified to include other block data (Guest hash, time created and session objectives)

// void printBlock(struct block *b)
// {
// 	printf("%p\t",b);
// 	hashPrinter(b->prevHash,sizeof(b->prevHash));
// 	printf("Block hash: \t[%d]\t \n",b->data);
// 	printf("Block contents:\n");
// 	printf("Guest hash: %s \n", b->guestHashID);
// 	printf("Time block created: %ld \n", b->currentTime);
// 	printf("Session Objectives: %s \n", b->newHash);
// 	printf("%p\n",b->link);

// }

// Check for messages from usb
char* checkNewMessages(char* newHash)
{
    if(usbReceivedData)
    {
        //some types of verification of valid data received?
        // memcpy(hash. newHash, SHA256_DIGEST_LENGTH);
        return newHash;
    }
    else{
    	return 0;
    }
}

// Check for guest devices hash (previous block) to compare and validate
char* checkNewDevices(char* guestHash)
{
    if(radioReceivedData)
    {
    	char *lastblock = NULL;
		struct block *currentBlock = head;
		while(currentBlock->next !=null){
    		currentBlock = currentBlock->next;
    	}
        //some types of verification of valid data received
        if(guestHash == hashCompare(guestHash, currentblock->hash)){
        	return 1;
        }
        else{
        	return 0;
        }
    }
    else{
    	return 0;
    }
}

// Handles advertising start for beacon
void startAdvertising() {
	uBit.bleManager.advertiseEddystoneUid(UID_NAMESPACE, UID_INSTANCE, CALIBRATED_POWERS[tx_power_level-1], false);
	uBit.bleManager.setTransmitPower(20);
	uBit.display.scroll("+ADV+");
}

// Kills advertising beacon
void stopAdvertising() {
    uBit.bleManager.stopAdvertising();
    uBit.display.scroll("-ADV-");
}

// Function to handle connection status of beacon
void connected(int k) {
	if(k == 1){
		stopAdvertising();
	}
	else if(k==0){
		startAdvertising();
	}
	else{
		return 0;
	}
}

// Main function

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
	// Is unsigned char the correct type?
	unsigned char x = serial.read(ASYNC);
	int wait = 600;

	// Create instance of usb connection
	MicroBitSerial serial(USBTX, USBRX);
	serial.baud(115200);

	while(1)
	{

		// Create thread for RX?
		// Add to assigned place in memory
		startAdvertising()

		// listen for radio signal
		// Interact with visiting device

		// Possibly change this function to be more appropriate?
		// https://lancaster-university.github.io/microbit-docs/ble/ble-connection-events/
		if(checkNewDevices(guestHash))
		{
			connected(1);
			// Wait 10 minutes
			wait = sleep(wait);
        	// Check these values for correctness (some pointers missing)
        	addBlock(newHash, sessionObjectives, time(timeNow), serialisationNumber);
			// Needs function to return completed blocks via USB to the base controller unit
			// Send new block on radio
			connected(0);

		}
		
		while(x != MICROBIT_NO_DATA)
		{
		    x = serial.read(ASYNC);
		    // Is this method of assigning to variable correct?
		    unsigned char* sessObjects = x;
		}
		// release_fiber();
		
	}

}


// gcc tempBlockChain.c -o blockChain.c.o -lcrypto
// ./blockChain.c.o

// Made with help from: https://www.youtube.com/watch?v=1O-XnbRYJHM
