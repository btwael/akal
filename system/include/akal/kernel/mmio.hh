#ifndef BOUTGLAY_AKAL_KERNEL_MMIO_HH
#define BOUTGLAY_AKAL_KERNEL_MMIO_HH

#include "akal/core/types.hh"

inline void write8(Size address, u8 value);

inline void write32(Size address, u32 value);

inline void write64(Size address, u64 value);

inline u8 read8(Size address);

inline u32 read32(Size address);

inline u64 read64(Size address);


#endif // BOUTGLAY_AKAL_KERNEL_MMIO_HH
