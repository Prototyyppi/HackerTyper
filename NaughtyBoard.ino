#include <Arduino.h>
#include "PS2Protocol.h"

/* Initialize protocol pins */
PS2Protocol ps2protocol(12, 13);

/* Declare names for pin numbers */
const int NUM_LED = 2;
const int CAPS_LED = 3;
const int SCROLL_LED = 4;

void setup() {
	// debug output at 9600 baud
	Serial.begin(9600);

	/* Led pins are output pins */
	pinMode(NUM_LED, OUTPUT);
	pinMode(CAPS_LED, OUTPUT);
	pinMode(SCROLL_LED, OUTPUT);
}


void loop() {
	uint8_t msg, parity;

	/* Wait for key press */
	while (!ps2protocol.wait_for_start_condition()) {
		/* Just wait... */
	}

	/* Receive data */
	msg = ps2protocol.receive();

	/* Check data parity */
	msg = ps2protocol.calculate_parity(msg);

	/* Transfer the same data */
	ps2protocol.transfer(msg, parity);

	Serial.print(msg, BIN);
	Serial.print(msg);

	}
