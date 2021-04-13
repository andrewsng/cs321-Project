#ifndef VGACONST_H
#define VGACONST_H

#include <cstdint>


extern unsigned int SCREEN_WIDTH;
extern unsigned int SCREEN_HEIGHT;

const unsigned int X_WIDTH_BYTES   = 80;
const uint16_t     SC_INDEX        = 0x03C4;
const uint16_t     INPUT_STATUS_1  = 0x03DA;
const uint16_t     CRTC_INDEX      = 0x03D4;
const uint16_t     CRTC_DATA       = 0x03D5;
const uint16_t     MISC_INDEX      = 0x03C2;
const uint16_t     SCREEN_SEG      = 0xA000;
const unsigned int SCREEN_ADR      = 0xA0000;
const uint8_t      MAP_MASK        = 0x02;
const uint8_t      START_ADDR_HIGH = 0x0C;
const uint8_t      START_ADDR_LOW  = 0x0D;


#endif  // VGACONST_H