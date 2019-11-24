#pragma once

#include <cstdint>

namespace dvmpredictor {
	class ShadowEdge {
	public:
		ShadowEdge();
		ShadowEdge(uint32_t width);
		ShadowEdge(uint32_t low_width, uint32_t hi_width);

		uint32_t low_width() const;
		uint32_t hi_width() const;
	private:
		uint32_t _low_width;
		uint32_t _hi_width;
	};
}
