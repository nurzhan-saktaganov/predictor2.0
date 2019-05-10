#include <cassert>
#include <algorithm>

#include "AlignGraph.hpp"

namespace dvmpredictor {
	AlignGraph::AlignGraph() {}

	void AlignGraph::align(DArray a, DArray on)
	{
		assert(a.defined());
		assert(on.defined());

		assert(!_is_aligned(a));

		auto ensure = std::max(a.id(), on.id());

		if (_aligned_on.size() < ensure)
			_aligned_on.resize(ensure);

		if (_aligner_off.size() < ensure)
			_aligner_off.resize(ensure);

		_aligner_off[a.id()] = on;
		_aligned_on[on.id()].push_back(a);
	}

	void AlignGraph::realign(DArray a, DArray on)
	{
		assert(a.defined());
		assert(on.defined());

		assert(_is_aligned(a));

		// remove `a' from old aligner
		DArrays &aligned = _aligned_on[aligner_off(a).id()];
		for (auto it = aligned.begin(); it != aligned.end(); ++it) {
			if (it->id() != a.id())
				continue;

			auto last = --aligned.end();
			// store last element into current
			*it = *last;
			// just remove the last element
			aligned.pop_back();
			break;
		}


		_aligner_off[a.id()] = on;
		_aligned_on[on.id()].push_back(a);
	}

	DArrays AlignGraph::aligned_on(DArray a) const
	{
		assert(a.defined());
		assert(a.id() < _aligned_on.size());
		return _aligned_on[a.id()];
	}

	DArray AlignGraph::aligner_off(DArray a) const
	{
		assert(a.defined());
		assert(a.id() < _aligner_off.size());
		return _aligner_off[a.id()];
	}

	bool AlignGraph::_is_aligned(DArray a) const
	{
		return a.id() < _aligner_off.size()
			&& _aligner_off[a.id()].defined();
	}
}
