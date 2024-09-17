Win64: ./assembler.exe
	./assembler.exe Win64 small
Linux: ./assembler.out
	./assembler.out Linux small
	chmod 777 ./small.out
./assembler.exe: makeBin ./bin/globalConstants.o ./bin/intelConstants.o ./bin/ELF.o ./bin/PE.o ./bin/assembler.o
	g++ -g ./bin/globalConstants.o ./bin/intelConstants.o ./bin/ELF.o ./bin/PE.o ./bin/assembler.o -I ./src -I ./src/constants -o ./assembler.exe
./assembler.out: makeBin ./bin/globalConstants.o ./bin/intelConstants.o ./bin/ELF.o ./bin/PE.o ./bin/assembler.o
	g++ -g ./bin/globalConstants.o ./bin/intelConstants.o ./bin/ELF.o ./bin/PE.o ./bin/assembler.o -I ./src -I ./src/constants -o ./assembler.out
./bin/assembler.o: makeBin ./src/constants/globalConstants.h ./src/ELF.h ./src/PE.h ./src/assembler.cpp
	g++ -c -g ./src/assembler.cpp -I ./src -I ./src/constants -o ./bin/assembler.o

Win64Test: ./instructionTest.exe
	./instructionTest.exe Win64 small
LinuxTest: ./instructionTest.exe
	./instructionTest.exe Win64 small
./instructionTest.exe: makeBin ./bin/globalConstants.o ./bin/intelConstants.o ./bin/ELF.o ./bin/PE.o ./bin/instructionTest.o
	g++ -g ./bin/globalConstants.o ./bin/intelConstants.o ./bin/ELF.o ./bin/PE.o ./bin/instructionTest.o -I ./src -I ./src/constants -o ./instructionTest.exe
./instructionTest.out: makeBin ./bin/globalConstants.o ./bin/intelConstants.o ./bin/ELF.o ./bin/PE.o ./bin/instructionTest.o
	g++ -g ./bin/globalConstants.o ./bin/intelConstants.o ./bin/ELF.o ./bin/PE.o ./bin/instructionTest.o -I ./src -I ./src/constants -o ./instructionTest.out
./bin/instructionTest.o: makeBin ./src/constants/globalConstants.h ./src/ELF.h ./src/PE.h ./src/instructionTest.cpp
	g++ -c -g ./src/instructionTest.cpp -I ./src -I ./src/constants -o ./bin/instructionTest.o

./bin/PE.o: makeBin ./src/constants/peConstants.h ./src/constants/globalConstants.h ./src/PE.h ./src/PE.cpp
	g++ -c -g ./src/assembler.cpp -I ./src -I ./src/constants -o ./bin/assembler.o
./bin/PE.o: makeBin ./src/constants/peConstants.h ./src/constants/globalConstants.h ./src/PE.h ./src/PE.cpp
	g++ -c -g ./src/PE.cpp -I ./src -I ./src/constants -o ./bin/PE.o
./bin/ELF.o: makeBin ./src/constants/elfConstants.h ./src/constants/globalConstants.h ./src/ELF.h ./src/ELF.cpp
	g++ -c -g ./src/ELF.cpp -I ./src -I ./src/constants -o ./bin/ELF.o
./bin/globalConstants.o: makeBin ./src/constants/intelConstants.h ./src/constants/globalConstants.h ./src/constants/globalConstants.cpp
	g++ -c -g ./src/constants/globalConstants.cpp -I ./src -I ./src/constants -o ./bin/globalConstants.o
./bin/intelConstants.o: makeBin ./src/constants/intelConstants.h ./src/constants/intelConstants.cpp
	g++ -c -g ./src/constants/intelConstants.cpp -I ./src -I ./src/constants -o ./bin/intelConstants.o

makeBin:
	-mkdir bin
clean:
	@-rm -rf ./bin
	@-rm ./assembler.out
	@-rm ./instructionTest.out
	@-del bin /s /q
	@-rmdir bin
	@-del assembler.exe
	@-del instructionTest.exe