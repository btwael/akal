#ifndef BOUTGLAY_AKAL_KERNEL_MMIO_HH
#define BOUTGLAY_AKAL_KERNEL_MMIO_HH

#include "akal/core/types.hh"

inline void write8(Size address, u8 value);

inline void write32(Size address, u32 value);

inline void write64(Size address, u64 value);

inline u8 read8(Size address);

inline u32 read32(Size address);

inline u64 read64(Size address);

namespace akal {

    typedef struct s_Zone {
        Size address;
        Size size;
        bool isFree;
        bool isUsed; // meta
        struct s_Zone *next;
        struct s_Zone *previous;
    } Zone;
    typedef struct s_ZoneContainer {
        Zone zones[10];
        struct s_ZoneContainer *next;
    } ZoneContainer;

    class MemoryManager {
    private:
        Zone *_head;
        ZoneContainer container;

    public:
        MemoryManager();

        void *allocate(Size size, Zone *zone = NULL);

    private:
        void _initZoneContainer(ZoneContainer *container);

        Zone *_getUnusedZone(ZoneContainer *container);

        Zone *_getFreeZone(Size size);
    };

}


#endif // BOUTGLAY_AKAL_KERNEL_MMIO_HH
