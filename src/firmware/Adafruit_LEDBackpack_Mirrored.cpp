#include "Adafruit_LEDBackpack_Mirrored.h"

static const uint8_t numbertable_mirrored[] = {
    0x3F, /* 0 */
    0b0000000000110000, /* 1 BE */
    0b0000000001101101, /* 2 ABDFG */
    0b0000000001111001, /* 3 ABDEG */
    0b0000000001110010, /* 4 BDCE */
    0x5B, /* 5 */
    0b0000000001011111, /* 6 ACDEFG */
    0b0000000000110001, /* 7 ABE */
    0x7F, /* 8 */
    0b0000000001111011, /* 9 ABCDE */
};

void Adafruit_7segment_Mirrored::writeDigitNumMirrored(uint8_t d, uint8_t num) {
  displaybuffer[d] = numbertable_mirrored[num];
}

void Adafruit_7segment_Mirrored::turnOffDigit(uint8_t d) {
  displaybuffer[d] = 0b0000000000000000;
}
