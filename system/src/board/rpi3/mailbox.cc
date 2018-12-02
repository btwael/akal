// akal/core
#include "akal/core/types.hh"
// akal/kernel
#include "akal/kernel/mmio.hh"

#define VIDEOCORE_MBOX  (MMIO_BASE + 0x0000B880)
#define MBOX_READ       (VIDEOCORE_MBOX + 0x0)
#define MBOX_POLL       (VIDEOCORE_MBOX + 0x10)
#define MBOX_SENDER     (VIDEOCORE_MBOX + 0x14)
#define MBOX_STATUS     (VIDEOCORE_MBOX + 0x18)
#define MBOX_CONFIG     (VIDEOCORE_MBOX + 0x1C)
#define MBOX_WRITE      (VIDEOCORE_MBOX + 0x20)
#define MBOX_RESPONSE   0x80000000
#define MBOX_FULL       0x80000000
#define MBOX_EMPTY      0x40000000

namespace akal {
    namespace rpi3 {

        volatile u32  __attribute__((aligned(16))) mbox[36];

        i32 mbox_call(u8 ch)
        {
            u32 r = (((u32)((u64) &mbox)&~0xF) | (ch & 0xF));
            do{
                asm volatile("nop");
            } while(read32(MBOX_STATUS) & MBOX_FULL);
            write32(MBOX_WRITE, r);
            while(1) {
                do{
                    asm volatile("nop");
                } while(read32(MBOX_STATUS) & MBOX_EMPTY);
                if(r == read32(MBOX_READ))
                    return mbox[1] == MBOX_RESPONSE;
            }
            return 0;
        }

    }
}
