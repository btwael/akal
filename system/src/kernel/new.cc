#include "akal/core/types.hh"

Size k = 0x100000;

void *operator new(Size sz) {
    return (void *) (k += sz);
}

void *operator new(Size sz, void *&p) {
    return p;
}

void *operator new(Size sz, void *p) {
    return p;
}

void *operator new[](Size sz) {
    return (void *) (k += sz);
}

void operator delete(void *pBlock) noexcept {
}

void operator delete[](void *pBlock) noexcept {
}

void operator delete(void *pBlock, Size nSize) noexcept {
}

void operator delete[](void *pBlock, Size nSize) noexcept {
}
