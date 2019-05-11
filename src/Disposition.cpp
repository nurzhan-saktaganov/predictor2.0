#include "Disposition.hpp"

namespace dvmpredictor {
	Disposition::Disposition() : Disposition(Node(), SubShape()) {}

	Disposition::Disposition(Node n, SubShape sh)
		: _node(n), _sub_shape(sh)
	{}

	Node Disposition::node() const
	{
		return _node;
	}

	Shape Disposition::sub_shape() const
	{
		return _sub_shape;
	}
}
