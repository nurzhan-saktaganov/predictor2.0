#include "Range.hpp"

namespace dvmpredictor {
	using namespace std;

	Range::Range() : Range(0, 0) {};

	Range::Range(int32_t start, uint32_t count, bool forward) :
		_start(start), _count(count), _forward(forward)
	{}

	int32_t Range::start() const
	{
		return _start;
	}

	uint32_t Range::count() const
	{
		return _count;
	}

	bool Range::forward() const
	{
		return _forward;
	}
}
