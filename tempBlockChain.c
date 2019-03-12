//#include "openssl/sha.h"
#define MICROBIT_BLE_ENABLED 1
#include "stdio.h"
#include "time.h"
#include "stdlib.h"
#include "string.h"
#include "MicroBit.h"
#include "MicroBitBLEManager.h
// https://lancaster-university.github.io/microbit-docs/ubit/io/
// or
// https://lancaster-university.github.io/microbit-docs/ubit/serial/#example
// Probably this one ^^^
// Need to begin including fibre?
#define SHA256_DIGEST_LENGTH 32

MicroBit uBit;

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
	ManagedString serialisationNumber;

// *head is the header of the linked list
}*head;


// Adds new block
struct block* addBlock(int data, unsigned char* newHash, time_t currentTime, ManagedString serialisationNumber);
// verifys chain integrity
void verifyChain();
// Compares hashes for validity checking
int hashCompare(unsigned char*,unsigned char*);
// Prints hash numbers
void hashPrinter(unsigned char hash[], int length);
// Turns unsigned char hashes into strings for SHA256 alg to interpret
unsigned char* toString(struct block);
// Print one block
// void printBlock();
// Check for new messages
char* checkNewMessages(char* newHash);
// Check for new devices within broadcast range
int checkNewDevices(char* guestHash);
// Begin beacon
void startAdvertising();
// Kill beacon
void stopAdvertising();
// Manage connections
void connected(int k);
/****************************
This is not needed, there already exists a sleep function in the Microbit.h.
Be warned, it's in milliseconds though, not seconds.
// wait to add block to chain
unsigned int sleep(unsigned int seconds);
*****************************/
// Declare memory space
void run_once_initialisation();
// Create instance of usb connection
MicroBitSerial serial(USBTX, USBRX);

char *lastBlock;
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
struct block* addBlock(int data, unsigned char* sessionObjectives, time_t currentTime, ManagedString serialisationNumber)
{
	// If head is empty generate genesis block
	if(head == NULL)
	{
		// Allocate an area of memory the size of a block
		// head=malloc returns an address in memory
		// head now equals a memory address
		head = (struct block*)malloc(sizeof(struct block));
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
	struct block *newBlock = (struct block*)malloc(sizeof(struct block));
	// creates area in memory for hashes to be stored for blocks
	char* hashData = (char*)malloc(sizeof(char)*SHA256_DIGEST_LENGTH);
	// Copy data from memory
	memcpy(hashData, sessionObjectives, SHA256_DIGEST_LENGTH);
	// New block gets linked to old block
	currentBlock->link = newBlock;
	// New blocks data is now at this memory address
	// Add own data here
	newBlock->data = data;
	// Add extra data:
	/*
	unsigned char sessionObjectives;
	int serialisationNumber;
	*/
	newBlock->currentTime = currentTime;
	// Add session objectives
	newBlock->newHash = (unsigned char*) hashData;
	newBlock->serialisationNumber = serialisationNumber;

	// Generate hash of current block and old block?
	SHA256(toString(*currentBlock), sizeof(*currentBlock), newBlock->prevHash);
	// Update lastBlock

	/*******************************
	Originally: strcpy(lastBlock, newHash, 32);
	newHash not declared previously, confirm this is the right data to set here.
	*******************************/
	strncpy(lastBlock, hashData, 32);

	return newBlock;
}

void run_once_initialisation()
{
	// Initialise microbit runtime environment
	uBit.init();
	uBit.display.scroll("ON");
	// Set baudrate for serial connection
	serial.baud(115200);
  lastBlock = (char*) malloc(sizeof(char) * 32);

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

//turns input into strings for hash alg
unsigned char* toString(struct block b)
{
	// Creates space in memory the size of next block
	unsigned char *str = (unsigned char*) malloc(sizeof(unsigned char)*sizeof(b));
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
	char x;

    if(x = serial.read(ASYNC))
    {
        //some types of verification of valid data received?
        // memcpy(hash. newHash, SHA256_DIGEST_LENGTH);
        return newHash;
    }
	return NULL;
}

// Check for guest devices hash (previous block) to compare and validate
int checkNewDevices(char* guestHash)
{
	ManagedString s = uBit.radio.datagram.recv();

    if(s != NULL)
    {
		struct block *currentBlock = head;
		while(currentBlock->link != NULL){
    		currentBlock = currentBlock->link;
    }
    //some types of verification of valid data received
		/********************
		Originally: hashCompare(s, currentBlock ->hash)

		No member hash, check that newHash is the value you want to compare.
		*********************/
    if(hashCompare(s, currentBlock->newHash)){
      /*************************************
      //I think you want to return the hash to be stored in a new block?
      //I think you'll want to calculate the hash of s as well though?
      //Rather than return the string that was received, unless that's already a hash?
      *********************/
      memcpy(guestHash,s);
      return 1;
    }
    else{
      return 0;
    }
  }
  return 0;
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
		return;
	}
}

// Main function

int main()
{

	run_once_initialisation();

	// Reserve a space in memory for newHashes
	char* newHash = (char*)malloc(sizeof(char)*SHA256_DIGEST_LENGTH);
	// Time (used for delaying block)
	time_t timeNow;
	// UNUSED: int c, n, r;
	/****************************
	Changed unsigned char x to be just char sessObjects

	MICROBIT_NO_DATA is a value of -1012 as an error code.
	As it was unsigned before, it would never equate, as it would always be read as positive.

	If sessObjects is a single character then OK. But, if you'll want strings, then this will need to modified.
	****************************/
	int sessObjects;
	int wait = 600*1000; //600 seconds in milliseconds
	/****************************************
	DEFINED AS TEMP, NEED TO SOURCE THEM CORRECTLY
	*****************************************/
	char sessionObjectives[] = "YAY";
	ManagedString serialisationNumber = uBit.getSerial();

	while(1)
	{
		// Create thread for RX?
		// Add to assigned place in memory
		startAdvertising();

		// listen for radio signal
		// Interact with visiting device
		// https://lancaster-university.github.io/microbit-docs/ble/ble-connection-events/

		if(checkNewDevices(newHash))
		{
			connected(1);
			// Wait 10 minutes
			uBit.sleep(wait);
        	// Check these values for correctness (some pointers missing?)
        	//
			/****************************
			SessionObjectives in below is not defined in this scope.
			If you are reading them from the serial connection then probably want a call before this to read them
			I've declared a variable above just to get rid of the error, but you'll need to make sure they are set in time for this call.
			Possibly the while loop below where you are reading them?

			serialisationNumber also not defined
			****************************/
        	struct block* newBlock = addBlock(newHash, sessionObjectives, time(&timeNow), serialisationNumber);
			// Needs function to return completed blocks via USB to the base controller unit
			// Clarify pointer required to send the next block to the base unit
			// Confirm this is the correct pointer?	newBlock->newHash = (unsigned char*) hashData;
			serial.send(toString(newBlock));
			// Send new block on radio
			// Confirm use of uBit message bus to pass data to BLE module
			// Clarify pointer required to send this block
			uBit.radio.datagram.send(toString(newBlock));
			connected(0);

		}
		// This will keep reading data until no more data is present
		while(sessObjects != MICROBIT_NO_DATA)
		{
		    /*x = serial.read(ASYNC);
		    // Is this method of assigning to variable correct?
		    unsigned char* sessObjects = x;*/
			sessObjects = serial.read(ASYNC);

		}
		// release_fiber();
	}

}


// gcc tempBlockChain.c -o blockChain.c.o -lcrypto
// ./blockChain.c.o

// Made with help from: https://www.youtube.com/watch?v=1O-XnbRYJHM
