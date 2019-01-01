// akal/board
#include "akal/board/rpi3/timer.hh"

extern "C" void generic_timer_init();

const unsigned int interval = 20000000;
unsigned int curVal = 0;
extern akal::rpi3::RaspberryPi3 *machine_ptr;

extern "C" void generic_timer_reset();

#define PERIPHERAL_BASE     0x40000000
#define TIMER_CTRL      (PERIPHERAL_BASE+0x34)
#define TIMER_FLAG      (PERIPHERAL_BASE+0x38)
#define CORE0_INT_CTR       (PERIPHERAL_BASE+0x40)
#define LOCAL_TIMER_INT     (1 << 11)
#define CNTPNSIRQ_Int       (1 << 1)

namespace akal {
    namespace rpi3 {

        void handle_timer_irq() {
            machine_ptr->console.print(0, 12, "irq");
            #ifdef AKAL_APPLICATION_TARGET_RPI3QEMU
                generic_timer_reset();
            #else
                write32(TIMER_FLAG, (3<<30));
            #endif
            machine_ptr->scheduler.schedule();
        }

        //*-- ARMTimer
        ARMTimer::ARMTimer() {
            // nothing go here
        }

        ARMTimer::~ARMTimer() {
            // nothing go here
        }

        void ARMTimer::init(Machine& machine) {
            u32 irq;
            #ifdef AKAL_APPLICATION_TARGET_RPI3QEMU
                generic_timer_init();
            #else
                // Set value, enable Timer and Interrupt
                write32(TIMER_CTRL, ((1<<28) | interval));
            #endif
            #ifdef AKAL_APPLICATION_TARGET_RPI3QEMU
                write32(CORE0_INT_CTR, (1 << 1));
                irq = CNTPNSIRQ_Int;
            #else
            // Enable IRQ Core 0 - Pag. 13 BCM2836_ARM-local_peripherals
                unsigned int local_timer_ctrl = read32(TIMER_CTRL);
                write32(TIMER_CTRL, (local_timer_ctrl | (1 << 29)));
                irq = LOCAL_TIMER_INT;
            #endif
            machine_ptr->interruptController.connectIRQ(irq, handle_timer_irq);
        }

        u64 ARMTimer::getTimer(){
            u32 h = -1, l;
            h = read32(SYSTMR_HI);
            l = read32(SYSTMR_LO);
            if(h != read32(SYSTMR_HI)) {
                h = read32(SYSTMR_HI);
                l = read32(SYSTMR_LO);
            }
            return ((unsigned long) h << 32) | l;
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
            #ifndef AKAL_APPLICATION_TARGET_RPI3QEMU
            unsigned long t = getTimer();
            if(t) {
                // TODO: support qemu
                while(getTimer() < t+n);
            }
            #else
            u64 f, t, r;
            asm volatile ("mrs %0, cntfrq_el0" : "=r"(f));
            asm volatile ("mrs %0, cntpct_el0" : "=r"(t));
            t+=((f/1000)*n)/1000;
            do{asm volatile ("mrs %0, cntpct_el0" : "=r"(r));}while(r<t);
            #endif
        }

    }
}
