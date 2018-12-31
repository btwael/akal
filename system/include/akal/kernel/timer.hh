#ifndef BOUTGLAY_AKAL_KERNEL_TIMER_HH
#define BOUTGLAY_AKAL_KERNEL_TIMER_HH

// akal/core
#include "akal/core/types.hh"

#include "akal/kernel/device.hh"

namespace akal {

    //*-- TimeUnit
    enum class TimeUnit {
        microsecond,
        millisecond,
        second,
        minute,
        hour,
        day
    };

    //*-- Timer
    class Timer: public Device {
    public:
        Timer();

        virtual ~Timer();

        virtual void init(Machine& machine) = 0;

        virtual u64 getTimer() = 0;

        virtual void delay(u32 n, TimeUnit unit = TimeUnit::microsecond) = 0;
    };

}

#endif // BOUTGLAY_AKAL_KERNEL_TIMER_HH
