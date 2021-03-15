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

	// The manhattan distance
	static inline uint32_t coord_distance(const Coord &a, const Coord &b)
	{
		uint32_t distance = 0;

		expect(a.size() > 0);
		expect(a.size() == b.size());

		for (uint32_t i = 0; i < a.size(); i++) {
			// because they are uint32_t
			if (a[i] > b[i])
				distance += a[i] - b[i];
			else
				distance += b[i] - a[i];
		}

		return distance;
	}

	static void shape_intersect(const Shape &a, const Shape &b, Shape &out)
	{
		expect(out.size() > 0);
		expect(out.size() == a.size());
		expect(out.size() == b.size());

		for (uint32_t i = 0; i < out.size(); i++) {
			Range r1 = a[i];
			Range r2 = b[i];

			uint64_t start = std::max(r1.start(), r2.start());
			uint64_t end = std::min(r1.start() + r1.count(), r2.start() + r2.count());
			uint64_t count = 0;

			if (end > start)
				count = end - start;

			out[i] = Range(start, count);
		}
	}

	static inline bool shape_intersects(const Shape &a, const Shape &b)
	{
		Shape out(a.size());

		shape_intersect(a, b, out);

		return volume(out) > 0;
	}

	static inline bool shape_equal(const Shape &a, const Shape &b)
	{
		expect(a.size() > 0);
		expect(a.size() == b.size());

		for (uint32_t i = 0; i < a.size(); i++) {
			expect(a[i].count() > 0);

			if (a[i].start() != b[i].start())
				return false;

			if (a[i].count() != b[i].count())
				return false;

			if (a[i].forward() != b[i].forward())
				return false;
		}

		return true;
	}
}
