#include "openssl/sha.h"
#include "stdio.h"
#include "time.h"
#include "stdlib.h"
#include "string.h"
// #include "MicroBit.h"
// https://lancaster-university.github.io/microbit-docs/ubit/io/
// or
// https://lancaster-university.github.io/microbit-docs/ubit/serial/#example
// Probably this one ^^^


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
	unsigned char* sessionObjectives;
	int serialisationNumber;


// *head is the header pointing to the next ref in the chain?
}*head;
// Adds new block
void addBlock(int data, unsigned char* sessionObjectives, time_t currentTime, int serialisationNumber);
// verifys chain integrity
void verifyChain();
// alters the block at position n [redundent feature]
void alterNthBlock(int n,int newData);
// Compares hashes for validity checking
int hashCompare(unsigned char*,unsigned char*);
// Prints hash numbers
void hashPrinter();
// Turns unsigned char hashes into strings for SHA256 alg to interpret
unsigned char* toString(struct block);
// Print one block
void printBlock();
// Print all blocks
void printAllBlocks();



// add a block to the chain
// Need some help discussing the workings of this
void addBlock(int data, unsigned char* sessionObjectives, time_t currentTime, int serialisationNumber)
{
	// If head is empty generate genesis block
	if(head==NULL)
	{
		// Allocate an area of memory the size of a block
		// head=malloc returns an address in memory
		// head now equals a memory address
		head=malloc(sizeof(struct block));
		// Hash "" and the size of "" and store in previous hash location(retrived from head)
		SHA256((unsigned char*)"",sizeof(""),head->prevHash);
		// head should now read block data
		head->data=data;
		return;
	}
	struct block *currentBlock=head;
	// While there is a previous block (i.e blockchain is valid) do:
	while(currentBlock->link)
		// link between blocks
		currentBlock=currentBlock->link;
	// New block created in memory with malloc
	struct block *newBlock=malloc(sizeof(struct block));
	// New block gets linked to old block
	currentBlock->link=newBlock;
	// New blocks data is now at this memory address
	// Add own data here
	newBlock->data=data;
	// Add extra data:
	/*
	time_t currentTime;
	unsigned char sessionObjectives;
	int serialisationNumber;
	*/
	// Add session objectives
	newBlock->sessionObjectives = sessionObjectives;
	newBlock->serialisationNumber = serialisationNumber;

	// Generate hash of current block and old block?
	SHA256(toString(*currentBlock),sizeof(*currentBlock),newBlock->prevHash);
}

// Need help explaining this one
/*line 73 (for me, but it's where the hashPrinter call is).
he generates the hash of the previous block, fine. doesn't store it because we aren't modifying or writing, just checking it against the value that is already stored. 
He then prints the stored version on line 75 with the second call to hashPrinter. so you would be able to see that it matches or differes.

However, his next line is to hashCompare, where he generates the hash of the previous block a second time. so he's repeating a step, so this part really takes twice as long as it needs to. 

This could be modifed to remove the two calls to hashPrinter. Instead, it would be better to inside hashCompare print str1 and str2 in the same way to give you that visual comparison, you're then only generating it once. 

equally, if it matches, do you really want it printed out? it would be better if it only printed it out when it failed, otherwise the screen will get very cluttered. This one is my opinion, and it depends on what you want from the program. if printing the valid data is useful to you then that's fine.
*/
void verifyChain()
{
	// If empty, say so
	if(head==NULL)
	{
		printf("Blockchain is empty, add some blocks!\n");
		return;
	}
	struct block *curr=head->link,*prev=head;
	int count=1;
	while(curr)
	{
		printf("%d\t[%d]\t\n",count++,curr->data);
		hashPrinter(SHA256(toString(*prev),sizeof(*prev),NULL),SHA256_DIGEST_LENGTH);
		printf(" - ");
		hashPrinter(curr->prevHash, SHA256_DIGEST_LENGTH);
		if(hashCompare(SHA256(toString(*prev),sizeof(*prev),NULL),curr->prevHash))
			printf("Verified!\n");
		else
			printf("Verification Failed! :(\n");
		prev=curr;
		curr=curr->link;
	}
}
// Alter the block at position n with new data
// Should this be necessary?
void alterNthBlock(int n, int newData)
{
// n = block position
// newData = new data
	// If head points to NULL, block is not valid
	struct block *curr=head;
	if(curr==NULL)
	{
		printf("%ith block does not exist! :( \n",n);
		return;

	}
	int count = 0;
	// while n is valid:
	while(count!=n)
	{
		// If head of next block points to null and count !=n then block is invalid
		// (Genesis block)
		if(curr->link==NULL && count!=n)
		{
			printf("Nth block does not exist! :( \n");
			return;
		}
		// Else if valid:
		else if(count==n)
			// Stop looping
			break;
		// Current becomes next block
		curr=curr->link;
		count++;
	}
	// Print fore and aft data
	printf("Before: ");
		printBlock(curr);
	curr->data=newData;
	printf("\nAfter: ");
		printBlock(curr);
	printf("\n");
}

//What is this for?
unsigned char* toString(struct block b)
{
	// Creates space in memory the size of next block
	unsigned char *str=malloc(sizeof(unsigned char)*sizeof(b));
	// Copy it into a string
	memcpy(str,&b,sizeof(b));
	return str;
	// This is so SHA256 can accept any data fed to it rather than rewrite the SHA256 alg
}

// Print hash of block (by loc)
void hashPrinter(unsigned char hash[], int length)
{
	for(int i=0;i<length;i++)
		printf("%02x",hash[i]);
}

// Compare hash numbers
int hashCompare(unsigned char *str1,unsigned char *str2)
{
	for(int i=0;i<SHA256_DIGEST_LENGTH;i++)
		if(str1[i]!=str2[i])
			return 0;
		return 1;
}

// Print block and contents??
// Modified to include other block data (Guest hash, time created and session objectives)

void printBlock(struct block *b)
{
	printf("%p\t",b);	
	hashPrinter(b->prevHash,sizeof(b->prevHash));
	printf("Block hash: \t[%d]\t \n",b->data);
	printf("Block contents:\n");
	printf("Guest hash: %s \n", b->guestHashID);
	printf("Time block created: %ld \n", b->currentTime);
	printf("Session Objectives: %s \n", b->sessionObjectives)
	printf("%p\n",b->link);

}

// Print all
void printAllBlocks()
{
	struct block *curr=head;
	int count=0;
	while(curr)
	{
		printBlock(curr);
		curr=curr->link;
	}

}

// ==========================================
// =				TO DO 					=
// ==========================================
/*
	-Build session objectives code
		-malloc(sizeof(char)*length)
			-dont forget to use free(string)


*/

// Main function
// Switch statements to let user select functions
// This needs rewriting to allow for radio-triggered blockchain interactions instead
int main()
{
	int c,n,r;
	// time_t timeNow;
	// unsigned char* ownHashID -> "SomeHash4Me";
	// unsigned char* guestHashID -> "SomeHash4U";
	// time_t currentTime -> currentTime;
	// unsigned char* sessionObjectives -> "These are some objectives";

	printf("1)addBlock\n2)add n random blocks\n3)alter nth block\n4)print all blocks\n5)verify chain\n");
	while(1)
	{
		printf("Choice: ");
		scanf("%d",&c);
		switch(c)
		{
			case 1:
				printf("Enter data: ");
				scanf("%d",&n);
				//int data, unsigned char* sessionObjectives, time_t currentTime, int serialisationNumber
				addBlock(n, "Delay", time(&timeNow), 1);
				break;
			case 2:
				printf("How Many blocks to enter?: ");
				scanf("%d",&n);
				for(int i=0; i<n;i++)
				{
					r=rand()%(n*10);
					printf("Entering: %d\n",r);
					addBlock(r);
				}
				break;
			case 3:
				printf("Which block to alter?: ");
				scanf("%d",&n);
				printf("Enter value: ");
				scanf("%d",&r);
				alterNthBlock(n,r);
				break;
			case 4:
				printAllBlocks();
				break;
			case 5:
				verifyChain();
				break;
			default:
				printf("Bad choice! :((((\n");
				break;

		}
	}
}


// gcc blockChain2.c -o blockChain.c.o -lcrypto
// ./blockChain.c.o

// Made with help from: https://www.youtube.com/watch?v=1O-XnbRYJHM





