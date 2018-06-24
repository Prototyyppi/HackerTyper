#ifndef PS2Protocolheader
#define PS2Protocolheader
#define DEBUG 0
#define PRINT 0

#include <stdint.h>

class PS2Protocol
{
	public:
		PS2Protocol(int8_t PS2_DATA_LINE, int8_t PS2_CLOCK_LINE);
    PS2Protocol(int8_t PS2_DATA_LINE, int8_t PS2_CLOCK_LINE, int8_t HOST_PS2_DATA_LINE, int8_t HOST_PS2_CLOCK_LINE);
		uint8_t receive();
		uint8_t xfer(uint8_t message);
		uint8_t host_receive();
		uint8_t host_transfer(uint8_t message);
		uint8_t check_parity(uint8_t result, uint8_t parity);
    uint8_t calculate_parity(uint8_t message);
    uint8_t check_line_busy();
    uint8_t generate_clock();
		uint8_t wait_for_start_condition();
		uint8_t poll();
    void charrify_hex(uint8_t msg); // Must be streamlined
    bool is_release_stroke(uint16_t msg);
    void debug();
	private:
		int8_t _PS2_DATA_LINE;
		int8_t _PS2_CLOCK_LINE;
		int8_t _HOST_PS2_DATA_LINE;
		int8_t _HOST_PS2_CLOCK_LINE;
    uint8_t send_bit(uint8_t bit_to_send);
};

#endif
