#define MICROBIT_BLE_ENABLED 1
#include "MicroBit.h"
#include "MicroBitBLEManager.h"


MicroBit uBit;

// Begin beacon
void startAdvertising();
// Kill beacon
void stopAdvertising();

// power level of the broadcast beacon
uint8_t tx_power_level = 10;
// UIDD4 number
char UID_NAMESPACE[] = {0x20,0x0b,0x2d,0xd4,0xf6,0xd1,0x1b,0x16,0x8a,0xdc}; // sha-1 hash of "com.bittysoftware"
// UID instance number
char UID_INSTANCE[]  = {0x84, 0x81, 0x81, 0x72, 0x63, 0x36};
// Powers that are calibrated for beacon
const int8_t CALIBRATED_POWERS[] = {-49, -37, -33, -28, -25, -20, -15, -10};


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

void main(){
	while(1)
	{
		// Create thread for RX?
		// Add to assigned place in memory
		startAdvertising();

	}
}