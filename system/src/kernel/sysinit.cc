#ifdef AKAL_APPLICATION_TARGET_RPI3
#include "akal/board/rpi3/machine.hh"
#include "src/board/rpi3/mailbox.cc"
#include "src/board/rpi3/device/uart.cc"
typedef akal::RaspberryPi3 Machine;
#else
#error "Unknow target platform!"
#endif

Machine machine;

void startup(Machine &machine);

extern "C" void akal_main(void) {
    machine.uart1.init();
    machine.uart0.init();
    startup(machine);
}
