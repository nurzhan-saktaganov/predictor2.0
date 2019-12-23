#include "expect.hpp"
#include "Distribution.hpp"
#include "Util.hpp"

namespace dvmpredictor {
	Distribution::Distribution() {}

	void Distribution::dispose(Template t, Dispositions d)
	{
		expect(t.defined());

		_dispose(_templates_dispositions, t.id(), d);
	}

	void Distribution::dispose(DArray a, Dispositions d)
	{
		expect(a.defined());

		_dispose(_darray_dispositions, a.id(), d);
	}

	void Distribution::_dispose(std::vector<Dispositions> &v, uint32_t at, Dispositions d)
	{
		ensure(v, at + 1);
		v[at] = d;
	}

	Dispositions Distribution::dispositions_of(Template t) const
	{
		expect(t.defined());

		return _dispositions(_templates_dispositions, t.id());
	}

	Dispositions Distribution::dispositions_of(DArray a) const
	{
		expect(a.defined());

		return _dispositions(_darray_dispositions, a.id());
	}

	Dispositions Distribution::_dispositions(const std::vector<Dispositions> &v, uint32_t at) const
	{
		expect(at < v.size());

		Dispositions d = v[at];

		expect(d.size() > 0);

		return d;
	}
}
