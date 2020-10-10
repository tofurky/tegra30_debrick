// based on ipatch_rcm_sample.c provided by ktemkin (https://gist.github.com/ktemkin/825d5f4316f63a7c11ea851a2022415a)
// unmodified copy of original source can also be found at https://github.com/tofurky/tegra30_debrick/payload/ipatch_rcm_sample.c
// ipatch_word(), unipatch_word(), dump_word(), and dump_byte() are more or less unmodified.
// clock/uart initialization and offsets have been consolidated and modified for tegra30

// begin original header
/**
 * Proof of Concept Payload
 * prints out SBK and then enables non-secure RCM 
 *
 * (some code based on coreboot)
 * ~ktemkin
 */
// end original header

#include <stdint.h>

// nexus 7
//#define UARTA

// ouya
//#define UARTD


// ODM_DEBRICK will heavy-handedly disable the MMC controller via IPATCH.
// if bad odmdata is flashed to the BCT and the board becomes unresponsive to nvflash, this can bring it back
//#define ODM_DEBRICK

#define _REG(base, off) *(volatile unsigned int *)((base) + (off))
#define reg_write(base, off, value) _REG(base, off) = value
#define reg_clear(base, off, value) _REG(base, off) &= ~value
#define reg_set(base, off, value) _REG(base, off) |= value
#define reg_read(base, off) _REG(base, off)

#define BOOTROM_START_POST_IPATCH	0xfff01004
#define DESIRED_SECURITY_MODE		3
#define IPATCH_SLOT					1
#define IROM_PATCH_ADDRESS			0xfff01CD4
#define PMC_SCRATCH0				(0x50)
#define PMC_SCRATCH0_WARMBOOT		(1 << 0)
/* ipatch hardware */
#define IPATCH_BASE					(0x6001dc00)
#define IPATCH_SELECT				(0x0)
#define IPATCH_REGS					(0x4)
#define APBDEV_PMC_RST_STATUS_0		(0x1b4)

#define HEX_CHAR(x) ((((x) + '0') > '9') ? ((x) + '7') : ((x) + '0'))

#define PMC_BASE					(0x7000e400)
#define APBDEV_PMC_SCRATCH42_0		(0x144)
#define MAGIC_VALUE					(0x42)
#define APBDEV_PMC_IO_DPD_REQ_0		(0x1b8)
#define UART_DPD_BIT				(1 << 14)

#define PINMUX_BASE					(0x70003000)
/* uart-a tx */
#define PINMUX_AUX_ULPI_DATA0_0		(0x00)
/* uart-a rx */
#define PINMUX_AUX_ULPI_DATA1_0		(0x04)
/* uart-d tx */
#define PINMUX_AUX_ULPI_CLK_0		(0x20)
/* uart-d rx */
#define PINMUX_AUX_ULPI_DIR_0		(0x24)

#define CAR_BASE					(0x60006000)
#define CLK_SOURCE_PLLP				(0x0)

#define UART_THR_DLAB				(0x00)
#define UART_IER_DLAB				(0x04)
#define UART_IIR_FCR				(0x08)
#define UART_LCR					(0x0c)
#define UART_LSR					(0x14)

/* aka 408000000/115200/29 - works for IROM, assuming 408000000 PLLP */
#define UART_RATE_115200			(122)
/* clear TX FIFO */
#define FCR_TX_CLR					(1 << 2)
/* clear RX FIFO */
#define FCR_RX_CLR					(1 << 1)
/* enable TX & RX FIFO */
#define FCR_EN_FIFO					(1 << 0)
/* Divisor Latch Access Bit */
#define LCR_DLAB					(1 << 7)
/* word length of 8 */
#define LCR_WD_SIZE_8				(0x3)

// uart a
#define UARTA_BASE					(0x70006000)
#define CLK_RST_CONTROLLER_CLK_OUT_ENB_L_0 (0x10)
#define CLK_RST_CONTROLLER_RST_DEVICES_L_0 (0x04)
#define CLK_RST_CONTROLLER_CLK_SOURCE_UARTA_0 (0x178)
#define UARTA_CAR_MASK (1 << 6)
// uart d
#define UARTD_BASE					(0x70006300)
#define CLK_RST_CONTROLLER_CLK_OUT_ENB_U_0 (0x18)
#define CLK_RST_CONTROLLER_RST_DEVICES_U_0 (0x0c)
#define CLK_RST_CONTROLLER_CLK_SOURCE_UARTD_0 (0x1c0)
#define UARTD_CAR_MASK (1 << 1)

#if defined UARTA
#define UART_BASE UARTA_BASE
#define CLK_RST_CONTROLLER_CLK_OUT_ENB CLK_RST_CONTROLLER_CLK_OUT_ENB_L_0
#define CLK_RST_CONTROLLER_RST_DEVICES CLK_RST_CONTROLLER_RST_DEVICES_L_0
#define CLK_RST_CONTROLLER_CLK_SOURCE_UART CLK_RST_CONTROLLER_CLK_SOURCE_UARTA_0
#define UART_CAR_MASK UARTA_CAR_MASK

#elif defined UARTD
#define UART_BASE UARTD_BASE
#define CLK_RST_CONTROLLER_CLK_OUT_ENB CLK_RST_CONTROLLER_CLK_OUT_ENB_U_0
#define CLK_RST_CONTROLLER_RST_DEVICES CLK_RST_CONTROLLER_RST_DEVICES_U_0
#define CLK_RST_CONTROLLER_CLK_SOURCE_UART CLK_RST_CONTROLLER_CLK_SOURCE_UARTD_0
#define UART_CAR_MASK UARTD_CAR_MASK

#else
#error "one of UARTA or UARTD must be defined"
#endif

// General next-stage image entry point type.
typedef void (*entry_point)(void);

void uart_print(const char *string);
void ipatch_word(uint8_t slot, uint32_t addr, uint16_t new_value);
void unipatch_word(uint8_t slot);
static void dump_byte(uint8_t b);
static void dump_word(uint32_t w);
static void dump_dword(uint32_t d);

void main()
{
	entry_point start;

	reg_write(PMC_BASE, APBDEV_PMC_SCRATCH42_0, 0);
	uart_print("----------------------------------------------------------------------------\n");
	uart_print("APBDEV_PMC_RST_STATUS_0: ");
	dump_dword(__builtin_bswap32(reg_read(PMC_BASE, APBDEV_PMC_RST_STATUS_0)));
	uart_print("\nBIT_BootType: ");
	dump_dword(__builtin_bswap32(reg_read(0x0, 0x4000000c)));
	uart_print("\n");

#ifdef ODM_DEBRICK
	uart_print("disabling emmc in IROM for odm debrick...\n");
	/* corrupt the mmc controller references in IROM so that a bad BCT / odmdata doesn't prevent nvflash from working */
	ipatch_word(2, 0xfff0926c, 0x0200); /* odmdata debrick: change reference from 78000600 -> 78000200 */
	ipatch_word(3, 0xfff09270, 0x0000); /* odmdata debrick: change reference from 78000400 -> 78000000 */
#endif

	/* Patch the getSecurityMode function to always return 3 (production non-secure). */
	uart_print("overriding getSecurityMode function to always return 3 (production non-secure)...\n");
	ipatch_word(IPATCH_SLOT, IROM_PATCH_ADDRESS, 0x2000 | DESIRED_SECURITY_MODE);

	/* Clear bit0 to indicate that this is a fresh boot, and then set bit2 to trigger RCM. */
	uart_print("writing PMC_SCRATCH0 to trigger RCM mode after soft reset...\n");
	reg_write(PMC_BASE, PMC_SCRATCH0, (1 << 1)); // wrong 2));

	uart_print("jumping to 0xfff01004...\n");
	reg_write(PMC_BASE, APBDEV_PMC_SCRATCH42_0, 0);
	/* Jump back into the bootloader immediately after ipatches are applied
	   to simulate a normal coldboot as best we can. :) */
	start = (entry_point)BOOTROM_START_POST_IPATCH;
	start();
}

void uart_print(const char *string) {
	/* use this to keep track of if uart has been initialized */
	if(reg_read(PMC_BASE, APBDEV_PMC_SCRATCH42_0) != MAGIC_VALUE) {
#if defined UARTA
		/* set pinmux for uart-a (nexus 7) */
		reg_write(PINMUX_BASE, PINMUX_AUX_ULPI_DATA0_0, 0b00000110); /* tx */
		reg_write(PINMUX_BASE, PINMUX_AUX_ULPI_DATA1_0, 0b00100110); /* rx */
#elif defined UARTD
		/* set pinmux for uart-d (ouya */
		reg_write(PINMUX_BASE, PINMUX_AUX_ULPI_CLK_0, 0b00000110); /* tx */
		reg_write(PINMUX_BASE, PINMUX_AUX_ULPI_DIR_0, 0b00100110); /* rx */
#endif

		/* clear deep power down for all uarts */
		reg_clear(PMC_BASE, APBDEV_PMC_IO_DPD_REQ_0, UART_DPD_BIT);

		/* enable clock for uart-N */
		reg_set(CAR_BASE, CLK_RST_CONTROLLER_CLK_OUT_ENB, UART_CAR_MASK);

		/* assert and deassert reset for uart-N */
		reg_set(CAR_BASE, CLK_RST_CONTROLLER_RST_DEVICES, UART_CAR_MASK);
		reg_clear(CAR_BASE, CLK_RST_CONTROLLER_RST_DEVICES, UART_CAR_MASK);

		/* set clock source to pllp for uart-N */
		reg_write(CAR_BASE, CLK_RST_CONTROLLER_CLK_SOURCE_UART, CLK_SOURCE_PLLP);

		/* set DLAB bit to enable programming of DLH/DLM registers */
		reg_set(UART_BASE, UART_LCR, LCR_DLAB);

		/* write lower 8 (DLH) and upper 8 (DLM) bits of 16 bit baud divisor */
		reg_write(UART_BASE, UART_THR_DLAB, (UART_RATE_115200 & 0xff));
		reg_write(UART_BASE, UART_IER_DLAB, (UART_RATE_115200 >> 8));

		/* clear DLAB bit to disable setting of baud divisor */
		reg_clear(UART_BASE, UART_LCR, LCR_DLAB);

		/* 8-bit word size - defaults are no parity and 1 stop bit */
		reg_write(UART_BASE, UART_LCR, LCR_WD_SIZE_8);

		/* enable tx/rx fifos */
		reg_write(UART_BASE, UART_IIR_FCR, FCR_EN_FIFO);

		/* prevent this uart-N initialization from being done on subsequent calls to uart_print() */
		reg_write(PMC_BASE, APBDEV_PMC_SCRATCH42_0, MAGIC_VALUE);
	}

	/* send all characters until NULL to uart-N */
	while(*string) {
		/* put the char into the tx fifo */
		reg_write(UART_BASE, UART_THR_DLAB, (char) *string);

		/* wait for tx fifo to clear */
		while(!((reg_read(UART_BASE, UART_LSR) >> 5) & 0x01));

		/* move on to next char */
		++string;
	}
}

/**
 * Patches over a given address in the IROM using the IPATCH hardware.
 */
void ipatch_word(uint8_t slot, uint32_t addr, uint16_t new_value)
{
	uint32_t slot_value;
	uint32_t offset;

	// Mark the relevant ipatch slot as not-in-use.
	reg_clear(IPATCH_BASE, IPATCH_SELECT, (1 << slot));

	// Compute the new patch value.
	offset = (addr & 0xFFFF) >> 1;
	slot_value = (offset << 16) | new_value;

	// Figure out the location of the slot to touch.
	reg_write(IPATCH_BASE, IPATCH_REGS + (slot * 4), slot_value);

	// Apply the new one.
	reg_set(IPATCH_BASE, IPATCH_SELECT, (1 << slot));
}

/**
 * Disables a given ipatch.
 */
void unipatch_word(uint8_t slot)
{
	// Mark the relevant ipatch slot as not-in-use.
	reg_clear(IPATCH_BASE, IPATCH_SELECT, (1 << slot));
}

/** send an 8 bit byte as two hex characters to the console */
static void dump_byte(uint8_t b)
{
	char str[2];
	str[0] = HEX_CHAR((b >> 4) & 0xf);
	str[1] = HEX_CHAR(b & 0xf);

	uart_print(str);
}

/** prints a word */
static void dump_word(uint32_t w)
{
	dump_byte(w & 0xff);
	dump_byte(w >> 8);
}

/** print a dword */
static void dump_dword(uint32_t d)
{
	dump_word(d & 0xffff);
	dump_word(d >> 16);
}
