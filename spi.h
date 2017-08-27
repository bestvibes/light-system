#ifndef SPI_H__
#define SPI_H__

#define SPI_PERIPHERAL "/dev/spidev0.0"

#define RPI_SPI_MOSI_PIN 10
#define RPI_SPI_SCLK_PIN  11

#define SPI_BITS_PER_WORD  8

// NOTE
//  cdiv    speed
//     2    125.0 MHz
//     4     62.5 MHz
//     8     31.2 MHz
//    16     15.6 MHz
//    32      7.8 MHz
//    64      3.9 MHz
//   128     1953 kHz
//   256      976 kHz
//   512      488 kHz
//  1024      244 kHz
//  2048      122 kHz
//  4096       61 kHz
//  8192     30.5 kHz
// 16384     15.2 kHz
// 32768     7629 Hz
// Speed rounds to nearest one on Raspberry Pi

typedef struct {
	uint8_t sclk_pin;
	uint8_t mosi_pin;
	int bitrate;
	int fd;
} spi_config;

void spi_open(spi_config *peripheral, uint8_t sclk_pin, uint8_t mosi_pin, int bitrate);
void spi_write(spi_config *peripheral, uint8_t *data, int len);
void spi_close(spi_config *peripheral);

#endif  /* _ SPI_H__ */
