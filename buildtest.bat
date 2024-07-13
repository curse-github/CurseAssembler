gcc -nostartfiles test.c -s -static -fno-exceptions -fno-asynchronous-unwind-tables -o test.exe
gcc -S -nostartfiles test.c -s -static -fno-exceptions -fno-asynchronous-unwind-tables -o test.s
C:\msys64\mingw32\bin\gcc.exe -static -static-libgcc -static-libstdc++ -m32 -nostartfiles test.c -s -fno-exceptions -fno-asynchronous-unwind-tables -o test32.exe
C:\msys64\mingw32\bin\gcc.exe -static -static-libgcc -static-libstdc++ -m32 -S -nostartfiles test.c -s -fno-exceptions -fno-asynchronous-unwind-tables -o test32.s
test.exe
echo %ERRORLEVEL%