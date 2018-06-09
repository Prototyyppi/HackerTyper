#include "PS2Protocol.h"

/* Initialize protocol pins */
PS2Protocol* ps2protocol = new PS2Protocol(7, 4);

/* Declare names for pin numbers */
const int NUM_LED = 2;
const int CAPS_LED = 3;
const int SCROLL_LED = 4;

void setup() {
	// debug output at 9600 baud
	Serial.begin(9600);

	/* Led pins are output pins */
	//pinMode(NUM_LED, OUTPUT);
	//pinMode(CAPS_LED, OUTPUT);
	//pinMode(SCROLL_LED, OUTPUT);
}


void loop() {
  uint8_t msg, parity, ignore = 0;
  while (1) {


#if DEBUG
 
    ps2protocol->debug();
#endif
  
 
	/* Wait for key press */
	while (ps2protocol->wait_for_start_condition()){}
		/* Just wait... */
//Serial.print("Waiting for key press... ");
	/* Receive data */
	msg = ps2protocol->receive();
 
  if(msg == 240) {
    ignore = 1;
#if PRINT
    Serial.print("WAS release stroke");
#endif
    continue;
  }
  if (ignore == 1)
    ignore = 0;
 ps2protocol->charrify_hex(msg);
	/* Check data parity */
	//msg = ps2protocol->calculate_parity(msg);

	/* Transfer the same data */
//	ps2protocol.transfer(msg, parity);

	//Serial.print(msg, BIN);
	//Serial.print(msg);

	}
 }
