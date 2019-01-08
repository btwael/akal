#include "akal/kernel/mmio.hh"
#include "akal/core/types.hh"

extern akal::rpi3::RaspberryPi3 *machine_ptr;

inline void write8(Size address, u8 value) {
  *((volatile u8 *) address) = value;
}

inline void write32(Size address, u32 value) {
  *((volatile u32 *) address) = value;
}

inline void write64(Size address, u64 value) {
  *((volatile u64 *) address) = value;
}

inline u8 read8(Size address) {
  return *((volatile u8 *) address);
}

inline u32 read32(Size address) {
  return *((volatile u32 *) address);
}

inline u64 read64(Size address) {
  return *((volatile u64 *) address);
}

namespace akal {

    MemoryManager::MemoryManager() {
        _initZoneContainer(&this->container);
        this->container.zones[0].address = 0x100000;
        this->container.zones[0].size = 0x100000; //  TODO:
        this->container.zones[0].isFree = true;
        this->container.zones[0].isUsed = true;
        this->container.zones[0].next = NULL;
        this->container.zones[0].previous = NULL;
        this->_head = &this->container.zones[0];
    }

    void *MemoryManager::allocate(Size size, Zone *zone) {
        if(zone == NULL) {
            zone = _getUnusedZone(&this->container);
        }
        Zone *freeZone = _getFreeZone(size);
        zone->address = freeZone->address;
        zone->size = size;
        zone->isFree = false;
        zone->isUsed = true;
        zone->next = freeZone;
        zone->previous = freeZone->previous;
        freeZone->address += size;
        freeZone->size -= size;
        freeZone->previous = zone;
        return (void *) zone->address;
    }

    void MemoryManager::_initZoneContainer(ZoneContainer *container) {
        for(i32 i = 0; i < 10; i++) {
            container->zones[i].isUsed = false;
        }
        container->next = NULL;
    }

    Zone *MemoryManager::_getUnusedZone(ZoneContainer *container) {
        for(i32 i = 0; i < 9; i++) {
            if(container->zones[i].isUsed == false) {
                return &container->zones[i];
            }
        }
        if(container->next != NULL) {
            return _getUnusedZone(container->next);
        }
        _initZoneContainer((ZoneContainer *) this->allocate(sizeof(ZoneContainer), &container->zones[9]));
        container->next = (ZoneContainer *) container->zones[9].address;
        return _getUnusedZone(container->next);
    }

    Zone *MemoryManager::_getFreeZone(Size size) {
        Zone *z = this->_head;
        while(z != NULL) {
            if(z->isFree && size < z->size) {
                return z;
            }
            z = z->next;
        }
        return NULL;
    }

}
