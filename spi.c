#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "spi.h"

// SPI transfer configs
static struct spi_ioc_transfer xfer[3] = {
 { .tx_buf        = 0, // Header (zeros)
   .rx_buf        = 0,
   .len           = 4,
   .delay_usecs   = 0,
   .bits_per_word = 8,
   .cs_change     = 0 },
 { .rx_buf        = 0, // Color payload
   .delay_usecs   = 0,
   .bits_per_word = 8,
   .cs_change     = 0 },
 { .tx_buf        = 0, // Footer (zeros)
   .rx_buf        = 0,
   .delay_usecs   = 0,
   .bits_per_word = 8,
   .cs_change     = 0 }
};

static void _spi_error(spi_config *peripheral) {
	spi_close(peripheral);

	// more logging stuff later here

	exit(1);
}

void spi_close(spi_config *peripheral) {
    if (peripheral->fd < 0)
        return;

    // to prevent infinite loops if close errors out
    int fd = peripheral->fd;
    peripheral->fd = -1;

    if (close(fd) < 0) {
    	printf("close: can't close SPI\n");
		_spi_error(peripheral);
    }

    return;
}

void spi_init(spi_config *peripheral, uint8_t sclk_pin, uint8_t mosi_pin, int bitrate) {
	// validate input
	if (sclk_pin != RPI_SPI_SCLK_PIN || mosi_pin != RPI_SPI_MOSI_PIN) {
		printf("Supports only RPI HW SPI! Set proper SCLK and MOSI pins!\n");
		exit(1);
	}

	if (bitrate <= 0) {
		printf("Invalid bitrate!\n");
		exit(1);
	}

	memset(peripheral, 0, sizeof(spi_config));
	peripheral->sclk_pin = sclk_pin;
	peripheral->mosi_pin = mosi_pin;
	peripheral->bitrate = bitrate;

	// TODO: try write only since this is for LEDs?

	// init SPI
	if ((peripheral->fd = open(SPI_PERIPHERAL, O_RDWR)) < 0) {
		printf("open: can't open %s (try 'sudo')\n", SPI_PERIPHERAL);
		_spi_error(peripheral);
	}

	// set mode
	uint8_t mode = SPI_MODE_0 | SPI_NO_CS;
	if (ioctl(peripheral->fd, SPI_IOC_WR_MODE, &mode) < 0) {
		printf("ioctl: can't set SPI mode to %d\n", mode);
		_spi_error(peripheral);
	}

	// set speed
	if (ioctl(peripheral->fd, SPI_IOC_WR_MAX_SPEED_HZ, peripheral->bitrate) < 0) {
		printf("ioctl: can't set SPI bitrate to %d\n", peripheral->bitrate);
		close(peripheral->fd);
		exit(1);
	}

	printf("SPI initialized at %dHz\n", peripheral->bitrate);
}

// TODO: this is APA102 LED specific, here for efficiency reasons for now...
void spi_write(spi_config *peripheral, uint8_t *data, int len) {
	xfer[0].speed_hz = peripheral->bitrate;
	xfer[1].speed_hz = peripheral->bitrate;
	xfer[2].speed_hz = peripheral->bitrate;

	xfer[1].tx_buf   = (unsigned long) data;
	xfer[1].len      = len;

	// footer length is 32 bits for every 64 LEDs (relying heavily on truncation here)
	xfer[2].len = ((len / 4) + 63) / 64 * 4 /* bytes per end frame */;
	
	// send the header, data & footer in one operation
	if (ioctl(peripheral->fd, SPI_IOC_MESSAGE(3), xfer) < 1) {
		printf("ioctl: error sending data");
		_spi_error(peripheral);
	}
}