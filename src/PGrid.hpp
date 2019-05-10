#pragma once

#include "Shape.hpp"
#include "Template.hpp"
#include "DArray.hpp"
#include "DRule.hpp"

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

		// TODO add align rule
		void align_on(DArray a, Template t);
		void align_on(DArray a, DArray b);

		// TODO add align rule
		void realign_on(DArray a, Template t);
		void realign_on(DArray a, DArray b);

	private:
		bool _inited() const;
		bool _is_declared(Template t) const;
		bool _is_declared(DArray a) const;

		Shape _shape;

		uint32_t _next_template_id;
		uint32_t _next_darray_id;
	};
}
