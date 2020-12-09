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
		expect(_inited());

		for (Range r: shape) {
			expect(r.start() == 0);
			expect(r.count() > 0);
			expect(r.forward() == true);
		}

		_mpi = mpisimulator::MPI(_rank(), latency, bandwidth);
	}

	Template PGrid::declare_template(Shape shape)
	{
		expect(_inited());
		expect(volume(shape) > 0);

		must(_next_template_id != Template::id_undef);

		auto t = Template(_next_template_id++);
		_meta.save(t, shape);

		return t;
	}

	DArray PGrid::declare_darray(Shape shape, Shadow shadow, uint32_t elem_size)
	{
		expect(_inited());
		expect(volume(shape) > 0);
		expect(elem_size > 0);

		must(_next_darray_id != DArray::id_undef);

		auto a = DArray(_next_darray_id++);
		_meta.save(a, shape, shadow, elem_size);

		return a;
	}

	void PGrid::distribute(Template t, DRule rule)
	{
		expect(_is_declared(t));
		expect(!_is_distributed(t));

		uint32_t at = t.id();
		ensure(_templates_disposition, at + 1);

		auto shape = _meta.shape(t);

		_distribute(shape, rule, _templates_disposition[at]);
	}

	void PGrid::distribute(DArray a, DRule rule)
	{
		expect(_is_declared(a));
		expect(!_is_distributed(a));

		auto shape = _meta.shape(a);

		uint32_t at = a.id();
		ensure(_darrays_disposition, at + 1);

		_distribute(shape, rule, _darrays_disposition[at]);
	}

	// TODO
	void PGrid::redistribute(Template t, DRule rule)
	{
		expect(_is_declared(t));
		expect(_is_distributed(t));

		// TODO ask: can we redistribute template that aligned on something? Let's assume we can't.
		// assert(!_align_graph.is_aligned(t));

		// Perform nested distribution using Deepth-first search
	}

	// TODO
	void PGrid::redistribute(DArray a, DRule rule)
	{
		// TODO redistribute darray
	}

	// TODO
	void PGrid::align_with(DArray a, Template t, ARule rule)
	{
		expect(_is_declared(a));
		expect(!_is_aligned(a) && !_is_distributed(a));

		expect(_is_declared(t));
		expect(_is_distributed(t) || _is_aligned(t));

		uint32_t at = a.id();
		ensure(_darrays_disposition, at + 1);

		auto shape = _meta.shape(a);

		const Disposition &with = _templates_disposition[t.id()];

		_align(shape, with, rule, _darrays_disposition[at]);
	}

	// TODO
	void PGrid::align_with(DArray a, DArray b, ARule rule)
	{
		expect(_is_declared(a));
		expect(!_is_aligned(a) && !_is_distributed(a));

		expect(_is_declared(b));
		expect(_is_distributed(b) || _is_aligned(b));

		uint32_t at = a.id();
		ensure(_darrays_disposition, at + 1);

		auto shape = _meta.shape(a);

		const Disposition &with = _darrays_disposition[b.id()];

		_align(shape, with, rule, _darrays_disposition[at]);
	}

	// TODO
	void PGrid::realign_with(DArray a, Template t, ARule rule)
	{
		// TODO realign array on template
	}

	// TODO
	void PGrid::realign_with(DArray a, DArray b, ARule rule)
	{
		// TODO realign array on array
	}

	bool PGrid::_inited() const
	{
		return _rank() > 0;
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

	bool PGrid::_is_aligned(Template t) const
	{
		// TODO
		return false;
	}

	bool PGrid::_is_aligned(DArray a) const
	{
		// TODO
		return false;
	}

	uint64_t PGrid::_rank() const
	{
		return volume(_shape);
	}

	void PGrid::_distribute(const Shape &shape, const DRule &drule, Disposition &disposition) const
	{
		/* Идея такая. Для каждого распределяемого измерения массива строим разбиение
		на соответствующие куски согласно размерам измерения процессорной решетки, по которому оно распределяется.
		После чего идем по всем узлам процессорной решетки и для каждого узла получаем координаты.
		После чего определяем какой кускок распределенного массива попадет на данный узел.
		Для этого идем по правилам распределения массива. Если очередное правило говорит, что нужно
		размножить измерение, то берем весь размер соответствующего измерения. Если правило говорит, что нужно
		распределить измерение, то берем нужный кусок (определяется очередным компонентом координаты узла)
		из ранее построенного распределения.
		*/

		/*
			shape - форма распределяемого шаблона или массива.
			drule - правило распределения измерений.
			disposition - результат в виде размещения.
		*/

		expect(shape.size() == drule.size());

		// Здесь будет разбиения распределяемых измерений.
		std::vector<Ranges> flatten;

		// Получение разбиений распределяемых измерений.
		uint32_t pgrid_axis = 0;
		for (uint32_t shape_axis = 0; shape_axis < shape.size(); shape_axis++) {
			DFormat dformat = drule[shape_axis];

			if (!dformat.distributes())
				continue;

			Range range = shape[shape_axis];
			expect(pgrid_axis < _shape.size());
			uint32_t procs = _shape[pgrid_axis].count();

			Ranges ranges = dformat.distribute(range, procs);
			flatten.push_back(ranges);

			pgrid_axis++;
		}

		disposition.resize(_rank());
		for (uint32_t node_id = 0; node_id < _rank(); node_id++) {
			Shape local_shape(shape.size());	// форма локального куска
			Coord coord(_shape.size());			// координата узла процессорной решетки

			id2coord(_shape, node_id, coord);	// получили координату узла

			uint32_t pgrid_axis = 0;
			for (uint32_t shape_axis = 0; shape_axis < shape.size(); shape_axis++) {
				DFormat dformat = drule[shape_axis];

				if (!dformat.distributes()) {
					local_shape[shape_axis] = shape[shape_axis];
					continue;
				}

				uint32_t component = coord[pgrid_axis];			// индекс узла в нужном измерении
				expect(component < flatten[pgrid_axis].size());
				local_shape[shape_axis] = flatten[pgrid_axis][component];
				pgrid_axis++;
			}

			disposition[node_id] = local_shape;
		}
	}

	void PGrid::_align(const Shape &shape, const Disposition &with, const ARule &rule, Disposition &disposition) const
	{
		expect(with.size() == _rank());
		expect(rule.size() > 0);

		disposition.resize(_rank());
		for (uint32_t node_id = 0; node_id < _rank(); node_id++) {
			const Shape &local_template = with[node_id];
			Shape local_shape(shape.size());
			// std::vector<bool> seems to be zero initialized:
			// https://stackoverflow.com/questions/22983707/stdvectorbool-guaranteed-to-default-all-entries-to-false
			std::vector<bool> seen_axis(local_shape.size());
			bool no_local = false;

			// validate rule and template simultaneously
			expect(rule.size() == local_template.size());

			for (uint32_t template_axis = 0; template_axis < local_template.size(); template_axis++) {
				const AFormat &aformat = rule[template_axis];

				if (aformat.dimension() == aformat.dim_all)
					// It is the case when we have empty braces in T, e.g.:
					// #pragma dvm align A ([i] with T[i][])
					continue;

				int64_t template_left = local_template[template_axis].start();
				int64_t template_right = template_left + local_template[template_axis].count() - 1;

				expect(template_left <= template_right);

				if (aformat.a() == 0) {
					expect(aformat.dimension() == aformat.dim_none);
					// In this the case when we have constant in T, e.g.:
					// #pragma dvm align A ([i] with T[i][2])
					if (aformat.b() < template_left || template_right < aformat.b())
						no_local = true;
					continue;
				}

				uint32_t local_axis = aformat.dimension();

				expect(seen_axis[local_axis] == false);
				seen_axis[local_axis] = true;

				// division with round up
				int64_t local_left = (template_left + aformat.a() - 1 - aformat.b()) / aformat.a();
				// division with round down
				int64_t local_right = (template_right - aformat.b()) / aformat.a();

				int64_t global_left = shape[local_axis].start();
				int64_t global_right = global_left + shape[local_axis].count() - 1;

				expect(global_left <= local_left && local_right <= global_right);

				int64_t local_count = local_right - local_left + 1;

				local_shape[local_axis] = Range(local_left, local_count);
			}

			for (uint32_t local_axis = 0; local_axis < local_shape.size(); local_axis++) {
				if (no_local == false && seen_axis[local_axis] == false)
					// In othercase it has been initialized alredy by default constructor
					local_shape[local_axis] = shape[local_axis];
			}

			disposition[node_id] = local_shape;
		}
	}
}
