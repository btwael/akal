#ifdef AKAL_APPLICATION_TARGET_RPI3
#include "akal/board/rpi3/machine.hh"
#include "src/board/rpi3/mailbox.cc"
#include "src/board/rpi3/timer.cc"
#include "src/board/rpi3/board.cc"
#include "src/board/rpi3/device/uart.cc"
typedef akal::rpi3::RaspberryPi3 Machine;
#else
#error "Unknow target platform!"
#endif

Machine machine;

void startup(Machine &machine);

extern "C" void akal_main(void) {
    akal::board::init();
    //*-- Clear bss
    extern u8 __bss_start__;
    extern u8 __bss_end__;
    u8 *p = &__bss_start__;
    for(; p + sizeof(u32) < &__bss_end__; p += sizeof(u32)) {
        *((u32 *) p) = 0x0;
    }
    for(; p < &__bss_end__; p++) {
        *p = 0x0;
    }

    //*-- Call constructors for global variable
    extern void (*__init_start__)(void);
    extern void (*__init_end__)(void);
    if(((u64) __init_start__) < ((u64) __init_end__)) {
        for(void (**pFunc) (void) = &__init_start__; pFunc < &__init_end__; pFunc++) {
            if(*pFunc != NULL) {
                (**pFunc)();
            }
        }
    }
    machine.uart1.init(machine);
    machine.uart0.init(machine);
    machine.console.init();
    startup(machine);
}
