#ifndef BOUTGLAY_AKAL_BOARD_RPI3_DEVICE_UART_HH
#define BOUTGLAY_AKAL_BOARD_RPI3_DEVICE_UART_HH

// akal/kernel
#include "akal/kernel/device.hh"
#include "akal/kernel/mmio.hh"

namespace akal {
    namespace rpi3 {

        //*-- Uart1Device
        // this is a driver for mini UART
        class Uart1Device: public UartDevice {
        public:
            Uart1Device();

            ~Uart1Device();

            void init(Machine& machine);

            void write(i32 c);

            void write(const char *str);
        };

        //*-- Uart0Device
        class Uart0Device: public UartDevice {
        public:
            Uart0Device();

            ~Uart0Device();

            void init(Machine& machine);

            void write(i32 c);

            void write(const char *str);
        };

    }
}


#endif // BOUTGLAY_AKAL_BOARD_RPI3_DEVICE_UART_HH
