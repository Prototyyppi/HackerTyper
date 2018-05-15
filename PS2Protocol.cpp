#include "PS2Protocol.h"
#define CLOCK 10

PS2Protocol::PS2Protocol(int8_t PS2_DATA_LINE, int8_t PS2_CLOCK_LINE) {
	_PS2_DATA_LINE = PS2_DATA_LINE;
	_PS2_CLOCK_LINE = PS2_CLOCK_LINE;
}

/* Poll host pins for data and device pins for typos */
uint8_t PS2Protocol::poll() {
	uint8_t ret;
	uint8_t clk_val, data_val;

	p_clk_val = digitalRead(_PS2_DATA_LINE);
	p_data_val = digitalRead(_PS2_CLOCK_LINE);
	if (!(p_clk_val && p_data_val))
		return 2;
	while (1) {
		clk_val = digitalRead(_PS2_DATA_LINE);

		}
			for (int i=0;i<50)
		data_val = digitalRead(_PS2_CLOCK_LINE);
		if (clk)
	}
	return ret;
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
	uint8_t ret;
	uint8_t clk_val, data_val, p_clk_val, p_data_val, trigger=0, cnt=0, result, vals=0;
	if(wait_for_start_condition() == 2)
		return 2;
	p_clk_val = digitalRead(_PS2_DATA_LINE);
	p_data_val = digitalRead(_PS2_CLOCK_LINE);
	while (1) {
		clk_val = digitalRead(_PS2_CLOCK_LINE);
			if (clk_val != p_clk_val) {
				trigger;
				p_clk_val = clk_val;
			}
		if (trigger) {
			trigger = 0;
			
			data_val = digitalRead(_PS2_DATA_LINE);
			if (data_val) {
				result |= data_val << cnt;
				vals++;
			}
			cnt++;
		}
		if(cnt == 9)
			if((vals % 2) == data_val)
				Serial.print("All Good");
			break;
	
	}
}

uint8_t PS2Protocol::transfer(uint8_t message, uint8_t parity_bit) {
	uint8_t parity = 0;
	/* First generate start condition (clock goes low, and data low)*/
	digitalWrite(_PS2_DATA_LINE, LOW);
	digitalWrite(_PS2_CLOCK_LINE, LOW);
	delay(CLOCK);

	/* Then transmit data */
	for(int i = 0; i < 8; i++) {
		digitalWrite(_PS2_CLOCK_LINE, HIGH);
		delay(CLOCK);
		if ((message << i) & 1) {
			digitalWrite(_PS2_DATA_LINE, HIGH);
			parity++;
		} else
			digitalWrite(_PS2_DATA_LINE, LOW);
		digitalWrite(_PS2_CLOCK_LINE, LOW);
		delay(CLOCK);
	}
	digitalWrite(_PS2_CLOCK_LINE, HIGH);
	delay(CLOCK);
	/* Last ctransfer parity bit */
	if((parity & 1) == 0)
		digitalWrite(_PS2_DATA_LINE, HIGH);
	else
		digitalWrite(_PS2_DATA_LINE, LOW);
	digitalWrite(_PS2_CLOCK_LINE, LOW);
	/* Stop signal */
	delay(CLOCK);
	digitalWrite(_PS2_DATA_LINE, HIGH);
	digitalWrite(_PS2_CLOCK_LINE, HIGH);
	delay(CLOCK);
}

uint8_t PS2Protocol::host_receive() {

}

uint8_t PS2Protocol::host_transfer(uint8_t message, uint8_t parity_bit) {

}
uint8_t PS2Protocol::calculate_parity(uint8_t message) {
	/* Even number of 1's -> parity bit 1, odd number -> 0 */
}