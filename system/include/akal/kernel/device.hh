#ifndef BOUTGLAY_AKAL_KERNEL_DEVICE_HH
#define BOUTGLAY_AKAL_KERNEL_DEVICE_HH

// akal/core
#include "akal/core/types.hh"

#include "akal/kernel/machine.hh"

namespace akal {

    class Device {
    public:
        Device();

        virtual ~Device();

        virtual void init(Machine& machine) = 0;
    };

    class UartDevice {
    public:
        UartDevice();

        virtual ~UartDevice();

        virtual void init(Machine& machine) = 0;

        virtual void write(i32 c) = 0;

        virtual void write(const char *str) = 0;

        void writeAsHex(u32 d);

        void writeAsHex(u64 d);
    };

}


#endif // BOUTGLAY_AKAL_KERNEL_DEVICE_HH
