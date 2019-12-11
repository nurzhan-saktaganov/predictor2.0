#pragma once

#include "Exception.hpp"

namespace mpisimulator {
	class MisusageImpl : public Misusage {
	public:
		MisusageImpl();
		MisusageImpl(const char *what);

		const char * what() const noexcept;
	private:
		const char *_what;
	};
}
