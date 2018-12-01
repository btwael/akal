#ifndef BOUTGLAY_AKAL_BOARD_RPI3_MACHINE_HH
#define BOUTGLAY_AKAL_BOARD_RPI3_MACHINE_HH

// akal/kernel
#include "akal/kernel/machine.hh"
#include "akal/kernel/mmio.hh"
// akal/board
#include "akal/board/rpi3/device/uart.hh"

namespace akal {

    //*-- RaspberryPi3
    class RaspberryPi3: public Machine {
    public:
        Uart1Device uart1;
    };

}


#endif // BOUTGLAY_AKAL_BOARD_RPI3_MACHINE_HH
