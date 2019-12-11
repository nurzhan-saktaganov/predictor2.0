#pragma once

#include <cstdint>

namespace mpisimulator {

	enum OperationType {
		undefined,
		send,
		recv,
		skip,
		barrier,
		last,
	};

	struct operation {
		OperationType type;
		uint32_t at;
		union {
			struct {
				uint32_t to;
				uint64_t bytes;
			} send;
			struct {
				uint32_t from;
				uint64_t bytes;
			} recv;
			struct {
				double time;
			} skip;
		};
	};
}
