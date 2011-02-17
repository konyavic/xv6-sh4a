#include "stdio.h"
#include "assert.h"
#include "scif.h"
#include "stdarg.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

main(void)
{
int i;
//scif_init();
i = mknod("console", 1, 1);

open("console", O_RDWR);
  dup(0);  // stdout
  dup(0);  // stderr
printf(1, "i=\n");
printf(1, "init: starting sh\n");
  printf(1, "usertests starting\n");

//  if(open("usertests.ran", 0) >= 0){
//    printf(1, "already ran user tests -- rebuild fs.img\n");
//    exit();
//  }
//  close(open("usertests.ran", O_CREATE));
  //sbrktest();
//i = mknod("console", 1, 1);
//cprintf("i=%x\n", i);
while (1)
{;}
}
