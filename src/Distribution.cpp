#include <cassert>

#include "Distribution.hpp"

namespace dvmpredictor {
	Distribution::Distribution() {}

	void Distribution::dispose(Template t, Dispositions d)
	{
		assert(t.defined());

		_dispose(_templates_dispositions, t.id(), d);
	}

	void Distribution::dispose(DArray a, Dispositions d)
	{
		assert(a.defined());

		_dispose(_darray_dispositions, a.id(), d);
	}

	void Distribution::_dispose(std::vector<Dispositions> &v, uint32_t at, Dispositions d)
	{
		auto ensure = at + 1;

		if (v.size() < ensure)
			v.resize(ensure);

		v[at] = d;
	}
}
