#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void main(int argc, char *argv[])
{
  int i = 0;
  printf("\n%s ",argv[0]);
  printf("Программа test запущена и получила строку\n");
  while(argv[++i] != NULL)
  printf("%s ",argv[i]);
  exit(EXIT_SUCCESS);
}
