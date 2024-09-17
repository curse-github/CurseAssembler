#include <unistd.h>
int thing = 15;
int* ptr = NULL;
void _start (void) {
  ptr=&thing;
  _exit(*ptr);
}