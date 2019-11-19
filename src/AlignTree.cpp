#include "expect.hpp"
#include "Util.hpp"
#include "AlignTree.hpp"

namespace dvmpredictor {
	AlignTree::AlignTree() {}

	void AlignTree::align(DArray a, Template on)
	{
		expect(!_is_aligned(a));

		_set_aligner(a, Aligner(on));
		_set_aligned(_aligned_on_template, on.id(), a);
	}

	void AlignTree::align(DArray a, DArray on)
	{
		expect(!_is_aligned(a));

		_set_aligner(a, Aligner(on));
		_set_aligned(_aligned_on_darray, on.id(), a);
	}

	void AlignTree::realign(DArray a, Template on)
	{
		expect(_is_aligned(a));

		_clear_aligned(a);

		_set_aligner(a, Aligner(on));
		_set_aligned(_aligned_on_template, on.id(), a);
	}

	void AlignTree::realign(DArray a, DArray on)
	{
		expect(_is_aligned(a));

		_clear_aligned(a);

		_set_aligner(a, Aligner(on));
		_set_aligned(_aligned_on_darray, on.id(), a);
	}

	Aligner AlignTree::_aligner(DArray a) const
	{
		must(a.defined());

		if (a.id() >= _aligner_of_darray.size())
			return Aligner();

		return _aligner_of_darray[a.id()];
	}

	bool AlignTree::_is_aligned(DArray a) const
	{
		Aligner aligner = _aligner(a);

		return aligner.type() != AlignerType::UNDEF;
	}

	void AlignTree::_set_aligner(DArray a, Aligner aligner)
	{
		auto at = a.id();

		ensure(_aligner_of_darray, at + 1);

		_aligner_of_darray[at] = aligner;
	}

	void AlignTree::_clear_aligned(DArray a)
	{
		must(a.defined());

		auto aligner = _aligner(a);

		must(aligner.type() != AlignerType::UNDEF);

		if (aligner.type() == AlignerType::TEMPLATE)
			_remove_aligned(_aligned_on_template, aligner.templ().id(), a);
		else
			_remove_aligned(_aligned_on_darray, aligner.darray().id(), a);
	}

	void AlignTree::_set_aligned(std::vector<DArrays> &v, uint32_t at, DArray aligned)
	{
		must(aligned.defined());

		ensure(v, at + 1);

		DArrays &darrays = v[at];

		darrays.push_back(aligned);
	}

	void AlignTree::_remove_aligned(std::vector<DArrays> &v, uint32_t at, DArray aligned)
	{
		must(aligned.defined());
		must(at + 1 <= v.size());

		DArrays darrays = v[at];

		for (uint32_t i = 0; i < darrays.size(); i++) {
			DArray &current = darrays[i];

			must(current.defined());

			if (current.id() == aligned.id()) {
				std::swap(darrays[i], darrays[v.size() - 1]);
				// just remove the last element
				darrays.pop_back();
				return;
			}
		}

		// Must be unreachable case
		abort();
	}
}
