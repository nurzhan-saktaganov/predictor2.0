#include "expect.hpp"
#include "Meta.hpp"
#include "Util.hpp"

namespace dvmpredictor {
	Meta::Meta() {}

	void Meta::save(DArray a, Shape sh, Shadow sw, uint32_t elem_size)
	{
		expect(a.defined());
		expect(!_saved(a));

		auto at = a.id();

		ensure(_darray, at + 1);

		_darray[at].darray = a;
		_darray[at].shadow = sw;
		_darray[at].shape = sh;
		_darray[at].elem_size = elem_size;
	}

	void Meta::save(Template t, Shape sh)
	{
		expect(t.defined());
		expect(!_saved(t));

		auto at = t.id();

		ensure(_template, at + 1);

		_template[at].templ = t;
		_template[at].shape = sh;
	}

	Shape Meta::shape(Template t) const
	{
		expect(t.defined());
		expect(_saved(t));

		auto at = t.id();

		return _template[at].shape;
	}

	Shape Meta::shape(DArray a) const
	{
		expect(a.defined());
		expect(_saved(a));

		auto at = a.id();

		return _darray[at].shape;
	}

	Shadow Meta::shadow(DArray a) const
	{
		expect(a.defined());
		expect(_saved(a));

		auto at = a.id();

		return _darray[at].shadow;
	}

	uint32_t Meta::elem_size(DArray a) const
	{
		expect(a.defined());
		expect(_saved(a));

		auto at = a.id();

		return _darray[at].elem_size;
	}

	bool Meta::_saved(DArray a) const
	{
		expect(a.defined());

		auto at = a.id();

		if (at >= _darray.size())
			return false;

		return _darray[at].darray.defined();
	}

	bool Meta::_saved(Template t) const
	{
		expect(t.defined());

		auto at = t.id();

		if (at >= _template.size())
			return false;

		return _template[at].templ.defined();
	}
}
