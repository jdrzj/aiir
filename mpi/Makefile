mpi: mpi.cpp zhelpers.hpp task.o Attack.o CryptoUtils.o RainbowCracker.o
	mpiCC -o mpi mpi.cpp task.o Attack.o CryptoUtils.o RainbowCracker.o -lzmq -lssl -lcrypto -std=c++11

task.o: task.cpp task.hpp
	mpiCC -c task.cpp -std=c++11

Attack.o: Attack.cpp Attack.h
	mpiCC -c Attack.cpp -std=c++11

CryptoUtils.o: CryptoUtils.cpp CryptoUtils.h
	mpiCC -c CryptoUtils.cpp -std=c++11

RainbowCracker.o: RainbowCracker.cpp RainbowCracker.h
	mpiCC -c RainbowCracker.cpp -std=c++11
