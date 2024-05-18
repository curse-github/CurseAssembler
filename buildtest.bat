gcc -m32 -nostartfiles test.c -s -static -fno-exceptions -fno-asynchronous-unwind-tables -o test.exe
gcc -S -m32 -nostartfiles test.c -s -static -fno-exceptions -fno-asynchronous-unwind-tables -o test.s
test.exe
echo %ERRORLEVEL%