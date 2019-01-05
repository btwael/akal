extern "C" void asm_aarch64_enableInterrupts();
extern "C" void asm_aarch64_disableInterrupts();

namespace akal {
    namespace arch {

        void enableInterrupts() {
            asm_aarch64_enableInterrupts();
        }

        void disableInterrupts() {
            asm_aarch64_disableInterrupts();
        }

    }
}
