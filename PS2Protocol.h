#ifndef PS2Protocolheader
#define PS2Protocolheader

#include <stdint.h>

class PS2Protocol
{
	public:
		PS2Protocol(int8_t PS2_DATA_LINE, int8_t PS2_CLOCK_LINE);
		uint8_t receive();
		uint8_t xfer(uint8_t message);
		uint8_t host_receive();
		uint8_t host_transfer(uint8_t message);
		uint8_t calculate_parity(uint8_t message); // not needed
		uint8_t wait_for_start_condition();
		uint8_t poll();
	private:
		int8_t _PS2_DATA_LINE;
		int8_t _PS2_CLOCK_LINE;
		int8_t _HOST_PS2_DATA_LINE;
		int8_t _HOST_PS2_CLOCK_LINE;
};

#endif
