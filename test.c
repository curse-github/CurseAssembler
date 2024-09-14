#include <unistd.h>
int code=15;
int* codeptr=&code;
void _start (void) {
  _exit(*codeptr+2);
}