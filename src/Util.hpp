#pragma once

#include <vector>

#include "Shape.hpp"

namespace dvmpredictor {
	template <class T>
	void ensure(std::vector<T> &v, int size)
	{
		if (v.size() < size)
			v.resize(size);
	}

	static inline uint32_t volume(const Shape &sh)
	{
		uint32_t v = 1;
		for (auto &r: sh) v *= r.count();
		return v;
	}
}
