#pragma once

#include "Shape.hpp"
#include "Template.hpp"
#include "DArray.hpp"
#include "DRule.hpp"
#include "ARule.hpp"
#include "Coord.hpp"
#include "Node.hpp"
#include "Distribution.hpp"
#include "Meta.hpp"

namespace dvmpredictor {
	class PGrid {
	public:
		PGrid();
		PGrid(Shape shape);

		Template declare_template(Shape shape);
		DArray declare_darray(Shape shape, uint32_t elem_size);

		void distribute(Template t, DRule rule);
		void distribute(DArray a, DRule rule);

		void redistribute(Template t, DRule rule);
		void redistribute(DArray a, DRule rule);

		void align_on(DArray a, Template t, ARule rule);
		void align_on(DArray a, DArray b, ARule rule);

		void realign_on(DArray a, Template t, ARule rule);
		void realign_on(DArray a, DArray b, ARule rule);

	private:
		Node _node(Coord coord) const;

		bool _inited() const;
		bool _is_declared(Template t) const;
		bool _is_declared(DArray a) const;

		const Shape _shape;

		uint32_t _next_template_id;
		uint32_t _next_darray_id;

		Distribution _distribution;
		Meta _meta;
	};
}
