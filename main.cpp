#include <iostream>

#include "MPI.hpp"

#define ms * 0.001
#define Mbps * 1000000
#define Mb * 1000000

int main(int argc, const char *argv[])
{
	mpisimulator::MPI mpi(3, 10 ms, 10 Mbps);

	// What happens on node 0
	{
		mpi.send(0, 2, 100 Mb);
		mpi.skip(0, 10);
		mpi.recv(0, 1, 1 Mb);
	}

	// What happens on node 1
	{
		mpi.send(1, 0, 1 Mb);
	}

	// What happens on node 2
	{
		mpi.skip(2, 5);
		mpi.recv(2, 0, 100 Mb);
	}

	for (uint32_t node = 0; node < 3; node++) {
		std::cout<<"Node["<<node<<"] local time is: "<<mpi.wtime(node)<<std::endl;
	}

	return 0;
}
