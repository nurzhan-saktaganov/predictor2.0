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
		expect(range.count() > 0);
		expect(procs > 0);

		Ranges ranges;

		// BACKLOG: for non forward not implemented yet.
		expect(range.forward());

		int64_t shift = range.start();
		// We pass normalized range
		range = Range(0, range.count());

		switch (_format) {
		case NONE:
			ranges = _distribute_none(range, procs);
			break;
		case BLOCK:
			ranges = _distribute_block(range, procs);
			break;
		case MULT_BLOCK:
			// fallthrough
		case WGT_BLOCK:
			// fallthrough
		case GEN_BLOCK:
			// fallthrough
		default:
			expect(false && "unsupported format");
		}

		// Restore initial shift
		for (Range &range: ranges) {
			range = Range(range.start() + shift, range.count());
		}

		return ranges;
	}

	Ranges DFormat::_distribute_none(Range range, uint32_t procs) const
	{
		Ranges ranges(procs);

		for (Range &r: ranges) {
			r = range;
		}

		return ranges;
	}

	Ranges DFormat::_distribute_block(Range range, uint32_t procs) const
	{
		Ranges ranges(procs);

		if (range.count() < procs) {
			for (uint64_t i = 0; i < range.count(); ++i) {
				ranges[i] = Range(i, 1);
			}

			return ranges;
		}

		for (uint32_t i = 0; i < procs; ++i) {
			int64_t start = i * range.count() / procs;
			uint64_t count = (i + 1) * range.count() / procs - start;

			ranges[i] = Range(start, count);
		}

		return ranges;
	}
}
