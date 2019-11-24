#include "ShadowEdge.hpp"

namespace dvmpredictor {
	static const uint32_t default_width = 1;

	ShadowEdge::ShadowEdge()
		: ShadowEdge(default_width, default_width)
	{}

	ShadowEdge::ShadowEdge(uint32_t width)
		: ShadowEdge(width, width)
	{}

	ShadowEdge::ShadowEdge(uint32_t low_width, uint32_t hi_width)
		: _low_width(low_width), _hi_width(hi_width)
	{}

	uint32_t ShadowEdge::low_width() const
	{
		return _low_width;
	}

	uint32_t ShadowEdge::hi_width() const
	{
		return _hi_width;
	}
}
