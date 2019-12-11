#pragma once

#include "OperationQueue.hpp"

namespace mpisimulator {
	struct node {
		node() : local_time(0), waiting(false) {}

		OperationQueue ops;
		double local_time;
		bool waiting;
	};
}
