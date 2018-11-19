#include "akal/akal.hh"

void startup(Machine &machine) {
    machine.uart1.write("Hello world");
}
