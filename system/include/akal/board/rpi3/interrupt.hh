#ifndef BOUTGLAY_AKAL_BOARD_RPI3_INTERRUPT_HH
#define BOUTGLAY_AKAL_BOARD_RPI3_INTERRUPT_HH

// akal/kernel

#define ARM_IRQS_PER_REG    32

#define IRQ_LINES       (ARM_IRQS_PER_REG * 2 + 8)

namespace akal {

    class Machine;

    namespace rpi3 {

        typedef void IRQHandler();

        class InterruptController {
        private:
            IRQHandler *_handlers[IRQ_LINES];

        public:
            InterruptController();

            ~InterruptController();

            void init(Machine& machine);

            void connectIRQ(u32 irq, IRQHandler *handler);

            void enableIRQ(u32 irq);

            void handle(u32 irq);
        };

    }
}


#endif // BOUTGLAY_AKAL_BOARD_RPI3_INTERRUPT_HH
