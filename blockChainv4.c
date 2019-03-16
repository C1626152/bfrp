#include "stdio.h"
#include "time.h"
#include "stdlib.h"
#include "string.h"
#include "MicroBit.h"
#include <string>
#include "sha256.h"





// https://lancaster-university.github.io/microbit-docs/ubit/io/

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
// SHA256 hash call
void calc_sha_256(uint8_t hash[32], const void * input, size_t len);
// Check for new messages
char* checkNewMessages(char* newHash);
// Check for new devices within broadcast range
int checkNewDevices(char* guestHash);
// Manage connections
void connected(int k);
// Declare memory space
void run_once_initialisation();
// Create instance of usb connection
MicroBitSerial serial(USBTX, USBRX);

char *lastBlock;


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
		calc_sha_256(head->prevHash, "", strlen(""));
		// head should now read block data
		head->data = data;

		return head;
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
	calc_sha_256(toString(newBlock->prevHash, toString(*currentBlock), strlen(toString(*currentBlock)));
	
	// Update lastBlock
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
		hashPrinter(calc_sha_256(NULL, toString(*prev), strlen(toString(*prev))) ,SHA256_DIGEST_LENGTH);
		printf(" - ");
		hashPrinter(curr->prevHash, SHA256_DIGEST_LENGTH);
		if(hashCompare(calc_sha_256(NULL, toString(*prev), strlen(toString(*prev))), curr->prevHash))
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
		if(str1[i]!=str2[i]){
			return 0;
		}
		return 1;
}


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

    if(s.length() != 0)
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

  return 0;
}

// Function to handle connection status of beacon
// THIS NOW NEEDS RE-WRITING TO IMPLEMENT BLOCK CHAIN
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
	//String for handling incoming session Objectives rx'd over serial
	std::string objString
	/****************************
	Changed unsigned char x to be just char sessObjects

	MICROBIT_NO_DATA is a value of -1012 as an error code.
	As it was unsigned before, it would never equate, as it would always be read as positive.

	If sessObjects is a single character then OK. But, if you'll want strings, then this will need to modified.
	* 
	* CHLOE: No the seeObjects will be coming in the form of a string of hex numbers (already hashed), to save on size/space
	****************************/
	std::string objString = serial.read(ASYNC);
	int wait = 600*1000; //600 seconds in milliseconds

	ManagedString serialisationNumber = uBit.getSerial();
		// This will keep reading data until no more data is present
	while(sessionObjectives != MICROBIT_NO_DATA)
	{
		/*x = serial.read(ASYNC);
		// Is this method of assigning to variable correct?
		unsigned char* sessionObjectives = x;*/
		sessionObjectives = serial.read(ASYNC);

	}
	
	while(1)
	{

		// listen for radio signal
		// Interact with visiting device
		// https://lancaster-university.github.io/microbit-docs/ble/ble-connection-events/

		if(checkNewDevices(newHash))
		{
			connected(1);
			// Wait 10 minutes
			uBit.sleep(wait);

        	struct block* newBlock = addBlock(newHash, sessionObjectives, time(&timeNow), serialisationNumber);
			// Needs function to return completed blocks via USB to the base controller unit
			// Clarify pointer required to send the next block to the base unit
			// Confirm this is the correct pointer?	newBlock->newHash = (unsigned char*) hashData;
			serial.send(toString(*newBlock));

			// Send new block on radio
			// Confirm use of uBit message bus to pass data to BLE module
			// Clarify pointer required to send this block
			uBit.radio.datagram.send(toString(*newBlock));
			connected(0);

		}

		// release_fiber();
	}

}


// gcc tempBlockChain.c -o blockChain.c.o -lcrypto
// ./blockChain.c.o

// Made with help from: https://www.youtube.com/watch?v=1O-XnbRYJHM
