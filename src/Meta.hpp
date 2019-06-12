#pragma once

#include <vector>

#include "Shape.hpp"
#include "Template.hpp"
#include "DArray.hpp"

namespace dvmpredictor {
	class Meta {
	public:
		Meta();

		void of_template(Template t, Shape sh);
		void of_darray(DArray a, Shape sh, int elem_size);

		Shape shape(Template t) const;
		Shape shape(DArray a) const;
		int elem_size(DArray a) const;

	private:
		std::vector<Shape> _template_shape;
		std::vector<Shape> _darray_shape;
		std::vector<int> _elem_size;
	};
}
