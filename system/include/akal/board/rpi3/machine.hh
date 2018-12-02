#ifndef BOUTGLAY_AKAL_BOARD_RPI3_MACHINE_HH
#define BOUTGLAY_AKAL_BOARD_RPI3_MACHINE_HH

// akal/kernel
#include "akal/kernel/machine.hh"
#include "akal/kernel/mmio.hh"
// akal/board
#include "akal/board/rpi3/device/uart.hh"

#define MMIO_BASE       (0x3f000000)

namespace akal {

    //*-- RaspberryPi3
    class RaspberryPi3: public Machine {
    public:
        Uart1Device uart1;
        Uart0Device uart0;
    };

}


#endif // BOUTGLAY_AKAL_BOARD_RPI3_MACHINE_HH
