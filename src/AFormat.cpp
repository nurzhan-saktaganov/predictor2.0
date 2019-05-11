#include "AFormat.hpp"

namespace dvmpredictor {
	AFormat::AFormat() : AFormat(0, 0, dim_undefined) {};

	AFormat::AFormat(int32_t a, int32_t b, uint32_t dimension)
		: _a(a), _b(b), _dimension(dimension)
	{}

	bool AFormat::defined() const
	{
		return _dimension != dim_undefined;
	}

	int32_t AFormat::a() const
	{
		return _a;
	}

	int32_t AFormat::b() const
	{
		return _b;
	}

	uint32_t AFormat::dimension() const
	{
		return _dimension;
	}
}
