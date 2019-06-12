#pragma once

#include <cstdint>

namespace dvmpredictor {
	class Range {
	public:
		Range();
		Range(int32_t start, uint32_t count, bool forward = true);

		int32_t start() const;
		uint32_t count() const;
		bool forward() const;

	private:
		const int32_t _start;
		const uint32_t _count;
		const bool _forward;
	};
}
