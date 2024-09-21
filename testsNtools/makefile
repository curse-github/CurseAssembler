buildTestsWin: ./testInstructions.exe ./testParsePeFile.exe ./testParseElfFile.exe
	@echo building finished
buildTestsLinux: ./testInstructions.out ./testParsePeFile.out ./testParseElfFile.out
	chmod 777 ./testInstructions.out
	chmod 777 ./testParsePeFile.out
	chmod 777 ./testParseElfFile.out
	@echo building finished

./testInstructions.exe: makeBin ./bin/testInstructions.o ./bin/globalConstants.o ./bin/PE.o ./bin/ELF.o
	g++ -g ./bin/testInstructions.o ./bin/globalConstants.o ./bin/PE.o ./bin/ELF.o -I ../src -I ../src/constants -o ./testInstructions.exe
./testInstructions.out: makeBin ./bin/testInstructions.o ./bin/globalConstants.o ./bin/PE.o ./bin/ELF.o
	g++ -g ./bin/testInstructions.o ./bin/globalConstants.o ./bin/PE.o ./bin/ELF.o -I ../src -I ../src/constants -o ./testInstructions.out
./bin/testInstructions.o: makeBin ./testInstructions.cpp ../src/constants/globalConstants.h ../src/PE.h ../src/ELF.h
	g++ -c -g ./testInstructions.cpp -I ../src -I ../src/constants -o ./bin/testInstructions.o

./testParsePeFile.exe: makeBin ./bin/testParsePeFile.o ./bin/globalConstants.o ./bin/PE.o ./bin/ELF.o
	g++ -g ./bin/testParsePeFile.o ./bin/globalConstants.o ./bin/PE.o ./bin/ELF.o -I ../src -I ../src/constants -o ./testParsePeFile.exe
./testParsePeFile.out: makeBin ./bin/testParsePeFile.o ./bin/globalConstants.o ./bin/PE.o ./bin/ELF.o
	g++ -g ./bin/testParsePeFile.o ./bin/globalConstants.o ./bin/PE.o ./bin/ELF.o -I ../src -I ../src/constants -o ./testParsePeFile.out
./bin/testParsePeFile.o: makeBin ./testParsePeFile.cpp ../src/constants/globalConstants.h ../src/PE.h ../src/ELF.h
	g++ -c -g ./testParsePeFile.cpp -I ../src -I ../src/constants -o ./bin/testParsePeFile.o

./testParseElfFile.exe: makeBin ./bin/testParseElfFile.o ./bin/globalConstants.o ./bin/PE.o ./bin/ELF.o
	g++ -g ./bin/testParseElfFile.o ./bin/globalConstants.o ./bin/PE.o ./bin/ELF.o -I ../src -I ../src/constants -o ./testParseElfFile.exe
./testParseElfFile.out: makeBin ./bin/testParseElfFile.o ./bin/globalConstants.o ./bin/PE.o ./bin/ELF.o
	g++ -g ./bin/testParseElfFile.o ./bin/globalConstants.o ./bin/PE.o ./bin/ELF.o -I ../src -I ../src/constants -o ./testParseElfFile.out
./bin/testParseElfFile.o: makeBin ./testParseElfFile.cpp ../src/constants/globalConstants.h ../src/PE.h ../src/ELF.h
	g++ -c -g ./testParseElfFile.cpp -I ../src -I ../src/constants -o ./bin/testParseElfFile.o

./bin/PE.o: makeBin ../src/PE.cpp ../src/PE.h ../src/constants/peConstants.h ../src/constants/globalConstants.h
	g++ -c -g ../src/PE.cpp -I ../src -I ../src/constants -o ./bin/PE.o
./bin/ELF.o: makeBin ../src/ELF.cpp ../src/ELF.h ../src/constants/elfConstants.h ../src/constants/globalConstants.h 
	g++ -c -g ../src/ELF.cpp -I ../src -I ../src/constants -o ./bin/ELF.o
./bin/globalConstants.o: makeBin ../src/constants/globalConstants.cpp ../src/constants/globalConstants.h ../src/PE.h ../src/constants/peConstants.h ../src/ELF.h ../src/constants/elfConstants.h
	g++ -c -g ../src/constants/globalConstants.cpp -I ../src -I ../src/constants -o ./bin/globalConstants.o

makeBin:
	-mkdir bin
clean:
	@-rm -rf ./bin
	@-del bin /s /q
	@-rmdir bin
	@-del testInstructions.exe
	@-del testParsePeFile.exe
	@-del testParseElfFile.exe
	@-rm ./testInstructions.out
	@-rm ./testParsePeFile.out
	@-rm ./testParseElfFile.out