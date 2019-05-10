#pragma once

#include <vector>

#include "DArray.hpp"
#include "DArrays.hpp"

namespace dvmpredictor {
	class AlignGraph {
	public:
		AlignGraph();

		void align(DArray a, DArray on);
		void realign(DArray a, DArray on);

		DArrays aligned_on(DArray a) const;
		DArray aligner_off(DArray a) const;

	private:
		bool _is_aligned(DArray a) const;

		// TODO consider using std::set instead of std::vector for _aligned_on;
		std::vector<DArrays> _aligned_on;
		std::vector<DArray> _aligner_off;
	};
}
