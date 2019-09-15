#include <cassert>
#include <stack>

#include "MPI.hpp"
#include "MPIImpl.hpp"

namespace mpisimulator {
	MPI::MPI() :
		impl(nullptr)
	{}

	MPI::MPI(uint32_t size, double latency, double bandwidth)
	{
		impl = new MPIImpl(size, latency, bandwidth);
	}

	MPI::MPI(const MPI &mpi) :
		impl(nullptr)
	{
		if (mpi.impl != nullptr) {
			impl = new MPIImpl;
			*impl = *mpi.impl;
		}
	}

	MPI& MPI::operator=(const MPI &mpi)
	{
		if (this == &mpi)
			return *this;

		if (mpi.impl == nullptr) {
			if (impl != nullptr)
				delete impl;
			impl = nullptr;
		} else {
			if (impl == nullptr)
				impl = new MPIImpl;
			*impl = *mpi.impl;
		}

		return *this;
	}

	MPI::~MPI()
	{
		if (impl != nullptr) delete impl;
	}

	void MPI::send(uint32_t at, uint32_t to, uint64_t bytes)
	{
		assert(impl != nullptr);

		impl->send(at, to, bytes);
	}

	void MPI::recv(uint32_t at, uint32_t from, uint64_t bytes)
	{
		assert(impl != nullptr);

		impl->recv(at, from, bytes);
	}

	void MPI::skip(uint32_t at, double time)
	{
		assert(impl != nullptr);

		impl->skip(at, time);
	}

	double MPI::wtime(uint32_t at)
	{
		assert(impl != nullptr);

		return impl->wtime(at);
	}

	void MPI::compact()
	{
		assert(impl != nullptr);

		impl->compact();
	}

	void MPI::barrier()
	{
		assert(impl != nullptr);

		impl->barrier();
	}
}
