gcc -nostartfiles test.c -s -static -fno-exceptions -fno-asynchronous-unwind-tables -o test.exe
gcc -S -nostartfiles test.c -s -static -fno-exceptions -fno-asynchronous-unwind-tables -o test.s
test.exe
echo %ERRORLEVEL%
pause