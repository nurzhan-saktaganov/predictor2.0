#pragma once

#include <vector>

#include "Template.hpp"
#include "DArray.hpp"
#include "Dispositions.hpp"

namespace dvmpredictor {
	class Distribution {
	public:
		Distribution();

		// saves template distribution info
		void dispose(Template t, Dispositions d);

		// saves darray distribubtion info
		void dispose(DArray a, Dispositions d);

		Dispositions dispositions_of(Template t) const;
		Dispositions dispositions_of(DArray a) const;
	private:
		void _dispose(std::vector<Dispositions> &v, uint32_t at, Dispositions d);
		Dispositions _dispositions(const std::vector<Dispositions> &v, uint32_t at) const;

		std::vector<Dispositions> _templates_dispositions;
		std::vector<Dispositions> _darray_dispositions;
	};
}
