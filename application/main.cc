#include "akal/akal.hh"
#include "akal/kernel/mmio.hh"

void startup(Machine &machine) {
    //machine.uart0.write("Akal-based Bare-Metal Application\n");
    machine.console.print(0, 0, "Akal-based Bare-Metal Application\n");
    machine.timer.delay(5e+6);
    machine.console.print(0, 28, "Waex Operating System v0.1\nBoutglay Wael-Amine");
    int i = 1;
    while(i < 10) {
        //machine.uart0.write("Deadloop\n");
        machine.console.print(0, i, "Deadloop\n");
        machine.timer.delay(10e+6);
        i++;
    }
}
