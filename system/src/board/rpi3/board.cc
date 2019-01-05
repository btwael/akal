#include "akal/kernel/board.hh"

extern "C" void asm_rpi3_aarch64_switchToEL1();
extern "C" void irq_vector_init();

namespace akal {
    namespace board {

        void setupInterruptVector() {
            irq_vector_init();
        }

        void init() {
            asm_rpi3_aarch64_switchToEL1();
        }

    }
}
