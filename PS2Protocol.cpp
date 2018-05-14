#include "PS2Protocol.h"

PS2Protocol::PS2Protocol(int8_t PS2_DATA_LINE, int8_t PS2_CLOCK_LINE) {
	_PS2_DATA_LINE = PS2_DATA_LINE;
	_PS2_CLOCK_LINE = PS2_CLOCK_LINE;
}

uint8_t PS2Protocol::wait_for_start_condition() {
	uint8_t ret;
	uint8_t clk_val, data_val;
	clk_val = digitalRead(_PS2_DATA_LINE);
	data_val = digitalRead(_PS2_CLOCK_LINE);
	if (!clk_val && !data_val)
		ret = 0;
	else
		ret = 2;
	return ret;
}

uint8_t PS2Protocol::receive() {
	digitalRead(_pin, HIGH);
	delay(250);
	digitalWrite(_pin, LOW);
	delay(250);
}

uint8_t PS2Protocol::transfer(uint8_t message, uint8_t parity_bit) {
	/* First generate start condition (clock goes low, and data low)*/
	digitalWrite(_PS2_DATA_LINE, LOW);
	digitalWrite(_PS2_CLOCK_LINE, LOW);
	delay(40);

	/* Then transmit data */
	//Some nice functionality here

	/* Last ctransfer parity bit */

}

uint8_t PS2Protocol::calculate_parity(uint8_t message) {
	/* Even number of 1's -> parity bit 1, odd number -> 0 */
}