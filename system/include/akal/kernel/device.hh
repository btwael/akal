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

    class UartDevice: public Device {
    public:
        UartDevice();

        virtual ~UartDevice();

        virtual void init(Machine& machine) = 0;

        virtual void write(i32 c) = 0;

        virtual void write(const char *str) = 0;

        void writeAsHex(u32 d);

        void writeAsHex(u64 d);
    };

    class Point {
    public:
        u32 x, y;
        Point(u32 x, u32 y) : x(x), y(y) {}
    };

    class Color {
    public:
        u8 r, g, b;
        Color(u8 r, u8 g, u8 b) : r(r), g(g), b(b) {}
    };

    class ScreenDevice: public Device {
    public:
        ScreenDevice();

        virtual ~ScreenDevice();

        virtual void setPixel(Point p, Color c) = 0;

        virtual void plotLine(Point p0, Point p1, Color color) = 0;
    };

}


#endif // BOUTGLAY_AKAL_KERNEL_DEVICE_HH
