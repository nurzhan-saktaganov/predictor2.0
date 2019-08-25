#pragma once

#include "OperationQueue.hpp"

namespace mpisimulator {
	struct node {
		OperationQueue ops;
		double local_time;
	};
}
