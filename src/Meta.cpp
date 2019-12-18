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

		return _template[t.id()].shape;
	}

	Shape Meta::shape(DArray a) const
	{
		expect(a.defined());
		expect(_saved(a));

		return _darray[a.id()].shape;
	}

	Shadow Meta::shadow(DArray a) const
	{
		expect(a.defined());
		expect(_saved(a));

		return _darray[a.id()].shadow;
	}

	uint32_t Meta::elem_size(DArray a) const
	{
		expect(a.defined());
		expect(_saved(a));

		return _darray[a.id()].elem_size;
	}

	bool Meta::_saved(DArray a) const
	{
		expect(a.defined());

		if (a.id() >= _darray.size())
			return false;

		return _darray[a.id()].darray.defined();
	}

	bool Meta::_saved(Template t) const
	{
		expect(t.defined());

		if (t.id() >= _template.size())
			return false;

		return _template[t.id()].templ.defined();
	}
}
