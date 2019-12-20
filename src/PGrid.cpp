#include "expect.hpp"
#include "MPI.hpp"
#include "PGrid.hpp"
#include "Util.hpp"
#include "SubShapes.hpp"

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

		// BACKLOG: add shape validation

		auto t = Template(_next_template_id++);
		_meta.save(t, shape);

		return t;
	}

	DArray PGrid::declare_darray(Shape shape, Shadow shadow, uint32_t elem_size)
	{
		expect(_inited());
		expect(_next_darray_id != DArray::id_undef);

		expect(elem_size > 0);

		// BACKLOG: add shape, shadow validation

		auto a = DArray(_next_darray_id++);
		_meta.save(a, shape, shadow, elem_size);

		return a;
	}

	void PGrid::distribute(Template t, DRule rule)
	{
		expect(_is_declared(t));
		expect(!_is_distributed(t));

		auto shape = _meta.shape(t);
		auto dispositions = _distribute(shape, rule);
		_distribution.dispose(t, dispositions);
	}

	void PGrid::distribute(DArray a, DRule rule)
	{
		expect(_is_declared(a));
		expect(!_is_distributed(a));

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

	Node PGrid::_node(Coord coord) const
	{
		expect(_inited());
		expect(_shape.size() == coord.size());

		// imagine `_shape' as a n-dimension rectangle,
		// we just need enumerate vertexes of this rectangle.

		uint32_t sum = 0;
		uint32_t multiplier = 1;
		for (uint32_t component = 0; component < coord.size(); component++) {
			sum += coord[component] * multiplier;
			multiplier *= _shape[component].count();
		}

		Node node(sum);
		must(node.defined());

		return node;
	}

	// PGrid has a processor set with the given shape.
	// In this processor set every node has it's own id.
	Coord PGrid::_coord(Node node) const
	{
		expect(_inited());
		expect(node.defined());

		Coord coord(_shape.size());

		uint32_t sum = node.id();
		for (uint32_t component = 0; component < coord.size(); component++) {
			uint32_t divider = _shape[component].count();
			coord[component] = sum % divider;
			sum /= divider;
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
		// We need distribution format for each dimensions of shape
		expect(sh.size() == rule.size());

		uint32_t nodes_cnt = volume(_shape);

		SubShapes sub_shapes(nodes_cnt);

		uint32_t distribute_on_dim = 0;
		for (uint32_t dim = 0; dim < rule.size(); dim++) {
			DFormat dformat = rule[dim];
			Range range = sh[dim];

			expect(distribute_on_dim < _shape.size());
			uint32_t procs = _shape[distribute_on_dim].count();
			Ranges ranges = dformat.distribute(range, procs);

			for (uint32_t node_id = 0; node_id < nodes_cnt; node_id++) {
				Node node(node_id);
				Coord coord = _coord(node);

				expect(coord[distribute_on_dim] < _shape[distribute_on_dim].count());

				int32_t at = coord[distribute_on_dim];

				Range range = ranges[at];

				sub_shapes[node_id].push_back(range);
			}

			if (dformat.distributes())
				distribute_on_dim++;
		}

		Dispositions dispositions;

		for (uint32_t node_id = 0; node_id < nodes_cnt; node_id++) {
			dispositions[node_id] = Disposition(Node(node_id), sub_shapes[node_id]);
		}

		return dispositions;
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
