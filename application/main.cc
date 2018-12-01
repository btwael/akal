#include "akal/akal.hh"

void startup(Machine &machine) {
    machine.uart1.write("Akal-based Bare-Metal Application\n");
}
