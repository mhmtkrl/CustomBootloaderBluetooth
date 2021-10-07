#include <stdint.h>

#define XMODEM_SOH 0x01
#define XMODEM_STX 0x02
#define XMODEM_ETX 0x03
#define XMODEM_EOT 0x04
#define XMODEM_ACK 0x06
#define XMODEM_NAK 0x15
#define XMODEM_CAN 0x18
#define XMODEM_SUB 0x1A

__packed typedef struct {
	uint8_t pakcet_SOH;
	uint8_t packetNumber;
	uint8_t pakcetNumberComp;
	uint32_t packetData[32];
	uint16_t packetCRC;
}BootPacket;

BootPacket bootPacket[20];
