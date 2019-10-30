#include <cassert>

#include "Meta.hpp"
#include "Util.hpp"

namespace dvmpredictor {
	Meta::Meta() {}

	void Meta::of_darray(DArray a, Shape sh, Shadow sw, int elem_size)
	{
		assert(a.defined());

		auto at = a.id();

		ensure(_darray_shape, at + 1);
		ensure(_darray_shadow, at + 1);
		ensure(_elem_size, at + 1);

		_darray_shape[at] = sh;
		_darray_shadow[at] = sw;
		_elem_size[at] = elem_size;
	}

	void Meta::of_template(Template t, Shape sh)
	{
		assert(t.defined());

		auto at = t.id();

		ensure(_template_shape, at + 1);
		_template_shape[at] = sh;
	}

	Shape Meta::shape(Template t) const
	{
		assert(t.defined());

		return _template_shape[t.id()];
	}

	Shape Meta::shape(DArray a) const
	{
		assert(a.defined());

		return _darray_shape[a.id()];
	}

	Shadow Meta::shadow(DArray a) const
	{
		assert(a.defined());

		return _darray_shadow[a.id()];
	}

	int Meta::elem_size(DArray a) const
	{
		assert(a.defined());

		return _elem_size[a.id()];
	}
}
