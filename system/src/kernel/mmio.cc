#include "akal/kernel/mmio.hh"
#include "akal/core/types.hh"

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
