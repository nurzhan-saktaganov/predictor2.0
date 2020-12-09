#pragma once

#include <cstdint>

namespace dvmpredictor {
	class Range {
	public:
		Range();
		Range(int64_t start, uint64_t count, bool forward = true);

		int64_t start() const;
		uint64_t count() const;
		bool forward() const;

	private:
		int64_t _start;
		uint64_t _count;
		bool _forward;
	};
}
