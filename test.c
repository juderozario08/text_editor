#include <stdio.h>
#include <termios.h>

int main() {
  printf("%d\n", ~(ECHO));
  return 0;
}
