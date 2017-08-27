#ifndef LEDS_H__
#define LEDS_H__

#define LEDS_MOSI_PIN 10
#define LEDS_SCLK_PIN 11
#define LEDS_BITRATE 1000000 //1Mhz

/**
APA102 stuff, format: 0x(0b111LLLLL)BBGGRR
where L = Brightness
*/
#define APA102_BYTES_PER_LED 4
#define APA102_BRIGHTNESS_HEADER 0xE0
#define APA102_R_OFFSET 3
#define APA102_G_OFFSET 2
#define APA102_B_OFFSET 1

void leds_init();
void leds_clear();
void leds_deinit();

#endif  /* _ LEDS_H__ */
