#pragma once

#include <vector>

#include "Coord.hpp"
#include "Shape.hpp"


namespace dvmpredictor {
	template <class T>
	static inline void ensure(std::vector<T> &v, std::size_t size)
	{
		if (v.size() < size)
			v.resize(size);
	}

	static inline uint32_t volume(const Shape &sh)
	{
		uint32_t v = sh.size() > 0 ? 1 : 0;
		for (auto &r: sh) v *= r.count();
		return v;
	}

	static inline uint32_t coord2id(const Shape &shape, const Coord &coord)
	{
		expect(volume(shape) > 0);
		expect(coord.size() == shape.size());

		// imagine `shape' as a n-dimension grid,
		// we just need enumerate vertices of this grid.

		uint32_t id = 0;
		uint32_t multiplier = 1;
		for (uint32_t axis = 0; axis < coord.size(); axis++) {
			expect(coord[axis] < shape[axis].count());

			id += coord[axis] * multiplier;
			multiplier *= shape[axis].count();
		}

		return id;
	}

	static inline void id2coord(const Shape &shape, uint32_t id, Coord &coord)
	{
		expect(volume(shape) > 0);
		expect(coord.size() == shape.size());
		expect(id < volume(shape));

		for (uint32_t axis = 0; axis < coord.size(); axis++) {
			uint32_t divider = shape[axis].count();
			coord[axis] = id % divider;
			id /= divider;
		}
	}
}
