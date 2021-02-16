#pragma once

#include <cstdint>

namespace dvmpredictor {
	class AFormat {
	public:
		// undefined/uninitialized
		AFormat();
		// constant b
		AFormat(int32_t b);
		// a * i + b
		AFormat(int32_t a, int32_t b, uint32_t dimension);

		bool defined() const;

		int32_t a() const;
		int32_t b() const;
		uint32_t dimension() const;

		bool is_constant() const;
		bool is_asterisk() const;

		// for empty braces, eg. with T[][a*I+j]
		static const uint32_t dim_all = UINT32_MAX;
		// default value, protection from uninitialized value
		static const uint32_t dim_undefined = UINT32_MAX - 1;
		// for constant expression, e.g. T[2][a*I+j]
		static const uint32_t dim_none = UINT32_MAX - 2;
		// returns new asterisk AFormat
		static AFormat asterisk();
	private:
		int32_t _a;
		int32_t _b;
		uint32_t _dimension;
	};
}
