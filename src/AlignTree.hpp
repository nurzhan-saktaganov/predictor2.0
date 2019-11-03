#pragma once

#include <vector>

#include "DArray.hpp"
#include "Template.hpp"
#include "Aligner.hpp"
#include "DArrays.hpp"

namespace dvmpredictor {
	class AlignTree {
	// XXX: Only an darray may be aligned, aligner is darray or template.
	public:
		AlignTree();

		void align(DArray a, Template on);
		void align(DArray a, DArray on);

		void realign(DArray a, Template on);
		void realign(DArray a, DArray on);
	private:
		Aligner _aligner(DArray a) const;
		bool _is_aligned(DArray a) const;

		void _set_aligner(DArray a, Aligner aligner);
		void _clear_aligned(DArray a);
		void _set_aligned(std::vector<DArrays> &v, uint32_t at, DArray aligned);
		void _remove_aligned(std::vector<DArrays> &v, uint32_t at, DArray aligned);

		// BACKLOG: consider using std::set instead of std::vector
		std::vector<DArrays> _aligned_on_template;
		std::vector<DArrays> _aligned_on_darray;

		std::vector<Aligner> _aligner_of_darray;
	};
}
