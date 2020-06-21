// uart_print stub, based on uart printing functionality of ktemkin's ipatch_rcm_sample.c
// prints a null terminated string passed to it over UARTA on tegra30. pinmux based on n7.

#include <stdint.h>

#define _REG(base, off) *(volatile unsigned int *)((base) + (off))
#define reg_write(base, off, value) _REG(base, off) = value
#define reg_clear(base, off, value) _REG(base, off) &= ~value
#define reg_set(base, off, value) _REG(base, off) |= value
#define reg_read(base, off) _REG(base, off)

#define PMC_BASE (0x7000E400)
#define APBDEV_PMC_SCRATCH42_0 (0x144)
#define MAGIC_VALUE (0x42)
#define APBDEV_PMC_IO_DPD_REQ_0 (0x1b8)
#define UART_DPD_BIT (1 << 14)

#define PINMUX_BASE (0x70003000)
/* uart-a tx */
#define PINMUX_AUX_ULPI_DATA0_0 (0x00)
/* uart-a rx */
#define PINMUX_AUX_ULPI_DATA1_0 (0x04)

#define CAR_BASE (0x60006000)
#define CLK_RST_CONTROLLER_CLK_OUT_ENB_L_0 (0x10)
#define CLK_RST_CONTROLLER_RST_DEVICES_L_0 (0x04)
#define CLK_RST_CONTROLLER_CLK_SOURCE_UARTA_0 (0x178)
#define CLK_SOURCE_PLLP (0x0)
#define UARTA_CAR_MASK (1 << 6)

#define UARTA_BASE (0x70006000)

#define UART_THR_DLAB (0x00)
#define UART_IER_DLAB (0x04)
#define UART_IIR_FCR (0x08)
#define UART_LCR (0x0c)
#define UART_LSR (0x14)

/* aka 408000000/115200/29 - works for IROM, assuming 408000000 PLLP  */
//#define UART_RATE_115200 (122) // ~215000
//#define UART_RATE_115200 (240) // ~105000
//#define UART_RATE_115200 (238) // ~110000
//#define UART_RATE_115200 (236) // ~110000
//#define UART_RATE_115200 (234) // ~110000
//#define UART_RATE_115200 (232)
#define UART_RATE_115200 (231)
/* clear TX FIFO */
#define FCR_TX_CLR (1 << 2)
/* clear RX FIFO */
#define FCR_RX_CLR (1 << 1)
/* enable TX & RX FIFO */
#define FCR_EN_FIFO (1 << 0)
/* Divisor Latch Access Bit */
#define LCR_DLAB (1 << 7)
/* word length of 8 */
#define LCR_WD_SIZE_8 (0x3)

void uart_print(const char *string) {
	/* use this to keep track of if uart-a has been initialized */
    if(reg_read(PMC_BASE, APBDEV_PMC_SCRATCH42_0) != MAGIC_VALUE) {
		/* set pinmux for uart-a */
		reg_write(PINMUX_BASE, PINMUX_AUX_ULPI_DATA0_0, 0b00000110); /* tx */
		reg_write(PINMUX_BASE, PINMUX_AUX_ULPI_DATA1_0, 0b00100110); /* rx */

		/* clear deep power down for all uarts */
		reg_clear(PMC_BASE, APBDEV_PMC_IO_DPD_REQ_0, UART_DPD_BIT);

		/* enable clock for uart-a */
		reg_set(CAR_BASE, CLK_RST_CONTROLLER_CLK_OUT_ENB_L_0, UARTA_CAR_MASK);

		/* assert and deassert reset for uart-a */
		reg_set(CAR_BASE, CLK_RST_CONTROLLER_RST_DEVICES_L_0, UARTA_CAR_MASK);
		reg_clear(CAR_BASE, CLK_RST_CONTROLLER_RST_DEVICES_L_0, UARTA_CAR_MASK);

		/* set clock source to pllp for uart-a */
		reg_write(CAR_BASE, CLK_RST_CONTROLLER_CLK_SOURCE_UARTA_0, CLK_SOURCE_PLLP);

		/* set DLAB bit to enable programming of DLH/DLM registers */
		reg_set(UARTA_BASE, UART_LCR, LCR_DLAB);

		/* write lower 8 (DLH) and upper 8 (DLM) bits of 16 bit baud divisor */
		reg_write(UARTA_BASE, UART_THR_DLAB, (UART_RATE_115200 & 0xff));
		reg_write(UARTA_BASE, UART_IER_DLAB, (UART_RATE_115200 >> 8));

		/* clear DLAB bit to disable setting of baud divisor */
		reg_clear(UARTA_BASE, UART_LCR, LCR_DLAB);

		/* 8-bit word size - defaults are no parity and 1 stop bit */
		reg_write(UARTA_BASE, UART_LCR, LCR_WD_SIZE_8);

		/* enable tx/rx fifos */
		reg_write(UARTA_BASE, UART_IIR_FCR, FCR_EN_FIFO);

		/* prevent this uart-a initialization from being done on subsequent calls to uart_print() */
		reg_write(PMC_BASE, APBDEV_PMC_SCRATCH42_0, MAGIC_VALUE);
	}

	/* send all characters until NULL to uart-a */
	while(*string) {
		/* put the char into the tx fifo */
		reg_write(UARTA_BASE, UART_THR_DLAB, (char) *string);

		/* wait for tx fifo to clear */
		while(!((reg_read(UARTA_BASE, UART_LSR) >> 5) & 0x01));

		/* move on to next char */
		++string;
	}
}
