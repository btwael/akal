#ifndef BOUTGLAY_AKAL_BOARD_RPI3_MAILBOX_HH
#define BOUTGLAY_AKAL_BOARD_RPI3_MAILBOX_HH

// akal/core
#include "akal/core/types.hh"

namespace akal {
    namespace rpi3 {

        extern volatile u32 mbox[36];

        #define MBOX_REQUEST    0

        /* channels */
        #define MBOX_CH_POWER   0
        #define MBOX_CH_FB      1
        #define MBOX_CH_VUART   2
        #define MBOX_CH_VCHIQ   3
        #define MBOX_CH_LEDS    4
        #define MBOX_CH_BTNS    5
        #define MBOX_CH_TOUCH   6
        #define MBOX_CH_COUNT   7
        #define MBOX_CH_PROP    8

        /* tags */
        #define MBOX_TAG_GETSERIAL      0x10004
        #define MBOX_TAG_SETCLKRATE     0x38002
        #define MBOX_TAG_LAST           0

        i32 mbox_call(u8 ch);

    }
}


#endif // BOUTGLAY_AKAL_BOARD_RPI3_MAILBOX_HH
