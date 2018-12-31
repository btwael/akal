#ifndef BOUTGLAY_AKAL_BOARD_RPI3_INTERRUPTSYSTEM_HH
#define BOUTGLAY_AKAL_BOARD_RPI3_INTERRUPTSYSTEM_HH

// akal/core
#include "akal/core/types.hh"
// akal/kernel
#include "akal/kernel/machine.hh"

namespace akal {
    namespace rpi3 {

        class InterruptSystem {
        public:
            void init(Machine& machine);
        };

    }
}


#endif // BOUTGLAY_AKAL_BOARD_RPI3_INTERRUPTSYSTEM_HH
