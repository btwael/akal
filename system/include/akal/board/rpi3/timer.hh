#ifndef BOUTGLAY_AKAL_BOARD_RPI3_TIMER_HH
#define BOUTGLAY_AKAL_BOARD_RPI3_TIMER_HH

// akal/kernel
#include "akal/kernel/mmio.hh"
#include "akal/kernel/timer.hh"

#define MMIO_BASE       (0x3f000000)

#define SYSTMR_LO       (MMIO_BASE + 0x00003004)
#define SYSTMR_HI       (MMIO_BASE + 0x00003008)

namespace akal {
    namespace rpi3 {

        class ARMTimer {
        public:
            ARMTimer();

            ~ARMTimer();

            u64 getTimer();

            void delay(u32 n, TimeUnit unit = TimeUnit::microsecond);
        };

    }
}


#endif // BOUTGLAY_AKAL_BOARD_RPI3_TIMER_HH
