#pragma once

#include "MPI.hpp"

#include "Shape.hpp"
#include "Template.hpp"
#include "DArray.hpp"
#include "DRule.hpp"
#include "ARule.hpp"
#include "Coord.hpp"
#include "Node.hpp"
#include "Distribution.hpp"
#include "Meta.hpp"
#include "Shadow.hpp"
#include "AlignTree.hpp"

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

		void _distribute(const Shape &shape, DRule drule, Disposition &disposition) const;
		Dispositions _align(Shape sh, Dispositions with, ARule rule) const;
		void _redispose(Dispositions before, Dispositions after);

		// PGrid shape
		Shape _shape;

		uint32_t _next_template_id;
		uint32_t _next_darray_id;

		Distribution _distribution;
		// Meta information about darrays and templates
		Meta _meta;
		AlignTree _align_tree;

		mpisimulator::MPI _mpi;
	};
}
