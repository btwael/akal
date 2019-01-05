#ifndef BOUTGLAY_AKAL_KERNEL_TASK_HH
#define BOUTGLAY_AKAL_KERNEL_TASK_HH

#include "akal/core/types.hh"

namespace akal {

    struct cpu_context {
        u64 x19;
        u64 x20;
        u64 x21;
        u64 x22;
        u64 x23;
        u64 x24;
        u64 x25;
        u64 x26;
        u64 x27;
        u64 x28;
        u64 fp;
        u64 sp;
        u64 pc;
    };

    typedef enum {
        Thread,
        Fiber
    } TaskKind;

    typedef struct {
        struct cpu_context cpu_context;
        i64 state;    
        i64 counter;
        i64 priority;
        i64 preempt_count;
        i64 pid;
        TaskKind kind;
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
