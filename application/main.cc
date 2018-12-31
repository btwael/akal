#include "akal/akal.hh"
#include "akal/kernel/mmio.hh"

u32 abs(i32 v) {
    return v > 0 ? v : -v;
}

void startup(Machine &machine) {
    //machine.uart0.write("Akal-based Bare-Metal Application\n");
    machine.console.print(0, 0, "Akal-based Bare-Metal Application\n");
    machine.timer.delay(1e+6);
    machine.console.print(0, 28, "Waex Operating System v0.1\nBoutglay Wael-Amine");
    for(int j = 50; j < 100; j++) {
        for(int k = 50; k < 100; k++) {
            machine.screen.setPixel(akal::rpi3::Point(j, k), akal::rpi3::Color(255, 0, 0));
    }
    }
    machine.screen.plotLine(100, 100, 200, 200);
    int i = 1;
    while(i < 10) {
        //machine.uart0.write("Deadloop\n");
        machine.console.print(0, i, "Deadloop\n");
        machine.timer.delay(5e+6);
        //if(i == 3) i = *((volatile unsigned int*)0xFFFFFFFFFF000000);
        i++;
    }
    while(1) {}
}
