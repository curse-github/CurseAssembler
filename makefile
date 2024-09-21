Win64: ./assembler.exe ./FakeDll.dll
	./assembler.exe Win64 small
Linux: ./assembler.out ./FakeDll.so
	./assembler.out Linux small
	chmod 777 ./small.out

./assembler.exe: makeBin ./bin/assembler.o ./bin/globalConstants.o ./bin/PE.o ./bin/ELF.o
	g++ -g ./bin/assembler.o ./bin/globalConstants.o ./bin/PE.o ./bin/ELF.o -I ./src -I ./src/constants -o ./assembler.exe
./assembler.out: makeBin ./bin/assembler.o ./bin/globalConstants.o ./bin/PE.o ./bin/ELF.o
	g++ -g ./bin/assembler.o ./bin/globalConstants.o ./bin/PE.o ./bin/ELF.o -I ./src -I ./src/constants -o ./assembler.out
./bin/assembler.o: makeBin ./src/assembler.cpp ./src/constants/globalConstants.h ./src/PE.h ./src/constants/peConstants.h ./src/ELF.h ./src/constants/elfConstants.h
	g++ -c -g ./src/assembler.cpp -I ./src -I ./src/constants -o ./bin/assembler.o
./FakeDll.dll: makeBin ./src/FakeDll.c
	gcc -nostartfiles ./src/FakeDll.c -s -shared -fno-exceptions -fno-asynchronous-unwind-tables -o ./FakeDll.dll
./FakeDll.so: makeBin ./src/FakeDll.c
	gcc -nostartfiles ./src/FakeDll.c -s -shared -fno-exceptions -fno-asynchronous-unwind-tables -o ./FakeDll.so

./bin/PE.o: makeBin ./src/PE.cpp ./src/PE.h ./src/constants/peConstants.h ./src/constants/globalConstants.h
	g++ -c -g ./src/PE.cpp -I ./src -I ./src/constants -o ./bin/PE.o
./bin/ELF.o: makeBin ./src/ELF.cpp ./src/ELF.h ./src/constants/elfConstants.h ./src/constants/globalConstants.h 
	g++ -c -g ./src/ELF.cpp -I ./src -I ./src/constants -o ./bin/ELF.o
./bin/globalConstants.o: makeBin ./src/constants/globalConstants.cpp ./src/constants/globalConstants.h ./src/PE.h ./src/constants/peConstants.h ./src/ELF.h ./src/constants/elfConstants.h
	g++ -c -g ./src/constants/globalConstants.cpp -I ./src -I ./src/constants -o ./bin/globalConstants.o

makeBin:
	-mkdir bin
clean:
	@-rm -rf ./bin
	@-del bin /s /q
	@-rmdir bin
	@-del assembler.exe
	@-rm ./assembler.out