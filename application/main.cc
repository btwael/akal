#include "akal/akal.hh"

void startup(Machine &machine) {
    machine.uart0.write("Akal-based Bare-Metal Application\n");
    while(1) {
        machine.uart0.write("Deadloop\n");
        machine.timer.delay(5, akal::TimeUnit::second);
    }
}
