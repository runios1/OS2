// Sleeping locks

#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sleeplock.h"

void initsleeplock(struct sleeplock *lk, char *name)
{
  initlock(&lk->lk, "sleep lock");
  lk->name = name;
  lk->locked = 0;
  lk->pid = 0;
  lk->isDestroyed = 0;
}

void acquiresleep(struct sleeplock *lk)
{
//  if (lk->name[0] == 'c')
//     printf("Got lock of %s\n", lk->name);

  acquire(&lk->lk);
  while (lk->locked)
  {
    sleep(lk, &lk->lk);
    if(lk->isDestroyed){
      release(&lk->lk);
      return;
    }
  }
  lk->locked = 1;
  lk->pid = myproc()->pid;
  release(&lk->lk);
}

void releasesleep(struct sleeplock *lk)
{
  acquire(&lk->lk);
  lk->locked = 0;
  lk->pid = 0;
  wakeup(lk);
  release(&lk->lk);
}

int holdingsleep(struct sleeplock *lk)
{
  int r;

  acquire(&lk->lk);
  r = lk->locked && (lk->pid == myproc()->pid);
  release(&lk->lk);
  return r;
}

void destroySleep(struct sleeplock *lk){
  acquire(&lk->lk);
  lk->isDestroyed = 1;
  release(&lk->lk);
  releasesleep(lk);
}