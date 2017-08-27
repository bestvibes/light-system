#include <stdio.h>
#include <stdint.h>

#include "led_spi.h"
#include "spi.h"

static spi_config peripheral = {};

inline static void set_led(uint8_t *data, uint8_t brightness, uint8_t r, uint8_t g, uint8_t b);
inline static void set_brightness(uint8_t *data, uint8_t brightness);

void leds_init() {
	spi_init(&peripheral, LEDS_SCLK_PIN, LEDS_MOSI_PIN, LEDS_BITRATE);
	printf("LEDs initialized\n");
}

void leds_clear(int nLEDs) {
	int data_size = APA102_BYTES_PER_LED * nLEDs;
	uint8_t *data = calloc(data_size); // init the data to all
	for (int i = 0; i < data_size; i += APA102_BYTES_PER_LED) {
		set_brightness(data + i, 0x1F); // 0xFF000000
	}

	spi_write(&peripheral, data, data_size);
}

inline static void set_led(uint8_t *data, uint8_t brightness, uint8_t r, uint8_t g, uint8_t b) {
	set_brightness(data, brightness);
	data[APA102_R_OFFSET] = r;
	data[APA102_G_OFFSET] = g;
	data[APA102_B_OFFSET] = b;
}

inline static void set_brightness(uint8_t *data, uint8_t brightness) {
	// for APA102 only 5 bits are for brightness, if > 5 given, will be truncated to 5
	data[0] = APA102_BRIGHTNESS_HEADER | brightness;
}