

#ifndef _T210_H_
#define _T210_H_

// FIXME: make these enums, like the ones below

/* Microseoncd timer base address. */
#define TIMER_BASE              (0x60005000)
#define TEGRA_TMRUS             (0x60005010UL)

/* Power management controller base address and offsets. */
#define PMC_BASE                (0x7000e400UL)
#define PMC_CONTROL             (0x0)
#define PMC_CONTROL_MAIN_RESET  (1 << 4)
#define PMC_SCRATCH0            (0x50)
#define PMC_SCRATCH0_WARMBOOT   (1 << 0)
#define PMC_SCRATCH31           (0x118)
#define PMC_SCRATCH32           (0x11c)

/* Clock and reset controller */
#define CAR_BASE                (0x60006000)

/* Pinmux in APB misc */
#define PINMUX_BASE             (0x70003000)

/* UARTA */
#define UARTA_BASE              (0x70006000)
#define UARTA_CAR_MASK          (1 << 6)

/* Fuses */
#define FUSE_BASE               (0x7000f800)
#define FUSE_REG_ADDR           (0x04)
#define FUSE_REG_READ           (0x08)
#define FUSE_CACHE_SBK_BASE     (0x7000F9A4)

/* APB misc block */
#define APB_MISC_BASE           (0x70000000)
#define APB_MISC_STRAPS         (0x8)

/* ipatch hardware */
#define IPATCH_BASE             (0x6001dc00)
#define IPATCH_SELECT           (0x0)
#define IPATCH_REGS             (0x4)



/**
 * from coreboot
 */
enum {
  PMC_XOFS_SHIFT = 1,
  PMC_XOFS_MASK = 0x3f << PMC_XOFS_SHIFT
};

enum {
  OSC_XOE = 0x1 << 0,
  OSC_XOFS_SHIFT = 4,
  OSC_XOFS_MASK = 0x3f << OSC_XOFS_SHIFT,
  OSC_FREQ_SHIFT = 28,
  OSC_FREQ_MASK = 0xf << OSC_FREQ_SHIFT
};

enum {
    CLK_M_DIVISOR_MASK = 0x3 << 2,
    CLK_M_DIVISOR_BY_2 = 0x1 << 2
};

enum {
  UART_THR_DLAB = 0x00,
  UART_IER_DLAB = 0x04,
  UART_IIR_FCR  = 0x08,
  UART_LCR      = 0x0C,
  UART_LSR      = 0x14
};

enum {
  UART_RATE_115200 = (408000000/115200/16), /* based on 408000000 PLLP */
  FCR_TX_CLR = 0x4, /* bit 2 of FCR : clear TX FIFO */
  FCR_RX_CLR = 0x2, /* bit 1 of FCR : clear RX FIFO */
  FCR_EN_FIFO = 0x1,  /* bit 0 of FCR : enable TX & RX FIFO */
  LCR_DLAB = 0x80,  /* bit 7 of LCR : Divisor Latch Access Bit */
  LCR_WD_SIZE_8 = 0x3,  /* bit 1:0 of LCR : word length of 8 */
};

enum {
    CLK_SRC_PLLP_OUT0 = (0x0 << 29),
    MSELECT_CLK_DIVISOR_4 = 6
};
enum {
    CLK_ENB_MSELECT = 0x1 << 3
};
enum {
    MSELECT_RST = 0x1 << 3
};

enum {
    SUPER_CDIV_ENB = 0x1 << 31
};


#endif