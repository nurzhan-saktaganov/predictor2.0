#pragma once

#include "Range.hpp"
#include "Ranges.hpp"

namespace dvmpredictor {
	using namespace std;

	class DFormat {
	public:
		DFormat();

		void none();
		void block();

		bool distributes() const;

		Ranges distribute(Range range, uint32_t procs) const;

	private:
		Ranges _distribute_block(Range range, uint32_t procs) const;

		enum Format { UNDEF, NONE, BLOCK, MULT_BLOCK, WGT_BLOC, GEN_BLOCK };

		enum Format _format;
	};
}
