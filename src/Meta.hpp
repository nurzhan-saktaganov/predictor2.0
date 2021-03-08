#pragma once

#include <cstdint>
#include <vector>

#include "Shape.hpp"
#include "Disposition.hpp"
#include "Template.hpp"
#include "DArray.hpp"
#include "ARule.hpp"
#include "Shadow.hpp"

namespace dvmpredictor {
	class Meta {
	private:
		Meta(Shape shape);

	public:
		Meta();
		Meta(Template t, Shape shape);
		Meta(DArray d, Shape shape, Shadow shadow, uint32_t elem_size);

		enum Type {UNDEF, DISTRIBUTABLE, ALIGNABLE};

		Shape shape;
		Type type;
		Disposition disposition;

		// We could use anonymous unions below,
		// but we cannot use them due to c++ restrictions.
		// See: https://en.cppreference.com/w/cpp/language/union
		// notes about non-trivial special member function and
		// anonymous unions.
		// In the future using std::variant may be option too.

		// The struct below is used when type is ALIGNABLE
		struct {
			bool is_template;
		//	union {
				Template templ;
				DArray darray;
		//	};
			ARule rule;
		} aligner;

		//union {
			struct {
				Template t;
			} templ;
			struct {
				DArray d;
				Shadow shadow;
				uint32_t elem_size;
			} darray;
		//};
	};
}
