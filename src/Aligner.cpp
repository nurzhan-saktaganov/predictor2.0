#include "expect.hpp"

#include "Aligner.hpp"

namespace dvmpredictor {
	Aligner::Aligner()
		: _type(AlignerType::UNDEF)
	{}

	Aligner::Aligner(DArray a)
		: _type(AlignerType::DARRAY)
		, _darray(a)
	{
		expect(a.defined());
	}

	Aligner::Aligner(Template t)
		: _type(AlignerType::TEMPLATE)
		, _templ(t)
	{
		expect(t.defined());
	}

	enum AlignerType Aligner::type() const
	{
		return _type;
	}

	DArray Aligner::darray() const
	{
		expect(_type == AlignerType::DARRAY);
		return _darray;
	}

	Template Aligner::templ() const
	{
		expect(_type == AlignerType::TEMPLATE);
		return _templ;
	}
}
