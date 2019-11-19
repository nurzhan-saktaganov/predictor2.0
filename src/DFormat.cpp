#include <cstdint>

#include "expect.hpp"
#include "DFormat.hpp"

namespace dvmpredictor {
	DFormat::DFormat()
		: _format(UNDEF)
	{}

	bool DFormat::distributes() const
	{
		expect(_format != UNDEF);

		return _format != NONE;
	}

	void DFormat::none()
	{
		expect(_format == UNDEF);

		_format = NONE;
	}

	void DFormat::block()
	{
		expect(_format == UNDEF);

		_format = BLOCK;
	}

	Ranges DFormat::distribute(Range range, uint32_t procs) const
	{
		expect(distributes());
		expect(range.count() > 0);
		expect(procs > 0);
		// BACKLOG: for non forward not implemented yet.
		expect(range.forward());

		expect(_format == BLOCK);
		// We pass normalized range
		auto ranges = _distribute_block(Range(0, range.count()), procs);

		// Restore initial shift
		int32_t shift = range.start();
		for (Range &range: ranges) {
			range = Range(range.start() + shift, range.count());
		}

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

		for (uint32_t i = 0; i < procs; ++i) {
			int32_t start = i * range.count() / procs;
			uint32_t count = (i + 1) * range.count() / procs - start;

			ranges[i] = Range(start, count);
		}

		return ranges;
	}
}
