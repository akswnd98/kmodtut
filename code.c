#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main() {
  int fd = open("/sys/class/kmodtut/kmodtut/kmodtut", O_RDWR);
  char buf[1005];
  write(fd, "kmj good", strlen("kmj good"));
  read(fd, buf, 1000);
  printf("%s\n", buf);
  close(fd);

  return 0;
}
