#include "akal/akal.hh"

void startup(Machine &machine) {
    machine.uart0.write("Akal-based Bare-Metal Application\n");
}
