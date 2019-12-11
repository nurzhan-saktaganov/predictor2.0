#pragma once

#include <exception>

namespace mpisimulator {
	class Misusage : public std::exception {
		using std::exception::exception;
	};
}
