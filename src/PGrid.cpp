#include "expect.hpp"
#include "MPI.hpp"
#include "PGrid.hpp"
#include "Util.hpp"

namespace dvmpredictor {
	PGrid::PGrid() : PGrid(Shape(), 0, 0) {}

	PGrid::PGrid(Shape shape, double latency, double bandwidth)
		: _shape(shape), _next_template_id(0), _next_darray_id(0)
	{
		for (Range r: shape) {
			expect(r.start() == 0);
			expect(r.count() > 0);
			expect(r.forward() == true);
		}

		if (_inited())
			_mpi = mpisimulator::MPI(volume(shape), latency, bandwidth);
	}

	Template PGrid::declare_template(Shape shape)
	{
		expect(_inited());
		expect(_next_template_id != Template::id_undef);

		auto t = Template(_next_template_id++);
		_meta.of_template(t, shape);

		return t;
	}

	DArray PGrid::declare_darray(Shape shape, Shadow shadow, uint32_t elem_size)
	{
		expect(_inited());
		expect(_next_darray_id != DArray::id_undef);

		auto a = DArray(_next_darray_id++);
		_meta.of_darray(a, shape, shadow, elem_size);

		return a;
	}

	void PGrid::distribute(Template t, DRule rule)
	{
		expect(_is_declared(t));
		auto shape = _meta.shape(t);
		auto dispositions = _distribute(shape, rule);
		_distribution.dispose(t, dispositions);
	}

	void PGrid::distribute(DArray a, DRule rule)
	{
		expect(_is_declared(a));
		auto shape = _meta.shape(a);
		auto dispositions = _distribute(shape, rule);
		_distribution.dispose(a, dispositions);
	}

	void PGrid::redistribute(Template t, DRule rule)
	{
		expect(_is_declared(t));
		expect(_is_distributed(t));

		// TODO ask: can we redistribute template that aligned on something? Let's assume we can't.
		// assert(!_align_graph.is_aligned(t));

		// Perform nested distribution using Deepth-first search
	}

	void PGrid::redistribute(DArray a, DRule rule)
	{
		// TODO redistribute darray
	}

	void PGrid::align_on(DArray a, Template t, ARule rule)
	{
		expect(_is_declared(a));
		// assert(!_align_graph.is_aligned(a));

		expect(_is_declared(t));
		expect(_is_distributed(t));

		// _align_graph.align(a, t);

		// TODO align array on template
	}

	void PGrid::align_on(DArray a, DArray b, ARule rule)
	{
		// TODO align array on array
	}

	void PGrid::realign_on(DArray a, Template t, ARule rule)
	{
		// TODO realign array on template
	}

	void PGrid::realign_on(DArray a, DArray b, ARule rule)
	{
		// TODO realign array on array
	}

	// private methods
	Node PGrid::_node(Coord coord) const
	{
		must(_inited());
		must(_shape.size() == coord.size());

		// imagine `_shape' as a n-dimension rectangle,
		// we just need enumerate vertexes in this rectangle.
		uint32_t id = 0;
		uint32_t order = 1;
		for (int i = coord.size() - 1; i >= 0; i--) {
			id += coord[i] * order;
			order *= _shape[i].count();
		}

		auto n = Node(id);
		must(n.defined());
		return n;
	}

	// PGrid has a processor set with the given shape.
	// In this processor set every node has it's own id.
	Coord PGrid::_coord(Node n) const
	{
		must(_inited());
		must(n.defined());

		Coord coord(_shape.size(), 0);

		uint32_t order = volume(_shape);
		uint32_t id = n.id();
		for (std::size_t i = 0; i < coord.size(); i++) {
			order /= _shape[i].count();
			coord[i] = id / order;
			id = id - coord[i] * order;
		}

		return coord;
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

	bool PGrid::_is_distributed(Template t) const
	{
		// TODO _is_distributed for template
		return false;
	}

	bool PGrid::_is_distributed(DArray a) const
	{
		// TODO _is_distributed for DArray
		return false;
	}

	Dispositions PGrid::_distribute(Shape sh, DRule rule) const
	{
		// TODO We need distribution format for each dimension of pgrid
		must(_shape.size() == rule.size());

		for (uint32_t dim = 0; dim < rule.size(); dim++) {
			auto dformat = rule[dim];

			// TODO _distribute
			(void)dformat;
		}

		Dispositions d;
		return d;
	}

	Dispositions PGrid::_align(Shape sh, ARule rule) const
	{
		// TODO _align shape on arule

		Dispositions d;
		return d;
	}

	void PGrid::_redispose(Dispositions before, Dispositions after)
	{
		// TODO _redispose
	}
}
