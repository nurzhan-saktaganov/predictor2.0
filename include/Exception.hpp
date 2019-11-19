#pragma once

#include <exception>

namespace dvmpredictor {
	class Misusage : public std::exception {
		using std::exception::exception;
	};
}
