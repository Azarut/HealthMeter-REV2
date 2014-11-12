#define CRC16_INITIAL_REMAINDER 0x0000
#define CRC16_FINAL_XOR_VALUE   0x0000
#define CRC16_WIDTH (8 * sizeof(uint16_t))
	
extern const uint16_t crcTable[256];
extern uint16_t crc16(const void * const message, const uint16_t nBytes) ;





