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

	private:
		void _dispose(std::vector<Dispositions> &v, uint32_t at, Dispositions d);

		std::vector<Dispositions> _templates_dispositions;
		std::vector<Dispositions> _darray_dispositions;
	};
}
