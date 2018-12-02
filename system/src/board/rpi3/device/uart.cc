// akal/kernel
#include "akal/kernel/mmio.hh"
// akal/board
#include "akal/board/rpi3/mailbox.hh"
#include "akal/board/rpi3/device/uart.hh"

#define GPFSEL0         (MMIO_BASE + 0x00200000)
#define GPFSEL1         (MMIO_BASE + 0x00200004)
#define GPFSEL2         (MMIO_BASE + 0x00200008)
#define GPFSEL3         (MMIO_BASE + 0x0020000c)
#define GPFSEL4         (MMIO_BASE + 0x00200010)
#define GPFSEL5         (MMIO_BASE + 0x00200014)
#define GPSET0          (MMIO_BASE + 0x0020001c)
#define GPSET1          (MMIO_BASE + 0x00200020)
#define GPCLR0          (MMIO_BASE + 0x00200028)
#define GPLEV0          (MMIO_BASE + 0x00200034)
#define GPLEV1          (MMIO_BASE + 0x00200038)
#define GPEDS0          (MMIO_BASE + 0x00200040)
#define GPEDS1          (MMIO_BASE + 0x00200044)
#define GPHEN0          (MMIO_BASE + 0x00200064)
#define GPHEN1          (MMIO_BASE + 0x00200068)
#define GPPUD           (MMIO_BASE + 0x00200094)
#define GPPUDCLK0       (MMIO_BASE + 0x00200098)
#define GPPUDCLK1       (MMIO_BASE + 0x0020009c)
#define AUX_ENABLE      (MMIO_BASE + 0x00215004)
#define AUX_MU_IO       (MMIO_BASE + 0x00215040)
#define AUX_MU_IER      (MMIO_BASE + 0x00215044)
#define AUX_MU_IIR      (MMIO_BASE + 0x00215048)
#define AUX_MU_LCR      (MMIO_BASE + 0x0021504c)
#define AUX_MU_MCR      (MMIO_BASE + 0x00215050)
#define AUX_MU_LSR      (MMIO_BASE + 0x00215054)
#define AUX_MU_MSR      (MMIO_BASE + 0x00215058)
#define AUX_MU_SCRATCH  (MMIO_BASE + 0x0021505c)
#define AUX_MU_CNTL     (MMIO_BASE + 0x00215060)
#define AUX_MU_STAT     (MMIO_BASE + 0x00215064)
#define AUX_MU_BAUD     (MMIO_BASE + 0x00215068)

/* PL011 UART registers */
#define UART0_DR        (MMIO_BASE + 0x00201000)
#define UART0_FR        (MMIO_BASE + 0x00201018)
#define UART0_IBRD      (MMIO_BASE + 0x00201024)
#define UART0_FBRD      (MMIO_BASE + 0x00201028)
#define UART0_LCRH      (MMIO_BASE + 0x0020102C)
#define UART0_CR        (MMIO_BASE + 0x00201030)
#define UART0_IMSC      (MMIO_BASE + 0x00201038)
#define UART0_ICR       (MMIO_BASE + 0x00201044)

namespace akal {

    //*-- Uart1Device
    Uart1Device::Uart1Device() {
        // nothing go here        
    }

    Uart1Device::~Uart1Device() {
        // nothing go here
    }

    void Uart1Device::init() {
        u32 r;
        write32(AUX_ENABLE, 0x1); // TODO: this may disable other stuff.
        write32(AUX_MU_IER, 0x0);
        write32(AUX_MU_CNTL, 0x0);
        write32(AUX_MU_LCR, 0x3);
        write32(AUX_MU_MCR, 0x0);
        write32(AUX_MU_IER, 0x0);
        write32(AUX_MU_IIR, 0xc6);
        write32(AUX_MU_BAUD, 270);
        r = read32(GPFSEL1);
        r &= ~((7 << 12) | (7 << 15));
        r |= (2 << 12) | (2 << 15);
        write32(GPFSEL1, r);
        write32(GPPUD, 0);
        r = 150;
        while(r--) {
            asm volatile("nop");
        }
        write32(GPPUDCLK0, (1 << 14) | (1 << 15));
        r = 150;
        while(r--) {
            asm volatile("nop");
        }
        write32(GPPUDCLK0, 0x0);
        write32(AUX_MU_CNTL, 0x3);
    }

    void Uart1Device::write(i32 c) {
        do {
            asm volatile("nop");
        } while(!(read32(AUX_MU_LSR) & 0x20));
        write32(AUX_MU_IO, c);
    }

    void Uart1Device::write(const char *str) {
        while(*str) {
            this->write(*str++);
        }
    }

    //*-- Uart0Device
    Uart0Device::Uart0Device() {
        // nothing go here        
    }

    Uart0Device::~Uart0Device() {
        // nothing go here
    }

    void Uart0Device::init() {
        unsigned int r;
        write32(UART0_CR, 0);
        rpi3::mbox[0] = 8*4;
        rpi3::mbox[1] = MBOX_REQUEST;
        rpi3::mbox[2] = MBOX_TAG_SETCLKRATE;
        rpi3::mbox[3] = 12;
        rpi3::mbox[4] = 8;
        rpi3::mbox[5] = 2;
        rpi3::mbox[6] = 4000000;     // 4Mhz
        rpi3::mbox[7] = MBOX_TAG_LAST;
        rpi3::mbox_call(MBOX_CH_PROP);
        r = read32(GPFSEL1);
        r &=~((7 << 12) | (7 << 15));
        r |=(4 << 12) | (4 << 15);
        write32(GPFSEL1, r);
        write32(GPPUD, 0);
        r = 150;
        while(r--) {
            asm volatile("nop");
        }
        write32(GPPUDCLK0, (1 << 14) | (1 << 15));
        r = 150;
        while(r--) {
            asm volatile("nop");
        }
        write32(GPPUDCLK0, 0x0);
        write32(UART0_ICR, 0x7FF);
        write32(UART0_IBRD, 2);
        write32(UART0_FBRD, 0xB);
        write32(UART0_LCRH, 0b11<<5);
        write32(UART0_CR, 0x301);
    }

    void Uart0Device::write(i32 c) {
        do{
            asm volatile("nop");
        } while(read32(UART0_FR) & 0x20);
        write32(UART0_DR, c);
    }

    void Uart0Device::write(const char *str) {
        while(*str) {
            this->write(*str++);
        }
    }

}
