#pragma once

#include <cstdint>

namespace mpisimulator {
	class MPIImpl;

	class MPI {
	public:
		MPI();
		MPI(uint32_t size, double latency, double bandwidth);
		MPI(const MPI &mpi);
		MPI& operator=(const MPI &mpi);
		~MPI();

		void send(uint32_t at, uint32_t to, uint64_t bytes);
		void recv(uint32_t at, uint32_t from, uint64_t bytes);

		void skip(uint32_t at, double time);
		void barrier();
		double wtime(uint32_t at);

		void compact();
	private:
		MPIImpl *impl;
	};
}
