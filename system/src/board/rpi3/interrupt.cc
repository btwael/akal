// akal/board
#include "akal/board/rpi3/machine.hh"
#include "akal/board/rpi3/interrupt.hh"

#define ARM_IO_BASE     0x3F000000

#define PERIPHERAL_BASE     0x40000000
#define CORE0_INT_SOURCE    (PERIPHERAL_BASE+0x60)

extern akal::rpi3::RaspberryPi3 *machine_ptr;

#define ARM_IC_BASE     (ARM_IO_BASE + 0xB000)

#define ARM_IRQ1_BASE       0
#define ARM_IRQ2_BASE       (ARM_IRQ1_BASE + ARM_IRQS_PER_REG)
#define ARM_IRQBASIC_BASE   (ARM_IRQ2_BASE + ARM_IRQS_PER_REG)

#define ARM_IC_IRQ_BASIC_PENDING  (ARM_IC_BASE + 0x200)
#define ARM_IC_IRQ_PENDING_1      (ARM_IC_BASE + 0x204)
#define ARM_IC_IRQ_PENDING_2      (ARM_IC_BASE + 0x208)
#define ARM_IC_FIQ_CONTROL    (ARM_IC_BASE + 0x20C)
#define ARM_IC_ENABLE_IRQS_1      (ARM_IC_BASE + 0x210)
#define ARM_IC_ENABLE_IRQS_2      (ARM_IC_BASE + 0x214)
#define ARM_IC_ENABLE_BASIC_IRQS  (ARM_IC_BASE + 0x218)
#define ARM_IC_DISABLE_IRQS_1     (ARM_IC_BASE + 0x21C)
#define ARM_IC_DISABLE_IRQS_2     (ARM_IC_BASE + 0x220)
#define ARM_IC_DISABLE_BASIC_IRQS (ARM_IC_BASE + 0x224)

#define ARM_IC_IRQ_PENDING(irq) (  (irq) < ARM_IRQ2_BASE    \
                 ? ARM_IC_IRQ_PENDING_1     \
                 : ((irq) < ARM_IRQBASIC_BASE   \
                   ? ARM_IC_IRQ_PENDING_2   \
                   : ARM_IC_IRQ_BASIC_PENDING))
#define ARM_IC_IRQS_ENABLE(irq) (  (irq) < ARM_IRQ2_BASE    \
                 ? ARM_IC_ENABLE_IRQS_1     \
                 : ((irq) < ARM_IRQBASIC_BASE   \
                   ? ARM_IC_ENABLE_IRQS_2   \
                   : ARM_IC_ENABLE_BASIC_IRQS))
#define ARM_IC_IRQS_DISABLE(irq) (  (irq) < ARM_IRQ2_BASE   \
                 ? ARM_IC_DISABLE_IRQS_1    \
                 : ((irq) < ARM_IRQBASIC_BASE   \
                   ? ARM_IC_DISABLE_IRQS_2  \
                   : ARM_IC_DISABLE_BASIC_IRQS))
#define ARM_IRQ_MASK(irq)   (1 << ((irq) & (ARM_IRQS_PER_REG-1)))

#define LOCAL_TIMER_INT     (1 << 11)
#define CNTPNSIRQ_Int       (1 << 1)

namespace akal {
    namespace rpi3 {

        InterruptController::InterruptController() {

        }

        InterruptController::~InterruptController() {

        }

        void InterruptController::init(Machine& machine) {
            for(u32 i = 0; i < IRQ_LINES; i++) {
                _handlers[i] = NULL;
            }
        }

        void InterruptController::connectIRQ(u32 irq, IRQHandler *handler) {
            _handlers[irq] = handler;
            this->enableIRQ(irq);
        }

        void InterruptController::enableIRQ(u32 irq) {
            if(irq != LOCAL_TIMER_INT && irq != CNTPNSIRQ_Int) {
                //DataMemBarrier ();

                write32(ARM_IC_IRQS_ENABLE(irq), ARM_IRQ_MASK(irq));

                //DataMemBarrier ();
            }
        }

        void InterruptController::handle(u32 irq) {
            (*_handlers[irq])();
        }

        const char *entry_error_messages[] = {
            "SYNC_INVALID_EL1t",
            "IRQ_INVALID_EL1t",     
            "FIQ_INVALID_EL1t",     
            "ERROR_INVALID_EL1T",       

            "SYNC_INVALID_EL1h",        
            "IRQ_INVALID_EL1h",     
            "FIQ_INVALID_EL1h",     
            "ERROR_INVALID_EL1h",       

            "SYNC_INVALID_EL0_64",      
            "IRQ_INVALID_EL0_64",       
            "FIQ_INVALID_EL0_64",       
            "ERROR_INVALID_EL0_64", 

            "SYNC_INVALID_EL0_32",      
            "IRQ_INVALID_EL0_32",       
            "FIQ_INVALID_EL0_32",       
            "ERROR_INVALID_EL0_32"  
        };

        #define PERIPHERAL_BASE     0x40000000
        #define TIMER_CTRL      (PERIPHERAL_BASE+0x34)
        #define TIMER_FLAG      (PERIPHERAL_BASE+0x38)

        extern "C" void timer_reset();

        extern "C" void show_invalid_entry_message(int type, unsigned long esr, unsigned long address) {
            machine_ptr->console.print(0, 10, "error");
            //machine_ptr->uart0.hex(esr);
            machine_ptr->console.print(0, 11, entry_error_messages[type]);
        }

        extern "C" void handle_irq() {
            unsigned int irq = read32(CORE0_INT_SOURCE);
            machine_ptr->interruptController.handle(irq);
        }

    }
}
