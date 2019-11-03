#include <cassert>

#include "Aligner.hpp"

namespace dvmpredictor {
	Aligner::Aligner()
		: _type(AlignerType::UNDEF)
	{}

	Aligner::Aligner(DArray a)
		: _type(AlignerType::DARRAY)
		, _darray(a)
	{
		assert(a.defined());
	}

	Aligner::Aligner(Template t)
		: _type(AlignerType::TEMPLATE)
		, _templ(t)
	{
		assert(t.defined());
	}

	enum AlignerType Aligner::type() const
	{
		return _type;
	}

	DArray Aligner::darray() const
	{
		assert(_type == AlignerType::DARRAY);
		return _darray;
	}

	Template Aligner::templ() const
	{
		assert(_type == AlignerType::TEMPLATE);
		return _templ;
	}
}
