extern akal::rpi3::RaspberryPi3 *machine_ptr;
int cc = 0;
extern "C" void schedule_tail(void) {
    machine_ptr->scheduler.preempt_enable();
}
extern "C" void schedule(void) {
    machine_ptr->scheduler.schedule();
}
extern "C" void end_pid(u64 pid) {
//machine_ptr->uart0.hex(machine_ptr->scheduler.current->pid);
    //machine_ptr->uart0.write("close\n");
    machine_ptr->scheduler._tasks[machine_ptr->scheduler.current->pid]->state = 0;
}
extern "C" void notify_pid(u64 pid) {
    //machine_ptr->uart0.hex(machine_ptr->scheduler.current->pid);
    //machine_ptr->uart0.write("not\n");
}
    extern "C" void enable_irq();
    extern "C" void disable_irq();
    extern "C" void ret_from_fork();
namespace akal {
    extern "C" void cpu_switch_to(Task*, Task*);
    Scheduler::Scheduler() {
            
        }
        void Scheduler::init() {
            nr_tasks = 0;
            current = NULL;
            for(int i = 0; i < 50; i++) {
                _tasks[i] = NULL;
            }
        }
        void Scheduler::addTask(TASKHandler handler, int priority, u64 arg) {
            preempt_disable();
            Task *p = (Task *) new u8[1000];

            p->priority = priority;
            //p->state = TASK_RUNNING;
            p->counter = p->priority;
            p->state = 1;
            p->preempt_count = 1;
            p->cpu_context.x20 = arg;
            p->cpu_context.x19 = (u64) handler;
            p->cpu_context.pc = (unsigned long) ret_from_fork;
            p->cpu_context.sp = (unsigned long) p + 1000;
            int pid = cc++;
            p->cpu_context.x21 = p->pid = pid;
            //machine_ptr->uart0.hex(pid);
            //machine_ptr->uart0.write("create\n");
            _tasks[pid] = p;
            preempt_enable();
        }

                void Scheduler::preempt_disable(void)
{
    if(current) {
      //  machine_ptr->uart0.hex(machine_ptr->scheduler.current->pid);
    //machine_ptr->uart0.write("preedisa\n");
    current->preempt_count++;
}
}

void Scheduler::preempt_enable(void)
{
    if(current) {
       // machine_ptr->uart0.hex(machine_ptr->scheduler.current->pid);
    //machine_ptr->uart0.write("preeena\n");
    current->preempt_count--;
}
}

        void Scheduler::schedule() {
            if(cc == 0) {
                return;
            }
            if(current && current->state == 1) {
            --current->counter;
            if(current->counter > 0 || current->preempt_count >0) {
                return;
            }
            current->counter = 0;
        }
            preempt_disable();
            Task *p;
            int next, c;
            while(1) {
                c = -1;
                next = 0;
                for (int i = 0; i < 50; i++){
                    p = _tasks[i];
                    if(p && p->state == 1 && p->counter > c) {
                        c = p->counter;
                        next = i;
                    }
                }
                if(c) {
                    break;
                }
                for(int i = 0; i < 50; i++) {
                    p = _tasks[i];
                    if(p) {
                        p->counter = p->priority;
                    }
                }
            }
//            machine_ptr->uart0.hex(next);
//            machine_ptr->uart0.write("next\n");
//            machine_ptr->uart0.hex((u64) _tasks[next]->cpu_context.pc);
//            machine_ptr->uart0.write("nextp\n");
            enable_irq();
            switchTo(_tasks[next]);
            preempt_enable();
            disable_irq();
        }

        void Scheduler::switchTo(Task *next) {
            Task * prev = current;
            current = next;
            cpu_switch_to(prev, next);
        }
}
