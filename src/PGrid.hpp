#pragma once

#include "MPI.hpp"

#include "Shape.hpp"
#include "Template.hpp"
#include "DArray.hpp"
#include "DRule.hpp"
#include "ARule.hpp"
#include "Coord.hpp"
#include "Node.hpp"
#include "Meta.hpp"
#include "Shadow.hpp"
#include "Disposition.hpp"
#include "Dispositions.hpp"

namespace dvmpredictor {
	class PGrid {
	public:
		PGrid();
		PGrid(Shape shape, double latency, double bandwidth);

		// TODO add validate to all public interface

		Template declare_template(Shape shape);
		DArray declare_darray(Shape shape, Shadow shadow, uint32_t elem_size);

		void distribute(Template t, DRule rule);
		void distribute(DArray a, DRule rule);

		void redistribute(Template t, DRule rule);
		void redistribute(DArray a, DRule rule);

		void align_with(DArray a, Template t, ARule rule);
		void align_with(DArray a, DArray b, ARule rule);

		void realign_with(DArray a, Template t, ARule rule);
		void realign_with(DArray a, DArray b, ARule rule);

	private:
		Dispositions _templates_disposition;
		Dispositions _darrays_disposition;

/*
		struct align_info {
			struct {
				DArray a;
				Template t;
			} aligned;

			struct {
				DArray a;
				Template t;
			} aligner;
		};
*/
		// private methods
		bool _inited() const;
		bool _is_declared(Template t) const;
		bool _is_declared(DArray a) const;
		bool _is_distributed(Template t) const;
		bool _is_distributed(DArray a) const;
		bool _is_aligned(Template t) const;
		bool _is_aligned(DArray a) const;

		// procs count of the pgrid
		uint64_t _rank() const;

		void _distribute(const Shape &shape, const DRule &drule, Disposition &disposition) const;
		void _align(const Shape &shape, const Disposition &with, const ARule &rule, Disposition &disposition) const;
		//void _redispose(Dispositions before, Dispositions after);

		void _redistribute(const Disposition &before, const Disposition &after, uint32_t elem_size, mpisimulator::MPI &mpi) const;

		uint32_t _distance(uint32_t node1, uint32_t node2) const;

		// PGrid shape
		Shape _shape;

		uint32_t _next_template_id;
		uint32_t _next_darray_id;

		// Meta information about darrays and templates
		Meta _meta;

		mpisimulator::MPI _mpi;
	};
}
