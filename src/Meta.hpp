#pragma once

#include <vector>

#include "Shape.hpp"
#include "Template.hpp"
#include "DArray.hpp"
#include "Shadow.hpp"

namespace dvmpredictor {
	class Meta {
	public:
		Meta();

		void save(Template t, Shape sh);
		void save(DArray a, Shape sh, Shadow sw, uint32_t elem_size);

		Shape shape(Template t) const;
		Shape shape(DArray a) const;
		Shadow shadow(DArray a) const;
		uint32_t elem_size(DArray a) const;
	private:
		struct aligner {
			int type;
			union {
				DArray darray;
				Template templ;
			};
		};

		struct template_meta {
			Shape shape;
			Template templ;
			struct aligner aligner;
		};

		struct darray_meta {
			Shape shape;
			Shadow shadow;
			uint32_t elem_size;
			DArray darray;
			struct aligner aligner;
		};

		bool _saved(DArray a) const;
		bool _saved(Template t) const;

		std::vector<struct template_meta> _template;
		std::vector<struct darray_meta> _darray;
	};
}
