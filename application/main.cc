#include "akal/akal.hh"
#include "akal/kernel/mmio.hh"

u32 abs(i32 v) {
    return v > 0 ? v : -v;
}

void startup3(void *arg) {
    Machine &machine = *((Machine *) arg);
    for(int j = 250; j < 300; j++) {
        for(int k = 250; k < 300; k++) {
            machine.screen.setPixel(akal::rpi3::Point(j, k), akal::rpi3::Color(0, 0, 255));
    }
    machine.timer.delay(1e+5);
    }
}

void startup2(void *arg) {
    Machine &machine = *((Machine *) arg);
    for(int j = 200; j < 250; j++) {
        for(int k = 200; k < 250; k++) {
            machine.screen.setPixel(akal::rpi3::Point(j, k), akal::rpi3::Color(255, 0, 0));
    }
    machine.timer.delay(1e+5);
    }
}

void startup(void *arg) {
    Machine &machine = *((Machine *) arg);
    machine.scheduler.addTask(startup2, 4, (u64) &machine);
    machine.scheduler.addTask(startup3, 2, (u64) &machine);
    //machine.uart0.write("Akal-based Bare-Metal Application\n");
    machine.console.print(0, 0, "Akal-based Bare-Metal Application\n");
    machine.timer.delay(1e+6);
    machine.console.print(0, 28, "Waex Operating System v0.1\nBoutglay Wael-Amine");
    for(int j = 50; j < 100; j++) {
        for(int k = 50; k < 100; k++) {
            machine.screen.setPixel(akal::rpi3::Point(j, k), akal::rpi3::Color(255, 0, 0));
            machine.timer.delay(1e+3);
    }
    }
    machine.screen.plotLine(100, 100, 200, 200);
    int i = 1;
    while(i < 10) {
        //machine.uart0.write("Deadloop\n");
        machine.console.print(0, i, "Deadloop\n");
        machine.timer.delay(1e+6);
        //if(i == 3) i = *((volatile unsigned int*)0xFFFFFFFFFF000000);
        i++;
    }
}
