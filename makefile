./small.out: ./assembler.out
	./assembler.out small
	chmod 777 ./small.out
./assembler.out: makeBin ./bin/globalConstants.o ./bin/elf.o ./bin/assembler.o
	g++ -g ./bin/globalConstants.o ./bin/elf.o ./bin/assembler.o -I./src/constants -o ./assembler.out
./bin/assembler.o: makeBin ./src/elf.h ./src/constants/intelConstants.h ./src/assembler.cpp
	g++ -c -g ./src/assembler.cpp -I./src/constants -o ./bin/assembler.o
./bin/elf.o: makeBin ./src/constants/elfConstants.h ./src/constants/globalConstants.h ./src/elf.h ./src/elf.cpp
	g++ -c -g ./src/elf.cpp -I./src/constants -o ./bin/elf.o
./bin/globalConstants.o: makeBin ./src/constants/globalConstants.cpp
	g++ -c -g ./src/constants/globalConstants.cpp -I./src/constants -o ./bin/globalConstants.o

makeBin:
	-mkdir bin
clean:
	-rm -rf ./bin
	-rm ./assembler.out
	-rm ./small.out