#include "PS2Protocol.h"
#include "PS2CharSet.h"
#include "Arduino.h"
#define CLOCK 10

PS2Protocol::PS2Protocol(int8_t PS2_DATA_LINE, int8_t PS2_CLOCK_LINE) {
	_PS2_DATA_LINE = PS2_DATA_LINE;
	_PS2_CLOCK_LINE = PS2_CLOCK_LINE;
   pinMode(_PS2_DATA_LINE, INPUT);
   pinMode(_PS2_CLOCK_LINE, INPUT); 
}

/* Poll host pins for data and device pins for typos */
uint8_t PS2Protocol::poll() {
	uint8_t ret;
	uint8_t clk_val, data_val;
  uint8_t p_clk_val, p_data_val;
 
	p_clk_val = digitalRead(_PS2_DATA_LINE);
	p_data_val = digitalRead(_PS2_CLOCK_LINE);
	if (!(p_clk_val && p_data_val))
		return 2;
	while (1) {
		clk_val = digitalRead(_PS2_DATA_LINE);

		}
			for (int i=0; i<50; i++)
		data_val = digitalRead(_PS2_CLOCK_LINE);
		if (clk_val)
	return ret;
}

uint8_t PS2Protocol::wait_for_start_condition() {
	//uint8_t ret;
	uint8_t clk_val, data_val;
	clk_val = digitalRead(_PS2_DATA_LINE);
	data_val = digitalRead(_PS2_CLOCK_LINE);
  /* following is a test
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
 
  Serial.print("Waiting...");
  Serial.print(data_val);
  Serial.print(clk_val);
   */
	if (clk_val == 0 && data_val == 0)
		return 0;
	else
		return 255;
	
}

void PS2Protocol::debug() {
  digitalWrite(LED_BUILTIN, digitalRead(_PS2_DATA_LINE));
  digitalWrite(LED_BUILTIN, digitalRead(_PS2_DATA_LINE));
  Serial.print(digitalRead(_PS2_CLOCK_LINE));
}

bool PS2Protocol::is_release_stroke(uint16_t msg) {
  if(msg == 240)
    return true;
  return false;
}

uint8_t PS2Protocol::receive() {
	uint16_t ret, golden_trigger = 0;
	uint16_t clk_val, data_val = 0, p_clk_val = 0, p_data_val=0, trigger=0, cnt=0, result=0;
	/*if(wait_for_start_condition() == 2)
		return 2;
   
	p_clk_val = digitalRead(_PS2_DATA_LINE);
	p_data_val = digitalRead(_PS2_CLOCK_LINE);
   */
	while (!golden_trigger) {
		clk_val = digitalRead(_PS2_CLOCK_LINE);
    data_val = digitalRead(_PS2_DATA_LINE);
		if (clk_val == 1 && p_clk_val == 0)
				trigger = 1;
    p_clk_val = clk_val;
		if (trigger) {
			trigger = 0;
      result |= data_val << cnt++;
			}
			
		/* Problem with data integrity bit --> solve later
		if(cnt == 9)
			if((vals % 2) == data_val) {
				Serial.print("Got: ");
        Serial.println(result);
			break;
			}
	  */ 

     if(cnt == 11 ) {
      /* the start condition and parity and stop are there, fix next line */
        result <<= 2;
        result >>= 3;
#if PRINT
        Serial.print("Got: ");
        Serial.println(result);
#endif

        golden_trigger = 1;
      }
      
	}

 delay(2); // Program too fast, need to check protocol more.

 return result;
}

uint8_t PS2Protocol::xfer(uint8_t message) {
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

void PS2Protocol::charrify_hex(uint8_t msg) {
  uint8_t match;

  for (match = 0; match < 45; match++){
    if (make_letters_and_numbers[match] == msg){
      Serial.print(letters_and_numbers[match]);
      //Serial.println(msg);
      break;
    }
  }
  //Serial.println(msg);
  //Serial.println(letters_and_numbers[msg]);
  
  
}

#if 0
uint8_t PS2Protocol::host_receive() {

}

uint8_t PS2Protocol::host_transfer(uint8_t message, uint8_t parity_bit) {

}
uint8_t PS2Protocol::calculate_parity(uint8_t message) {
	/* Even number of 1's -> parity bit 1, odd number -> 0 */
}
#endif
