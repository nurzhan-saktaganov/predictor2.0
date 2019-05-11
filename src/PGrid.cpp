#include <cassert>

#include "PGrid.hpp"

namespace dvmpredictor {
	PGrid::PGrid() : PGrid(Shape()) {}

	PGrid::PGrid(Shape shape)
		: _shape(shape), _next_template_id(0), _next_darray_id(0)
	{
		for (Range r: shape) {
			assert(r.start() == 0);
			assert(r.count() > 0);
			assert(r.forward() == true);
		}
	}

	Template PGrid::declare_template(Shape shape)
	{
		assert(_inited());
		assert(_next_template_id != Template::id_undef);
		// TODO store information about template

		return Template(_next_template_id++);
	}

	DArray PGrid::declare_darray(Shape shape, uint32_t elem_size)
	{
		assert(_inited());
		assert(_next_darray_id != DArray::id_undef);
		// TODO store information about darray

		return DArray(_next_darray_id++);
	}

	void PGrid::distribute(Template t, DRule rule)
	{
		assert(_is_declared(t));

		// TOOD
	}

	void PGrid::distribute(DArray a, DRule rule)
	{
		assert(_is_declared(a));

		// TODO
	}

	void PGrid::redistribute(Template t, DRule rule)
	{
		// TODO
	}

	void PGrid::redistribute(DArray a, DRule rule)
	{
		// TODO
	}

	void PGrid::align_on(DArray a, Template t, ARule rule)
	{
		// TODO
	}

	void PGrid::align_on(DArray a, DArray b, ARule rule)
	{
		// TODO
	}

	void PGrid::realign_on(DArray a, Template t, ARule rule)
	{
		// TODO
	}

	void PGrid::realign_on(DArray a, DArray b, ARule rule)
	{
		// TODO
	}

	// private methods
	Node PGrid::_node(Coord coord) const
	{
		assert(_inited());
		// TODO
		return Node();
	}

	bool PGrid::_inited() const
	{
		return _shape.size() > 0;
	}

	bool PGrid::_is_declared(Template t) const
	{
		return t.id() < _next_template_id;
	}

	bool PGrid::_is_declared(DArray a) const
	{
		return a.id() < _next_darray_id;
	}
}
