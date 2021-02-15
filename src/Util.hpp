#pragma once

#include <vector>

#include "expect.hpp"

#include "Coord.hpp"
#include "Shape.hpp"


namespace dvmpredictor {
	template <class T>
	static inline void ensure(std::vector<T> &v, std::size_t size)
	{
		if (v.size() < size)
			v.resize(size);
	}

	static inline uint64_t volume(const Shape &sh)
	{
		uint64_t v = sh.size() > 0 ? 1 : 0;
		for (auto &r: sh) v *= r.count();
		return v;
	}

	// See about coord2id and id2coord here:
	// https://keldysh.ru/dvm/dvmhtm1107/rus/sys/libdvm/rtsIDr1.html

	static inline uint32_t coord2id(const Shape &shape, const Coord &coord)
	{
		expect(volume(shape) > 0);
		expect(coord.size() == shape.size());

		// imagine `shape' as a n-dimension grid,
		// we just need enumerate vertices of this grid.

		uint32_t id = 0;
		uint32_t multiplier = 1;

		for (uint32_t i = 0; i < coord.size(); i++) {
			// The first axis is the most significant, and the last axis is the least significant.
			// So we start from the last axis.
			uint32_t axis = coord.size() - i - 1;

			expect(coord[axis] < shape[axis].count());

			id += coord[axis] * multiplier;
			multiplier *= shape[axis].count();	// update multiplier for the next axis
		}

		return id;
	}

	static inline void id2coord(const Shape &shape, uint32_t id, Coord &coord)
	{
		expect(volume(shape) > 0);
		expect(coord.size() == shape.size());
		expect(id < volume(shape));

		for (uint32_t i = 0; i < coord.size(); i++) {
			uint32_t axis = coord.size() - i - 1;	// starting from the least significant axis
			uint32_t divider = shape[axis].count();
			coord[axis] = id % divider;
			id /= divider;
		}
	}
}
