#pragma once

#include <vector>

namespace dvmpredictor {
	template <class T>
	void ensure(std::vector<T> &v, int size)
	{
		if (v.size() < size)
			v.resize(size);
	}
}
