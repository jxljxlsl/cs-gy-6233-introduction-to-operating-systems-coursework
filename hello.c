#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"


// int main(void) {
//     printf(1, "Hello world\n");
//     exit();
// }

int
main(int argc, char *argv[])
{
  int oldfd = open("tricky.txt", O_CREATE|O_WRONLY);
  int newfd = 10;
  printf(0,"oldfd = %d\n",oldfd);

  for (newfd =0;newfd<10;newfd++){
  printf(0,"newfd = %d\n",newfd);
  if (dup2(oldfd,newfd) == -1)
        printf(0,"%s\n"," the newfd is already taken or oldfd is not allocated");
  else
        printf(0,"%s\n","success" );
}
  exit();
}