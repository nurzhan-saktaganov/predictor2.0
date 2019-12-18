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
		void barrier(uint32_t at);
		double wtime(uint32_t at);

	private:
		void _execute(uint32_t at);
		void _put(uint32_t at, struct operation op);

		uint32_t _size;
		double _latency;
		double _bandwidth;

		Nodes _nodes;
	};
}
