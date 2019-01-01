#ifndef BOUTGLAY_AKAL_KERNEL_TASK_HH
#define BOUTGLAY_AKAL_KERNEL_TASK_HH

namespace akal {

    struct cpu_context {
        unsigned long x19;
        unsigned long x20;
        unsigned long x21;
        unsigned long x22;
        unsigned long x23;
        unsigned long x24;
        unsigned long x25;
        unsigned long x26;
        unsigned long x27;
        unsigned long x28;
        unsigned long fp;
        unsigned long sp;
        unsigned long pc;
    };

    typedef struct {
        struct cpu_context cpu_context;
        long state;    
        long counter;
        long priority;
        long preempt_count;
    } Task;

    typedef void TASKHandler(void *);

    class Scheduler {
    public:
        int nr_tasks = 0;
        Task *_tasks[50];
        int t = 0;
        Task *current;
        Scheduler();
        void init();
        void addTask(TASKHandler handler, int priority, u64 arg);
        void preempt_disable(void);

void preempt_enable(void);

        void schedule();

        void switchTo(Task *next);
    };

}


#endif // BOUTGLAY_AKAL_KERNEL_BOARD_HH
