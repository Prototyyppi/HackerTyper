#include "PS2Protocol.h"
#include "PS2CharSet.h"
#include "Arduino.h"
#define CLOCK_STEP 1
#define CLOCK_DATA 2

PS2Protocol::PS2Protocol(int8_t PS2_DATA_LINE, int8_t PS2_CLOCK_LINE) {
	_PS2_DATA_LINE = PS2_DATA_LINE;
	_PS2_CLOCK_LINE = PS2_CLOCK_LINE;
   pinMode(_PS2_DATA_LINE, INPUT);
   pinMode(_PS2_CLOCK_LINE, INPUT); 
}

PS2Protocol::PS2Protocol(int8_t PS2_DATA_LINE, int8_t PS2_CLOCK_LINE, int8_t HOST_PS2_DATA_LINE, int8_t HOST_PS2_CLOCK_LINE) {
  _PS2_DATA_LINE = PS2_DATA_LINE;
  _PS2_CLOCK_LINE = PS2_CLOCK_LINE;
   pinMode(_PS2_DATA_LINE, INPUT);
   pinMode(_PS2_CLOCK_LINE, INPUT); 

   _HOST_PS2_DATA_LINE = HOST_PS2_DATA_LINE;
   _PS2_CLOCK_LINE = HOST_PS2_CLOCK_LINE;
   pinMode(_HOST_PS2_DATA_LINE, INPUT);
   pinMode(_HOST_PS2_CLOCK_LINE, INPUT); 
   
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
			for (int i = 0; i < 50; i++)
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
   Serial.print("\n");
    */
	if (clk_val == 0 && data_val == 0){

		return 0;
	} else
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
	uint16_t clk_val, data_val = 0, p_clk_val = 1, p_data_val=0, trigger=0, cnt=0, result=0, parity = 0;
	while(wait_for_start_condition() != 0);
  /*
	p_clk_val = digitalRead(_PS2_DATA_LINE);
	p_data_val = digitalRead(_PS2_CLOCK_LINE);
   */
	while (!golden_trigger) {
		clk_val = digitalRead(_PS2_CLOCK_LINE);
    data_val = digitalRead(_PS2_DATA_LINE);
		if (clk_val == 0 && p_clk_val == 1)
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

     if(cnt == 12 ) {
        result >>= 1; // Remove start condition bit
        parity = (result >> 8) & 1; // Get parity bit
        result &= 0xFF; // Mask only the data bits
        ret = check_parity((uint8_t)result, (uint8_t)parity);
        if (ret != 0)
          Serial.print("Parity error"); // But dont care about it
#if PRINT
        Serial.print("Got: ");
        Serial.println(result);
#endif
        golden_trigger = 1;
      }
      
	}
 while(!digitalRead(_PS2_CLOCK_LINE)); // wait for clock to rise again


delay(3); // Skip data that we dont need, can be seen using following 
/*
 int k = 0;
while (k < 60) {
  //Serial.print(digitalRead(_PS2_CLOCK_LINE));
  Serial.print(digitalRead(_PS2_DATA_LINE));
  k++;
}
 */

 return result;
}

uint8_t PS2Protocol::send_bit(uint8_t bit_to_send) {

  digitalWrite(_HOST_PS2_DATA_LINE, bit_to_send);
  delay(CLOCK_STEP); // Get ready to publish bit
  digitalWrite(_HOST_PS2_CLOCK_LINE, LOW);
  delay(CLOCK_DATA); // Data is readable now
  digitalWrite(_HOST_PS2_CLOCK_LINE, HIGH);
  delay(CLOCK_STEP); // Reset clock high

}


uint8_t PS2Protocol::check_line_busy() {

  uint8_t clk_val, data_val;
   pinMode(_HOST_PS2_DATA_LINE, INPUT);
   pinMode(_HOST_PS2_CLOCK_LINE, INPUT); // Let them be inputs

  do {
    clk_val = digitalRead(_HOST_PS2_DATA_LINE);
    data_val = digitalRead(_HOST_PS2_CLOCK_LINE);
 
    if (clk_val == 0 && data_val == 1) {
     // host wants to send data
     while (!digitalRead(_HOST_PS2_CLOCK_LINE));
    
    generate_clock(); // Not interested in the data as of now. It may need to be received...
    
  }
  } while (clk_val != 1);
    Serial.print("Clear");
    delay(10);

    
  return 0;
}

uint8_t PS2Protocol::generate_clock() {
  
  pinMode(_HOST_PS2_CLOCK_LINE, OUTPUT); // Let them be outputs
  pinMode(_HOST_PS2_DATA_LINE, INPUT);
  for (int i = 0; i < 9; i++) {
  digitalWrite(_HOST_PS2_CLOCK_LINE, LOW);
  delay(1); // Get ready to publish bit;
  delay(1); // Data is readable now
  digitalWrite(_HOST_PS2_CLOCK_LINE, HIGH);
  delay(1); // Reset clock high
    Serial.print(digitalRead(_HOST_PS2_DATA_LINE));
  delay(1);
  
  }
  //Serial.print("Generated some clock");

  return 0;
}

uint8_t PS2Protocol::xfer(uint8_t message) {

	uint8_t parity = 0;

   pinMode(_HOST_PS2_DATA_LINE, OUTPUT);
  
	/* First generate start condition (clock goes low, and data low)*/
  send_bit(0);

	/* Then transmit data */
	for(int i = 0; i < 8; i++) {
		send_bit((message >> i) & 1);
  }
  send_bit(calculate_parity(message));
  send_bit(0); // End bit
  delay(10);
  send_bit(1); // Reset
}

/* Convert received hex number to char */
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

uint8_t PS2Protocol::check_parity(uint8_t result, uint8_t parity) {
  /* Even number of 1's -> parity bit 1, odd number -> 0 */
  uint8_t calculated_parity = 0;
  for (int i = 0; i < 8; i++) {
    calculated_parity += (result >> i) & 1;
  }
  if ( !(calculated_parity & 1) != parity)
    return 1;
  return 0;
}

uint8_t PS2Protocol::calculate_parity(uint8_t message) {
  /* Even number of 1's -> parity bit 1, odd number -> 0 */
  uint8_t calculated_parity = 0;
  for (int i = 0; i < 8; i++) {
    calculated_parity += (message >> i) & 1;
  }
  return !(calculated_parity & 1);
}



#if 0
uint8_t PS2Protocol::host_receive() {
 
}

uint8_t PS2Protocol::host_transfer(uint8_t message, uint8_t parity_bit) {

}

#endif
