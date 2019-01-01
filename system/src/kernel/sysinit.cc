#ifdef AKAL_APPLICATION_TARGET_RPI3
#include "akal/board/rpi3/machine.hh"
typedef akal::rpi3::RaspberryPi3 Machine;
#include "src/board/rpi3/mailbox.cc"
#include "src/board/rpi3/timer.cc"
#include "src/board/rpi3/interrupt.cc"
#include "src/board/rpi3/board.cc"
#include "src/board/rpi3/device/uart.cc"
#else
#error "Unknow target platform!"
#endif

Machine machine;

void startup(void *arg);
extern "C" void enable_irq();
extern Machine *machine_ptr = NULL;

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
    machine_ptr = &machine;
    akal::board::setupInterruptVector(); // TODO: arch
    machine.interruptController.init(machine);
    machine.timer.init(machine);
    machine.uart1.init(machine);
    machine.uart0.init(machine);
    machine.console.init();
    machine.scheduler.init();
    machine.scheduler.addTask(startup, 4, (u64) &machine);
    enable_irq();
    //startup(machine);
    while(1) {}
}
