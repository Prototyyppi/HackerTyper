#ifndef PS2Protocolheader
#define PS2Protocolheader
#include <cinttypes>

class PS2Protocol
{
	public:
		PS2Protocol(int8_t PS2_DATA_LINE, int8_t PS2_CLOCK_LINE);
		uint8_t receive();
		uint8_t transfer(uint8_t message, uint8_t parity_bit);
		uint8_t calculate_parity(uint8_t message);
		uint8_t wait_for_start_condition();
	private:
		int8_t _PS2_DATA_LINE;
		int8_t _PS2_CLOCK_LINE;
};

#endif