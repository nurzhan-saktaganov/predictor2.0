#include <cassert>
#include <cstdint>

#include "DFormat.hpp"

namespace dvmpredictor {
	using namespace std;

	DFormat::DFormat()
		: _format(UNDEF)
	{}

	bool DFormat::distributes() const
	{
		assert(_format != UNDEF);

		return _format != NONE;
	}

	void DFormat::none()
	{
		assert(_format == UNDEF);

		_format = NONE;
	}

	void DFormat::block()
	{
		assert(_format == UNDEF);

		_format = BLOCK;
	}

	Ranges DFormat::distribute(Range range, uint32_t procs) const
	{
		assert(distributes());
		assert(range.count() > 0);
		assert(procs > 0);
		// TODO: for non forward not implemented yet.
		assert(range.forward());

		// We pass normalized range
		auto ranges = _distribute_block(Range(0, range.count()), procs);

		// Restore initial shift
		int32_t shift = range.start();
		for (Range &range: ranges)
			range = Range(range.start() + shift, range.count());

		return ranges;
	}

	Ranges DFormat::_distribute_block(Range range, uint32_t procs) const
	{
		Ranges ranges(procs);

		if (range.count() < procs) {
			for (uint32_t i = 0; i < range.count(); ++i) {
				ranges[i] = Range(i, 1);
			}

			return ranges;
		}

		int32_t start = 0;
		for (uint32_t i = 0; i < procs; ++i) {
			uint32_t count = (i + 1) * range.count() / procs - i * range.count() / procs;

			ranges[i] = Range(start, count);

			start += count;
		}

		return ranges;
	}
}