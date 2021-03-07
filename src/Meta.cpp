#include "expect.hpp"
#include "Meta.hpp"
#include "Util.hpp"

namespace dvmpredictor {
	Meta::Meta()
	{
		this->type = UNDEF;
	}

	Meta::Meta(Shape shape) : Meta()
	{
		this->shape = shape;
	}

	Meta::Meta(Template t, Shape shape) : Meta(shape)
	{
		this->templ.t = t;
	};

	Meta::Meta(DArray d, Shape shape, Shadow shadow, uint32_t elem_size) : Meta(shape)
	{
		this->darray.d = d;
		this->darray.shadow = shadow;
		this->darray.elem_size = elem_size;
	};
}
