#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;


  /*printf("process ID: %d\n",myproc()->pid);
  printf("contents of register a0 : %d\n", myproc()-> tf -> a0);
  printf("contents of register a1 : %d\n", myproc()-> tf -> a1);
  printf("contents of register a2 : %d\n", myproc()-> tf -> a2);
  printf("contents of register a3 : %d\n", myproc()-> tf -> a3);
  printf("contents of register a4 : %d\n", myproc()-> tf -> a4);
  printf("contents of register a5 : %d\n", myproc()-> tf -> a5);
  printf("contents of register a6 : %d\n", myproc()-> tf -> a6);

  printf("arguments of exit system call:- %d\n",n);*/

  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;

  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;

  
  printf("process ID: %d\n",myproc()->pid);
  printf("contents of register a0 : %d\n", myproc()-> tf -> a0);
  printf("contents of register a1 : %d\n", myproc()-> tf -> a1);
  printf("contents of register a2 : %d\n", myproc()-> tf -> a2);
  printf("contents of register a3 : %d\n", myproc()-> tf -> a3);
  printf("contents of register a4 : %d\n", myproc()-> tf -> a4);
  printf("contents of register a5 : %d\n", myproc()-> tf -> a5);
  printf("contents of register a6 : %d\n", myproc()-> tf -> a6);

  printf("arguments of sleep system call:- %d\n",n);


  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  //backtrace();
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}


uint64
sys_pcbread(void)
{
  
  printf("Process ID: %d\n",myproc()->pid);
  printf("Name of the Process: %s\n",myproc()->name);
  printf("killed: %d\n",myproc()->killed);
  printf("xstate: %d\n",myproc()->xstate);
  printf("TrapFrame: %p\n",myproc()->tf);
  printf("Current directory address: %p\n",myproc()->cwd);
  printf("Parent proc structure: %p\n",myproc()->parent);
  printf("size of the Process: %d\n",myproc()->sz);
  printf("Bottom of kernel stack: %d\n",myproc()->kstack);
  printf("PageTable address: %p\n",myproc()->pagetable);
  printf("Context of the process: %p\n",myproc()->context);
  printf("\n");

  return 0;
}

uint64
sys_testing(void)
{
  int temp1,temp2,temp3,temp4,temp5;

  if(argint(0, &temp1) < 0)
    return -1;
  
  if(argint(1, &temp2) < 0)
    return -1;

  if(argint(2, &temp3) < 0)
    return -1;

  if(argint(3, &temp4) < 0)
    return -1;

  if(argint(4, &temp5) < 0)
    return -1;

  printf("process ID: %d\n",myproc()->pid);
  printf("contents of register a0 : %d\n", myproc()-> tf -> a0);
  printf("contents of register a1 : %d\n", myproc()-> tf -> a1);
  printf("contents of register a2 : %d\n", myproc()-> tf -> a2);
  printf("contents of register a3 : %d\n", myproc()-> tf -> a3);
  printf("contents of register a4 : %d\n", myproc()-> tf -> a4);
  printf("contents of register a5 : %d\n", myproc()-> tf -> a5);
  printf("contents of register a6 : %d\n", myproc()-> tf -> a6);

  printf("arguments of testing system call:- %d %d %d %d %d\n",temp1,temp2,temp3,temp4,temp5);
  return 0;
}


// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
