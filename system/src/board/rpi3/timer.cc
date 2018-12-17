// akal/board
#include "akal/board/rpi3/timer.hh"

namespace akal {
    namespace rpi3 {

        //*-- ARMTimer
        ARMTimer::ARMTimer() {
            // nothing go here
        }

        ARMTimer::~ARMTimer() {
            // nothing go here
        }

        u64 ARMTimer::getTimer(){
            u32 h = -1, l;
            h = read32(SYSTMR_HI);
            l = read32(SYSTMR_LO);
            if(h != read32(SYSTMR_HI)) {
                h = read32(SYSTMR_HI);
                l = read32(SYSTMR_LO);
            }
            return ((u64) h << 32) | l;
        }

        void ARMTimer::delay(u32 n, TimeUnit unit) {
            switch(unit) {
                case TimeUnit::microsecond:
                    break;
                case TimeUnit::millisecond:
                    n = n * 1000;
                    break;
                case TimeUnit::second:
                    n = n * 1000000;
                    break;
                case TimeUnit::minute:
                    n = n * 1000000 * 60;
                    break;
                case TimeUnit::hour:
                    n = n * 1000000 * 60 * 60;
                    break;
                 case TimeUnit::day:
                    n = n * 1000000 * 60 * 60 * 24;
                    break;
            }
            u64 f, t, r;
            // get the current counter frequency
            asm volatile ("mrs %0, cntfrq_el0" : "=r" (f));
            // read the current counter
            asm volatile ("mrs %0, cntpct_el0" : "=r" (t));
            // calculate expire value for counter
            t += ((f / 1000) * n) / 1000;
            do {
                asm volatile ("mrs %0, cntpct_el0" : "=r" (r));
            } while(r < t);
        }

    }
}
