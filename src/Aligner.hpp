#pragma once

#include "DArray.hpp"
#include "Template.hpp"

namespace dvmpredictor {
	enum AlignerType { UNDEF, DARRAY, TEMPLATE };

	class Aligner {
	public:
		Aligner();
		Aligner(DArray a);
		Aligner(Template t);

		enum AlignerType type() const;
		DArray darray() const;
		Template templ() const;

	private:
		AlignerType _type;
		union {
			DArray _darray;
			Template _templ;
		};
	};
}
