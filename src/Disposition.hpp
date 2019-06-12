#pragma once

#include "Node.hpp"
#include "SubShape.hpp"

namespace dvmpredictor {
	class Disposition {
	public:
		Disposition();
		Disposition(Node n, SubShape sh);

		Node node() const;
		SubShape sub_shape() const;

	private:
		const Node _node;
		const SubShape _sub_shape;
	};
}
