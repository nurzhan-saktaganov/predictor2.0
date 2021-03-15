#include <cassert>

#include "expect.hpp"
#include "MPI.hpp"
#include "PGrid.hpp"
#include "Util.hpp"

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

	uint32_t PGrid::rank() const
	{
		return volume(_shape);
	}

	double PGrid::wtime(uint32_t node) const
	{
		return _mpi.wtime(node);
	}

	Template PGrid::declare_template(Shape shape)
	{
		expect(_inited());
		expect(volume(shape) > 0);

		must(_next_template_id != Template::id_undef);

		auto t = Template(_next_template_id++);
		uint32_t at = t.id();

		ensure(_templates_meta, at + 1);

		_templates_meta[at] = Meta(t, shape);

		return t;
	}

	DArray PGrid::declare_darray(Shape shape, Shadow shadow, uint32_t elem_size)
	{
		expect(_inited());
		expect(volume(shape) > 0);
		expect(elem_size > 0);

		must(_next_darray_id != DArray::id_undef);

		auto d = DArray(_next_darray_id++);
		uint32_t at = d.id();

		ensure(_darrays_meta, at + 1);

		_darrays_meta[at] = Meta(d, shape, shadow, elem_size);

		return d;
	}

	void PGrid::distribute(Template t, DRule rule)
	{
		expect(_is_declared(t));
		expect(!_is_distributed(t));

		uint32_t at = t.id();

		_templates_meta[at].type = Meta::DISTRIBUTABLE;

		Shape &shape = _templates_meta[at].shape;
		_distribute(shape, rule, _templates_meta[at].disposition);
	}

	void PGrid::distribute(DArray a, DRule rule)
	{
		expect(_is_declared(a));
		expect(!_is_distributed(a));

		uint32_t at = a.id();

		_darrays_meta[at].type = Meta::DISTRIBUTABLE;

		Shape &shape = _darrays_meta[at].shape;
		_distribute(shape, rule, _darrays_meta[at].disposition);
	}

	void PGrid::redistribute(Template t, DRule rule)
	{
		expect(_is_declared(t));
		expect(_is_distributable(t));

		uint32_t at = t.id();

		Shape &shape = _templates_meta[at].shape;

		Disposition before = _templates_meta[at].disposition;

		_distribute(shape, rule, _templates_meta[at].disposition);

		// XXX: We don't send anyting on template redistribute
		// _redistribute(before, _darrays_meta[at].disposition, 0, _mpi);

		// nested redistribute
		for (uint32_t i = 0; i < _darrays_meta.size(); i++) {
			const Meta &meta = _darrays_meta[i];
			DArray darray = meta.darray.d;

			if (!_is_alignable(darray))
				continue;

			if (!meta.aligner.is_template)
				continue;

			if (meta.aligner.templ != t)
				continue;

			// realign every aligned on this root thing on this root again
			realign_with(darray, t, meta.aligner.rule);
		}
	}

	void PGrid::redistribute(DArray a, DRule rule)
	{
		expect(_is_declared(a));
		expect(_is_distributable(a));

		uint32_t at = a.id();
		uint32_t elem_size = _darrays_meta[at].darray.elem_size;

		Shape &shape = _darrays_meta[at].shape;

		Disposition before = _darrays_meta[at].disposition;

		_distribute(shape, rule, _darrays_meta[at].disposition);

		_redistribute(before, _darrays_meta[at].disposition, elem_size, _mpi);

		// nested redistribute
		for (uint32_t i = 0; i < _darrays_meta.size(); i++) {
			const Meta &meta = _darrays_meta[i];
			DArray darray = meta.darray.d;

			if (!_is_alignable(darray))
				continue;

			if (meta.aligner.is_template)
				continue;

			if (meta.aligner.darray != a)
				continue;

			// realign every aligned on this root thing on this root again
			realign_with(darray, a, meta.aligner.rule);
		}
	}

	void PGrid::_root_aligner(Meta &meta, Template via, ARule rule) const
	{
		if (_is_alignable(via)) {
			uint32_t at = via.id();

			ARule rule2, out;

			rule2 = _templates_meta[at].aligner.rule;

			combine_arule(rule, rule2, out);

			if (_templates_meta[at].aligner.is_template) {
				Template root = _templates_meta[at].aligner.templ;

				return _root_aligner(meta, root, out);
			}

			DArray root = _templates_meta[at].aligner.darray;

			return _root_aligner(meta, root, out);
		}

		meta.aligner.is_template = true;
		meta.aligner.templ = via;
		meta.aligner.rule = rule;
	}

	void PGrid::_root_aligner(Meta &meta, DArray via, ARule rule) const
	{
		if (_is_alignable(via)) {
			uint32_t at = via.id();

			ARule rule2, out;

			rule2 = _darrays_meta[at].aligner.rule;

			combine_arule(rule, rule2, out);

			if (_darrays_meta[at].aligner.is_template) {
				Template root = _darrays_meta[at].aligner.templ;

				return _root_aligner(meta, root, out);
			}

			DArray root = _darrays_meta[at].aligner.darray;

			return _root_aligner(meta, root, out);
		}

		meta.aligner.is_template = false;
		meta.aligner.darray = via;
		meta.aligner.rule = rule;
	}

	void PGrid::align_with(DArray a, Template t, ARule rule)
	{
		expect(_is_declared(a));
		expect(!_is_distributed(a));

		expect(_is_declared(t));
		expect(_is_distributed(t));

		uint32_t at = a.id();

		_darrays_meta[at].type = Meta::ALIGNABLE;

		_root_aligner(_darrays_meta[at], t, rule);

		Shape &shape = _darrays_meta[at].shape;
		const Disposition &with = _templates_meta[t.id()].disposition;

		_align(shape, with, rule, _darrays_meta[at].disposition);
	}

	void PGrid::align_with(DArray a, DArray b, ARule rule)
	{
		expect(_is_declared(a));
		expect(!_is_distributed(a));

		expect(_is_declared(b));
		expect(_is_distributed(b));

		uint32_t at = a.id();

		_darrays_meta[at].type = Meta::ALIGNABLE;

		_root_aligner(_darrays_meta[at], b, rule);

		Shape &shape = _darrays_meta[at].shape;
		const Disposition &with = _darrays_meta[b.id()].disposition;

		_align(shape, with, rule, _darrays_meta[at].disposition);
	}

	void PGrid::realign_with(DArray a, Template t, ARule rule)
	{
		expect(_is_declared(a));
		expect(_is_alignable(a));

		expect(_is_declared(t));
		expect(_is_distributed(t));

		uint32_t at = a.id();
		uint32_t elem_size = _darrays_meta[at].darray.elem_size;

		_root_aligner(_darrays_meta[at], t, rule);

		Shape &shape = _darrays_meta[at].shape;
		const Disposition &with = _templates_meta[t.id()].disposition;

		// save the previous disposition
		Disposition before = _darrays_meta[at].disposition;

		_align(shape, with, rule, _darrays_meta[at].disposition);

		_redistribute(before, _darrays_meta[at].disposition, elem_size, _mpi);
	}

	void PGrid::realign_with(DArray a, DArray b, ARule rule)
	{
		expect(_is_declared(a));
		expect(_is_alignable(a));

		expect(_is_declared(b));
		expect(_is_distributed(b));

		uint32_t at = a.id();
		uint32_t elem_size = _darrays_meta[at].darray.elem_size;

		_root_aligner(_darrays_meta[at], b, rule);

		Shape &shape = _darrays_meta[at].shape;
		const Disposition &with = _darrays_meta[b.id()].disposition;

		// save the previous disposition
		Disposition before = _darrays_meta[at].disposition;

		_align(shape, with, rule, _darrays_meta[at].disposition);

		_redistribute(before, _darrays_meta[at].disposition, elem_size, _mpi);
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
		uint32_t at = t.id();

		return _templates_meta[at].type != Meta::UNDEF;
	}

	bool PGrid::_is_distributed(DArray a) const
	{
		uint32_t at = a.id();

		return _darrays_meta[at].type != Meta::UNDEF;
	}

	bool PGrid::_is_distributable(Template t) const
	{
		uint32_t at = t.id();

		return _templates_meta[at].type == Meta::DISTRIBUTABLE;
	}

	bool PGrid::_is_distributable(DArray a) const
	{
		uint32_t at  = a.id();

		return _darrays_meta[at].type == Meta::DISTRIBUTABLE;
	}

	bool PGrid::_is_alignable(Template t) const
	{
		uint32_t at = t.id();

		return _templates_meta[at].type == Meta::ALIGNABLE;
	}

	bool PGrid::_is_alignable(DArray a) const
	{
		uint32_t at = a.id();

		return _darrays_meta[at].type == Meta::ALIGNABLE;
	}

	uint64_t PGrid::_rank() const
	{
		return volume(_shape);
	}

	void PGrid::_distribute(const Shape &shape, const DRule &drule, Disposition &out) const
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
			out - результат в виде размещения.
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

		out.resize(_rank());
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

			out[node_id] = local_shape;
		}
	}

	void PGrid::_align(const Shape &shape, const Disposition &with, const ARule &rule, Disposition &out) const
	{
		expect(with.size() == _rank());
		expect(rule.size() > 0);

		out.resize(_rank());
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

				if (aformat.is_asterisk())
					// It is the case when we have empty braces in T, e.g.:
					// #pragma dvm align A ([i] with T[i][])
					continue;

				int64_t template_left = local_template[template_axis].start();
				int64_t template_right = template_left + local_template[template_axis].count() - 1;

				expect(template_left <= template_right);

				if (aformat.is_constant()) {
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

			if (no_local == true)
				;// do nothing, local_shape has already been initialized by the default constructor
			else for (uint32_t local_axis = 0; local_axis < local_shape.size(); local_axis++) {
				if (seen_axis[local_axis] == false)
					local_shape[local_axis] = shape[local_axis];
			}

			out[node_id] = local_shape;
		}
	}

	uint32_t PGrid::_distance(uint32_t node1, uint32_t node2) const
	{
		Coord c1(_shape.size()), c2(_shape.size());

		id2coord(_shape, node1, c1);
		id2coord(_shape, node2, c2);

		return coord_distance(c1, c2);
	}

	void PGrid::_redistribute(const Disposition &before, const Disposition &after, uint32_t elem_size, mpisimulator::MPI &mpi) const
	{
		// computes and executes send recv pairs to perfrom given redistribution
		expect(before.size() > 0);
		expect(before.size() == after.size());
		expect(before.size() == _rank());

		for (uint32_t receiver = 0; receiver < after.size(); receiver++) {
			std::vector<uint32_t> senders;
			const Shape &target = after[receiver];
			uint64_t total_received = 0;

			if (volume(target) == 0)
				// nothing to receive
				continue;

			for (uint32_t node = 0; node < before.size(); node++) {
				const Shape &local = before[node];
				bool add = true;

				if (volume(local) == 0)
					// nothing to send
					continue;

				expect(target.size() == local.size());

				if (shape_intersects(target, local) == false)
					continue;

				for (uint32_t i = 0; i < senders.size(); i++) {
					uint32_t sender = senders[i];
					const Shape &stored = before[sender];

					if (shape_intersects(local, stored) == false)
						continue;

					// They are equal if they intersect
					expect(shape_equal(local, stored));

					// We replace this sender or ignore the current node
					add = false;

					if (_distance(receiver, node) < _distance(receiver, sender))
						// replace sender
						senders[i] = node;

					break;
				}

				if (add == false)
					continue;

				senders.push_back(node);
			}

			for (uint32_t i = 0; i < senders.size(); i++) {
				uint32_t sender = senders[i];
				const Shape &local = before[sender];

				Shape part(local.size());

				shape_intersect(target, local, part);

				uint64_t elem_cnt = volume(part);
				total_received += elem_cnt;

				if (sender == receiver)
					// Don't communicate with self.
					// But we have to check the consistency via
					// accumalating 'total_received' above.
					continue;

				uint64_t bytes = elem_cnt * elem_size;

				mpi.send(sender, receiver, bytes);
				mpi.recv(receiver, sender, bytes);
			}

			expect(volume(target) == total_received);
		}
	}
}
