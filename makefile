Win32: ./assembler.exe
	./assembler.exe Win32 small
Linux: ./assembler.out
	./assembler.out Linux small
	chmod 777 ./small.out
./assembler.exe: makeBin ./bin/globalConstants.o ./bin/ELF.o ./bin/PE.o ./bin/assembler.o
	g++ -g ./bin/globalConstants.o ./bin/ELF.o ./bin/PE.o ./bin/assembler.o -I ./src/constants -o ./assembler.exe
./assembler.out: makeBin ./bin/globalConstants.o ./bin/ELF.o ./bin/PE.o ./bin/assembler.o
	g++ -g ./bin/globalConstants.o ./bin/ELF.o ./bin/PE.o ./bin/assembler.o -I ./src/constants -o ./assembler.out
./bin/assembler.o: makeBin ./src/constants/globalConstants.h ./src/ELF.h ./src/PE.h ./src/assembler.cpp
	g++ -c -g ./src/assembler.cpp -I ./src/constants -o ./bin/assembler.o
./bin/PE.o: makeBin ./src/constants/peConstants.h ./src/constants/globalConstants.h ./src/PE.h ./src/PE.cpp
	g++ -c -g ./src/PE.cpp -I ./src/constants -o ./bin/PE.o
./bin/ELF.o: makeBin ./src/constants/elfConstants.h ./src/constants/globalConstants.h ./src/ELF.h ./src/ELF.cpp
	g++ -c -g ./src/ELF.cpp -I ./src/constants -o ./bin/ELF.o
./bin/globalConstants.o: makeBin ./src/constants/intelConstants.h ./src/constants/globalConstants.h ./src/constants/globalConstants.cpp
	g++ -c -g ./src/constants/globalConstants.cpp -I ./src/constants -o ./bin/globalConstants.o

makeBin:
	-mkdir bin
clean:
	-rm -rf ./bin
	-rm ./assembler.out
	-rm ./small.out
	-rm ./small.exe