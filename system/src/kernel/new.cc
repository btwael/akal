#include "akal/core/types.hh"

Size k = 0x100000;

extern akal::rpi3::RaspberryPi3 *machine_ptr;

void *operator new(Size sz) {
    return (void *) (k += sz);
    //return machine_ptr->memory.allocate(sz);
}

void *operator new(Size sz, void *&p) {
    return p;
}

void *operator new(Size sz, void *p) {
    return p;
}

void *operator new[](Size sz) {
    return (void *) (k += sz);
    //return machine_ptr->memory.allocate(sz);
}

void operator delete(void *address) noexcept {
    //return machine_ptr->memory.deallocate(address);
}

void operator delete[](void *address) noexcept {
    //return machine_ptr->memory.deallocate(address);
}

void operator delete(void *address, Size nSize) noexcept {
    //return machine_ptr->memory.deallocate(address);
}

void operator delete[](void *address, Size nSize) noexcept {
    //return machine_ptr->memory.deallocate(address);
}
