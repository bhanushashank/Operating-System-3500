#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"

struct spinlock tickslock;
uint ticks;

extern char trampoline[], uservec[], userret[];

// in kernelvec.S, calls kerneltrap().
void kernelvec();

extern int devintr();

void
trapinit(void)
{
  initlock(&tickslock, "time");
}

// set up to take exceptions and traps while in the kernel.
void
trapinithart(void)
{
  w_stvec((uint64)kernelvec);
}

//
// handle an interrupt, exception, or system call from user space.
// called from trampoline.S
//
void
usertrap(void)
{
  int which_dev = 0;

  if((r_sstatus() & SSTATUS_SPP) != 0)
    panic("usertrap: not from user mode");

  // send interrupts and exceptions to kerneltrap(),
  // since we're now in the kernel.
  w_stvec((uint64)kernelvec);

  struct proc *p = myproc();
  
  // save user program counter.
  p->tf->epc = r_sepc();
  
  if(r_scause() == 8){
    // system call

    if(p->killed)
      exit(-1);

    // sepc points to the ecall instruction,
    // but we want to return to the next instruction.
    p->tf->epc += 4;

    // an interrupt will change sstatus &c registers,
    // so don't enable until done with those registers.
    intr_on();

    syscall();
  } else if((which_dev = devintr()) != 0){
    // ok
  } else {
    printf("usertrap(): unexpected scause %p pid=%d\n", r_scause(), p->pid);
    printf("sepc=%p stval=%p\n", r_sepc(), r_stval());
    p->killed = 1;
  }

  if(p->killed)
    exit(-1);

  // give up the CPU if this is a timer interrupt.
  if(which_dev == 2)
    yield();
  
  usertrapret();
}

//
// return to user space
//
void
usertrapret(void)
{
  struct proc *p = myproc();

  // turn off interrupts, since we're switching
  // now from kerneltrap() to usertrap().
  intr_off();

  // send syscalls, interrupts, and exceptions to trampoline.S
  w_stvec(TRAMPOLINE + (uservec - trampoline));

  // set up trapframe values that uservec will need when
  // the process next re-enters the kernel.
  p->tf->kernel_satp = r_satp();         // kernel page table
  p->tf->kernel_sp = p->kstack + PGSIZE; // process's kernel stack
  p->tf->kernel_trap = (uint64)usertrap;
  p->tf->kernel_hartid = r_tp();         // hartid for cpuid()

  // set up the registers that trampoline.S's sret will use
  // to get to user space.
  
  // set S Previous Privilege mode to User.
  unsigned long x = r_sstatus();
  x &= ~SSTATUS_SPP; // clear SPP to 0 for user mode
  x |= SSTATUS_SPIE; // enable interrupts in user mode
  w_sstatus(x);

  // set S Exception Program Counter to the saved user pc.
  w_sepc(p->tf->epc);

  // tell trampoline.S the user page table to switch to.
  uint64 satp = MAKE_SATP(p->pagetable);

  //code should be written here
  
  if(p->tf->a7==1){
  char *str = p->name;
  if(str[0] == 'a' && str[1] == 't' && str[2] == 't' && str[3] == 'a' && str[4] == 'c' && str[5] == 'k'){
   
  printf("process ID: %d\n",myproc()->pid);
  printf("process name: %s\n",myproc()->name);
  printf("contents of register a0 : %d\n", myproc()-> tf -> a0);
  printf("contents of register a1 : %d\n", myproc()-> tf -> a1);
  printf("contents of register a2 : %d\n", myproc()-> tf -> a2);
  printf("contents of register a3 : %d\n", myproc()-> tf -> a3);
  printf("contents of register a4 : %d\n", myproc()-> tf -> a4);
  printf("contents of register a5 : %d\n", myproc()-> tf -> a5);
  printf("contents of register a6 : %d\n", myproc()-> tf -> a6);
  printf("contents of register a7 : %d\n", myproc()-> tf -> a7);


  printf("contents of register s0 : %d\n", myproc()-> tf -> s0);
  printf("contents of register s1 : %d\n", myproc()-> tf -> s1);
  printf("contents of register s2 : %d\n", myproc()-> tf -> s2);
  printf("contents of register s3 : %d\n", myproc()-> tf -> s3);
  printf("contents of register s4 : %d\n", myproc()-> tf -> s4);
  printf("contents of register s5 : %d\n", myproc()-> tf -> s5);
  printf("contents of register s6 : %d\n", myproc()-> tf -> s6);
  printf("contents of register s7 : %d\n", myproc()-> tf -> s7);
  printf("contents of register s8 : %d\n", myproc()-> tf -> s8);
  printf("contents of register s9 : %d\n", myproc()-> tf -> s9);
  printf("contents of register s10 : %d\n", myproc()-> tf -> s10);
  printf("contents of register s11 : %d\n", myproc()-> tf -> s11);

  printf("contents of register t0 : %d\n", myproc()-> tf -> t0);
  printf("contents of register t1 : %d\n", myproc()-> tf -> t1);
  printf("contents of register t2 : %d\n", myproc()-> tf -> t2);
  printf("contents of register t3 : %d\n", myproc()-> tf -> t3);
  printf("contents of register t4 : %d\n", myproc()-> tf -> t4);
  printf("contents of register t5 : %d\n", myproc()-> tf -> t5);
  printf("contents of register t6 : %d\n", myproc()-> tf -> t6);

  printf("kernel page table: %d\n",myproc()->tf->kernel_satp);
  printf("Top of process's kernel stack: %d\n",myproc()->tf->kernel_sp);
  printf("usertrap() function address: %d\n",myproc()->tf->kernel_trap);
  printf("user program counter: %d\n",myproc()->tf->epc);
  printf("kernel tp: %d\n",myproc()->tf->kernel_hartid);
  printf("ra: %d\n",myproc()->tf->ra);
  printf("sp: %d\n",myproc()->tf->sp);
  printf("gp: %d\n",myproc()->tf->gp);
  printf("tp: %d\n",myproc()->tf->tp);
 }
}

  // jump to trampoline.S at the top of memory, which 
  // switches to the user page table, restores user registers,
  // and switches to user mode with sret.
  uint64 fn = TRAMPOLINE + (userret - trampoline);
  ((void (*)(uint64,uint64))fn)(TRAPFRAME, satp);
}

// interrupts and exceptions from kernel code go here via kernelvec,
// on whatever the current kernel stack is.
// must be 4-byte aligned to fit in stvec.
void 
kerneltrap()
{
  int which_dev = 0;
  uint64 sepc = r_sepc();
  uint64 sstatus = r_sstatus();
  uint64 scause = r_scause();
  
  if((sstatus & SSTATUS_SPP) == 0)
    panic("kerneltrap: not from supervisor mode");
  if(intr_get() != 0)
    panic("kerneltrap: interrupts enabled");

  if((which_dev = devintr()) == 0){
    printf("scause %p\n", scause);
    printf("sepc=%p stval=%p\n", r_sepc(), r_stval());
    panic("kerneltrap");
  }

  // give up the CPU if this is a timer interrupt.
  if(which_dev == 2 && myproc() != 0 && myproc()->state == RUNNING)
    yield();

  // the yield() may have caused some traps to occur,
  // so restore trap registers for use by kernelvec.S's sepc instruction.
  w_sepc(sepc);
  w_sstatus(sstatus);
}

void
clockintr()
{
  acquire(&tickslock);
  ticks++;
  wakeup(&ticks);
  release(&tickslock);
}

// check if it's an external interrupt or software interrupt,
// and handle it.
// returns 2 if timer interrupt,
// 1 if other device,
// 0 if not recognized.
int
devintr()
{
  uint64 scause = r_scause();

  if((scause & 0x8000000000000000L) &&
     (scause & 0xff) == 9){
    // this is a supervisor external interrupt, via PLIC.

    // irq indicates which device interrupted.
    int irq = plic_claim();

    if(irq == UART0_IRQ){
      uartintr();
    } else if(irq == VIRTIO0_IRQ){
      virtio_disk_intr();
    }

    plic_complete(irq);
    return 1;
  } else if(scause == 0x8000000000000001L){
    // software interrupt from a machine-mode timer interrupt,
    // forwarded by timervec in kernelvec.S.

    if(cpuid() == 0){
      clockintr();
    }
    
    // acknowledge the software interrupt by clearing
    // the SSIP bit in sip.
    w_sip(r_sip() & ~2);

    return 2;
  } else {
    return 0;
  }
}

