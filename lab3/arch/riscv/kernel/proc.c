#include "proc.h"
#include "mm.h"
#include "rand.h"
#include "defs.h"

extern void __dummy();

struct task_struct* idle;           // idle process
struct task_struct* current;        // 指向当前运行线程的 `task_struct`
struct task_struct* task[NR_TASKS]; // 线程数组，所有的线程都保存在此

void task_init()
{
    // 1. 调用 kalloc() 为 idle 分配一个物理页
    idle = (struct task_struct*)kalloc();
    // 2. 设置 state 为 TASK_RUNNING;
    idle->state = TASK_RUNNING;
    // 3. 由于 idle 不参与调度 可以将其 counter / priority 设置为 0
    idle->counter = 0;
    idle->priority = 0;
    // 4. 设置 idle 的 pid 为 0
    idle->pid = 0;
    // 5. 将 current 和 task[0] 指向 idle
    current = idle;
    task[0] = idle;

    // 1. 参考 idle 的设置, 为 task[1] ~ task[NR_TASKS - 1] 进行初始化
    for(int i = 1; i < NR_TASKS; i++)
    {
        task[i] = (struct task_struct*)kalloc();
         // 2. 其中每个线程的 state 为 TASK_RUNNING, counter 为 0, priority 使用 rand() 来设置, pid 为该线程在线程数组中的下标。
        task[i]->state = TASK_RUNNING;
        task[i]->counter = 0;//rand();//rand();
        task[i]->priority = rand();
        task[i]->pid = i;
        // 3. 为 task[1] ~ task[NR_TASKS - 1] 设置 `thread_struct` 中的 `ra` 和 `sp`,
        // 4. 其中 `ra` 设置为 __dummy （见 4.3.2）的地址， `sp` 设置为 该线程申请的物理页的高地址
        //task[i]->thread.ra = ;
        uint64  ret_val;
        __asm__ volatile(
            "la t0, __dummy\n"
            "mv %[ret_val], t0\n"
            :[ret_val] "=r" (ret_val)
            :
            :"memory"
        );
        task[i]->thread.ra = ret_val;
        
        task[i]->thread.sp = (uint64)task[i]   + PGSIZE;
        //printk("SET [PID: %ld, COUNTER: %ld, PRIORITY: %ld, RA: %ld]\n",task[i]->pid, task[i]->counter, task[i]->priority, task[i]->thread.ra);
    }

    printk("...proc_init done!\n");
    // printk("size of thread_info: %ld\n", sizeof(idle->thread_info));
    // printk("size of thread_struct:%ld\n", sizeof(idle->thread));
    // printk("size of task_struct:%ld\n", sizeof(idle));
    //printk("size of page:%ld\n", sizeof(*idle));
}

void dummy() {
    uint64 MOD = 1000000007;
    uint64 auto_inc_local_var = 0;
    int last_counter = -1;

    
    while(1) {
        if (last_counter == -1 || current->counter != last_counter) {
            //current->counter = rand();
            last_counter = current->counter;
            auto_inc_local_var = (auto_inc_local_var + 1) ;//% MOD;
            printk("[PID = %d] is running. auto_inc_local_var = %d\n", current->pid, auto_inc_local_var);
        }
    }
}

extern void __switch_to(struct task_struct* prev, struct task_struct* next);

void switch_to(struct task_struct* next) {
    /* YOUR CODE HERE */
   // printk("current pid: %ld, next pid: %ld\n", current->pid, next->pid);
    if(next->pid != current->pid)
    {        
        //__switch_to(current, next);
        __asm__ volatile(
            "mv t0, %[current]\n"
            "sd ra, 0x28(t0)\n"
            "sd sp, 0x30(t0)\n"
            "sd s0, 0x38(t0)\n"
            "sd s1, 0x40(t0)\n"
            "sd s2, 0x48(t0)\n"
            "sd s3, 0x50(t0)\n"
            "sd s4, 0x58(t0)\n"
            "sd s5, 0x60(t0)\n"
            "sd s6, 0x68(t0)\n"
            "sd s7, 0x70(t0)\n"
            "sd s8, 0x78(t0)\n"
            "sd s9, 0x80(t0)\n"
            "sd s10, 0x88(t0)\n"
            "sd s11, 0x90(t0)\n"

            "mv t0, %[next]\n"
            "ld ra, 0x28(t0)\n"
            "ld sp, 0x30(t0)\n"
            "ld s0, 0x38(t0)\n"
            "ld s1, 0x40(t0)\n"
            "ld s2, 0x48(t0)\n"
            "ld s3, 0x50(t0)\n"
            "ld s4, 0x58(t0)\n"
            "ld s5, 0x60(t0)\n"
            "ld s6, 0x68(t0)\n"
            "ld s7, 0x70(t0)\n"//打开就没有时间间隔。。。
            "ld s8, 0x78(t0)\n"
            "ld s9, 0x80(t0)\n"
            "ld s10, 0x88(t0)\n"
            "ld s11, 0x90(t0)\n"
            :
            :[current]"r"(current), [next]"r"(next)
            :"memory"
        );
        current = next;
        printk("\nswitch to [PID %ld, COUNTER %ld, PRIORITY %ld]\n", next->pid, next->counter, next->priority);
    }
}

void do_timer(void) {
    /* 1. 如果当前线程是 idle 线程 直接进行调度 */
    /* 2. 如果当前线程不是 idle 对当前线程的运行剩余时间减 1 
          若剩余时间任然大于0 则直接返回 否则进行调度 */

    /* YOUR CODE HERE */

    uint64 counter_sum = 0;
    for(int i = 1; i < NR_TASKS; i++)
    {
        counter_sum += task[i]->counter;
    }
    if(counter_sum <= 1)
    {
        printk("\n");
        for(int i = 1; i < NR_TASKS; i++)
        {
            task[i]->counter = rand();
            printk("SET [PID: %ld, COUNTER: %ld, PRIORITY: %ld]\n",task[i]->pid, task[i]->counter, task[i]->priority);
        }
        schedule();
    }
    
    if(current == task[0])
        schedule();
    else
    {
        current->counter -= 1;

        if(current->counter > 0)        
            return;
        else
            schedule();            
    }
}

#ifdef SJF
//DSJF(短作业优先调度算法)
void schedule(void)
{
    /*YOUR CODE HERE*/
    struct task_struct* p;

    for(int i = 1; i < NR_TASKS; i++)
    {
        if(task[i]->counter == 0)
            continue;
        p = task[i];
        break;
    }    
    for(int i = p->pid + 1; i < NR_TASKS; i++)
    {
        if(task[i]->counter == 0)
            continue;
        p = (task[i])->counter < p->counter?task[i]:p;
    }   
         
    switch_to(p);
}
#endif

#ifdef PRIORITY
//DPRIORITY(优先级调度算法)
 void schedule(void)
{
    struct task_struct* p;

    for(int i = 1; i < NR_TASKS; i++)
    {
        if(task[i]->counter == 0)
            continue;
        p = task[i];
        break;
    }
    for(int i = p->pid + 1; i < NR_TASKS; i++)
    {
        if(task[i]->counter == 0)
            continue;
        p = (task[i]->priority > p->priority)?task[i]:p;
    }

    switch_to(p);
} 
#endif