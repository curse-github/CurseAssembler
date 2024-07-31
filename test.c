#include <unistd.h>
int code=15;
void _start (void) {
  _exit(code);
}