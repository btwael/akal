#include "akal/akal.hh"
#include "akal/kernel/mmio.hh"

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

const unsigned int interval = 20000000;
unsigned int curVal = 0;

#define PERIPHERAL_BASE     0x40000000
#define TIMER_CTRL      (PERIPHERAL_BASE+0x34)
#define TIMER_FLAG      (PERIPHERAL_BASE+0x38)

void timer_init( void )
{
    // Set value, enable Timer and Interrupt
    write32(TIMER_CTRL, ((1<<28) | interval));
}

extern "C" void generic_timer_reset();
extern "C" void timer_reset();

extern "C" void show_invalid_entry_message(int type, unsigned long esr, unsigned long address) {
    machine.console.print(0, 10, "error");
    machine.console.print(0, 11, entry_error_messages[type]);
}

extern "C" void handle_irq() {
    machine.console.print(0, 12, "irq");
    write32(TIMER_FLAG, (3<<30));
}

void enable_interrupt_controller()
{
    // Enable IRQ Core 0 - Pag. 13 BCM2836_ARM-local_peripherals
    unsigned int local_timer_ctrl = read32(TIMER_CTRL);
    write32(TIMER_CTRL, (local_timer_ctrl | (1 << 29)));
}

extern "C" void enable_irq();
extern "C" void irq_vector_init();
extern "C" void switchtoEL2();

void startup(Machine &machine) {
    u64 el;
    asm volatile ("mrs %0, CurrentEL" : "=r" (el));
    char *k = "A";
    k[0] = '0' + ((el>>2)&3);
    machine.console.print(0, 14, k);
    switchtoEL2();
    asm volatile ("mrs %0, CurrentEL" : "=r" (el));
    k[0] = '0' + ((el>>2)&3);
    machine.console.print(0, 15, k);
    //machine.uart0.write("Akal-based Bare-Metal Application\n");
    machine.console.print(0, 0, "Akal-based Bare-Metal Application\n");
    irq_vector_init();
    timer_init();
    enable_interrupt_controller();
    enable_irq();
    machine.timer.delay(1e+6);
    machine.console.print(0, 28, "Waex Operating System v0.1\nBoutglay Wael-Amine");
    int i = 1;
    while(i < 10) {
        //machine.uart0.write("Deadloop\n");
        machine.console.print(0, i, "Deadloop\n");
        machine.timer.delay(5e+6);
        //if(i == 3) i = *((volatile unsigned int*)0xFFFFFFFFFF000000);
        i++;
    }
    while(1) {}
}
