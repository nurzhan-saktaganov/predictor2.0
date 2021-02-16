#include "AFormat.hpp"

#include "expect.hpp"

namespace dvmpredictor {
	AFormat::AFormat() : AFormat(0, 0, dim_undefined) {};

	AFormat::AFormat(int32_t b) : AFormat(0, b, dim_none) {};

	AFormat::AFormat(int32_t a, int32_t b, uint32_t dimension)
		: _a(a), _b(b), _dimension(dimension)
	{
		if (_dimension == dim_none)
			expect(_a == 0);

		if (_dimension == dim_all || _dimension == dim_undefined) {
			expect(_a == 0);
			expect(_b == 0);
		}
	}

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

	bool AFormat::is_constant() const
	{
		return _dimension == dim_none;
	}

	bool AFormat::is_asterisk() const
	{
		return _dimension == dim_all;
	}

	AFormat AFormat::asterisk()
	{
		return AFormat(0, 0, dim_all);
	}
}
