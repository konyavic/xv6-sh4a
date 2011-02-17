#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int i;
  //i = argc;
//printf(1, "i=%x\n", i);
  //for(i = 1; i < 2; i++)
   // printf(1, "argv%s", argv[i]);
  for(i = 1; i < argc; i++)
    printf(1, "%s%s", argv[i], i+1 < argc ? " " : "\n");
 //printf(1, "%s%s", argv[1], 2 < argc ? " " : "\n");
  exit();
}
