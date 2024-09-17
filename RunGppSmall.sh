gcc -nostartfiles test.c -s -static -fno-exceptions -fno-asynchronous-unwind-tables -o test.out
gcc -S -nostartfiles test.c -s -static -fno-exceptions -fno-asynchronous-unwind-tables -o test.s
./test.out
echo exit code: $?
pause