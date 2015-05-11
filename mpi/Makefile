mpi: mpi.cpp zhelpers.hpp task.o
	mpiCC -o mpi mpi.cpp task.o -lzmq -std=c++11

task.o: task.cpp task.hpp
	mpiCC -c task.cpp -std=c++11

