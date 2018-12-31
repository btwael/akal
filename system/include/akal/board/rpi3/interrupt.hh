#ifndef BOUTGLAY_AKAL_BOARD_RPI3_INTERRUPT_HH
#define BOUTGLAY_AKAL_BOARD_RPI3_INTERRUPT_HH

// akal/kernel
#include "akal/kernel/machine.hh"

namespace akal {
    namespace rpi3 {

        class InterruptController {
        public:
            InterruptController();

            ~InterruptController();

            void init(Machine& machine);
        };

    }
}


#endif // BOUTGLAY_AKAL_BOARD_RPI3_INTERRUPT_HH
