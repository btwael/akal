#include "akal/akal.hh"
#include "akal/kernel/mmio.hh"

u32 abs(i32 v) {
    return v > 0 ? v : -v;
}

/*void startup3(void *arg) {
    Machine &machine = *((Machine *) arg);
    for(int j = 250; j < 300; j++) {
        for(int k = 250; k < 300; k++) {
            machine.screen.setPixel(akal::Point(j, k), akal::Color(0, 0, 255));
    }
    machine.timer.delay(1e+5);
    }
}

void startup2(void *arg) {
    Machine &machine = *((Machine *) arg);
    for(int j = 200; j < 250; j++) {
        for(int k = 200; k < 250; k++) {
            machine.screen.setPixel(akal::Point(j, k), akal::Color(255, 0, 0));
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
            machine.screen.setPixel(akal::Point(j, k), akal::Color(255, 0, 0));
            machine.timer.delay(1e+3);
    }
    }
    machine.screen.plotLine(akal::Point(100, 100), akal::Point(200, 200), akal::Color(0, 255, 255));
    int i = 1;
    while(i < 10) {
        //machine.uart0.write("Deadloop\n");
        machine.console.print(0, i, "Deadloop\n");
        machine.timer.delay(1e+6);
        //if(i == 3) i = *((volatile unsigned int*)0xFFFFFFFFFF000000);
        i++;
    }
}*/

akal::Color colors[3] = {akal::Color(255, 0, 0), akal::Color(0, 255, 0), akal::Color(0, 0, 255)};

void draw(Machine& machine, u32 id, const char *name, u32 step) {
    machine.console.print(3, (id + 5) * 2, name);
    //*-- Draw progressbar border
    //*---- Top
    machine.screen.plotLine(akal::Point(97, 195 + (id - 1) * 16 * 2), akal::Point(731, 195 + (id - 1) * 16 * 2), akal::Color(0, 0, 0));
    //*---- Down
    machine.screen.plotLine(akal::Point(97, 204 + (id - 1) * 16 * 2), akal::Point(731, 204 + (id - 1) * 16 * 2), akal::Color(0, 0, 0));
    //*---- Left
    machine.screen.plotLine(akal::Point(97, 195 + (id - 1) * 16 * 2), akal::Point(97, 204 + (id - 1) * 16 * 2), akal::Color(0, 0, 0));
    //*---- Right
    machine.screen.plotLine(akal::Point(731, 195 + (id - 1) * 16 * 2), akal::Point(731, 204 + (id - 1) * 16 * 2), akal::Color(0, 0, 0));
    //*-- Draw progress
    int width = (731 - 97) * step / 100;
    int height = 204 - 195;
    for(i32 i = 1; i < width; i++) {
        for(i32 j = 1; j < height; j++) {
            machine.screen.setPixel(akal::Point(97 + i, 195  + (id - 1) * 16 * 2 + j), colors[id - 1]);
        }
    }
}

void task1(void *arg) {
    Machine &machine = *((Machine *) arg);
    draw(machine, 1, "task1", 0);
    for(int j = 0; j < 250; j++) {
        for(int k = 0; k < 250; k++) {
            draw(machine, 1, "task1", j * k * 100 / (250 * 250));
        }
    }
    draw(machine, 1, "task1", 100);
}

void task2(void *arg) {
    Machine &machine = *((Machine *) arg);
    draw(machine, 2, "task2", 0);
    for(int j = 0; j < 250; j++) {
        for(int k = 0; k < 250; k++) {
            draw(machine, 2, "task2", j * k * 100 / (250 * 250));
        }
    }
    draw(machine, 2, "task2", 100);
}

void task3(void *arg) {
    Machine &machine = *((Machine *) arg);
    draw(machine, 3, "task3", 0);
    for(int j = 0; j < 250; j++) {
        for(int k = 0; k < 250; k++) {
            draw(machine, 3, "task2", j * k * 100 / (250 * 250));
        }
    }
    draw(machine, 3, "task3", 100);
}

void startup(void *arg) {
    Machine &machine = *((Machine *) arg);
    machine.scheduler.addTask(task1, 1, (u64) &machine);
    machine.scheduler.addTask(task2, 1, (u64) &machine);
    machine.scheduler.addTask(task3, 2, (u64) &machine);
}
