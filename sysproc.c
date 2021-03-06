#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
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

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
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
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_getparentid(void)
{
  return getparentid();
}

int
sys_getSyscallCounter(void)
{
  int sysCallID;
  if(argint(0, &sysCallID) < 0)
    return -1;
  return myproc()->syscallCounter[sysCallID];
}
int 
sys_getchildren(void)
{
  int* p;
  int i;
  p=getchildren();

  for (i=0 ; i<63 && *(p + i)>-1 ; i++){
      cprintf("%d /", *(p + i));
  }
  cprintf("\n");
  return 1;
}
int
sys_setpriority(void){
  int priority;
  if(argint(0,&priority)<0)
    return -1;
  return setpriority(priority);
}

int
sys_getCBT(void){
  int *CBT;
  int *TT;
  int *WT;
  
 if (argptr(0, (void*)&CBT, sizeof(CBT)) < 0)
    return -1;
  if (argptr(1, (void*)&TT, sizeof(TT)) < 0)
    return -1;
  if (argptr(2, (void*)&WT, sizeof(WT)) < 0)
    return -1;
  return waitForProcessTime(CBT, TT, WT);
}
int
sys_changepolicy(void){
  int p;
  if(argint(0,&p)<0)
    return -1;
  return changepolicy(p);
}