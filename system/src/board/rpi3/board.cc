#include "akal/kernel/board.hh"

extern "C" void asm_rpi3_switchToEL1();

namespace akal {
    namespace board {

        void init() {
            asm_rpi3_switchToEL1();
        }

    }
}
