// akal/kernel
#include "akal/kernel/device.hh"

namespace akal {

    //*-- Device
    Device::Device() {
        // nothing go here
    }

    Device::~Device() {
        // nothing go here
    }

    //*-- UartDevice
    UartDevice::UartDevice() {
        // nothing go here
    }

    UartDevice::~UartDevice() {
        // nothing go here
    }

    void UartDevice::writeAsHex(u32 d) {
        u32 n;
        for(i32 c = 28; c >= 0; c -= 4) {
            n = (d>>c) & 0xF;
            // 0-9 => '0'-'9', 10-15 => 'A'-'F'
            n += n > 9 ? 0x37 : 0x30;
            this->write(n);
        }
    }

    void UartDevice::writeAsHex(u64 d) {
        this->writeAsHex(*(((unsigned int *) &d) + 1));
        this->writeAsHex(*((unsigned int *) &d));
    }

    //*-- ScreenDevice
    ScreenDevice::ScreenDevice() {
        // nothing go here
    }

    ScreenDevice::~ScreenDevice() {
        // nothing go here
    }
}
