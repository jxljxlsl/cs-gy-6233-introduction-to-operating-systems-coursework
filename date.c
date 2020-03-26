#include "types.h" 
#include "user.h" 
#include "date.h"

int
main(int argc, char *argv[])
{
  struct rtcdate r;
  if (date(&r)) {
    printf(2, "date failed\n");
  }
  if(r.second < 10) {
  	printf(1, "%d-%d-%d  %d:%d:0%d\n", r.year, r.month, r.day, r.hour, r.minute, r.second);
  } else {
  	printf(1, "%d-%d-%d  %d:%d:%d\n", r.year, r.month, r.day, r.hour, r.minute, r.second);
  }
  exit();
}