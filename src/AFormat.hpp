#pragma once

#include <cstdint>

namespace dvmpredictor {
	class AFormat {
	public:
		AFormat();
		AFormat(int32_t a, int32_t b, uint32_t dimension);

		bool defined() const;

		int32_t a() const;
		int32_t b() const;
		uint32_t dimension() const;

		static const uint32_t dim_undefined = UINT32_MAX;
	private:
		const int32_t _a;
		const int32_t _b;
		const uint32_t _dimension;
	};
}
