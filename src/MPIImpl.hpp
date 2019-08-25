#pragma once

#include <cstdint>

#include "Nodes.hpp"

namespace mpisimulator {
	class MPIImpl {
	public:
		MPIImpl();
		MPIImpl(uint32_t size, double latency, double bandwidth);

		void send(uint32_t at, uint32_t to, uint64_t bytes);
		void recv(uint32_t at, uint32_t from, uint64_t bytes);

		void skip(uint32_t at, double time);
		double wtime(uint32_t at);

		void compact();

	private:
		uint32_t _size;
		double _latency;
		double _bandwidth;

		Nodes _nodes;
	};
}
