#pragma once

#include <vector>

#include "AFormat.hpp"

namespace dvmpredictor {
	using ARule = std::vector<AFormat>;

	void combine_arule(const ARule &r1, const ARule &r2, ARule &out);
}
