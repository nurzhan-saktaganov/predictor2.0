#include "Range.hpp"

namespace dvmpredictor {
	Range::Range() : Range(0, 0) {};

	Range::Range(int64_t start, uint64_t count, bool forward) :
		_start(start), _count(count), _forward(forward)
	{}

	int64_t Range::start() const
	{
		return _start;
	}

	uint64_t Range::count() const
	{
		return _count;
	}

	bool Range::forward() const
	{
		return _forward;
	}
}
