#ifndef BOUTGLAY_AKAL_BOARD_RPI3_MACHINE_HH
#define BOUTGLAY_AKAL_BOARD_RPI3_MACHINE_HH

// akal/kernel
#include "akal/kernel/machine.hh"
#include "akal/kernel/mmio.hh"
#include "akal/kernel/timer.hh"
// akal/board/rpi3
#include "akal/board/rpi3/timer.hh"
#include "akal/board/rpi3/device/uart.hh"

namespace akal {
    namespace rpi3 {

        //*-- RaspberryPi3
        class RaspberryPi3: public Machine {
        public:
            Uart1Device uart1;
            Uart0Device uart0;
            ARMTimer timer;
        };

    }
}


#endif // BOUTGLAY_AKAL_BOARD_RPI3_MACHINE_HH
