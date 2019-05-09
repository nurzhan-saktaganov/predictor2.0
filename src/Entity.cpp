#include "Entity.hpp"

namespace dvmpredictor {

	Entity::Entity(uint32_t id)
		: _id(id)
	{}

	uint32_t Entity::id() const
	{
		return _id;
	}

	bool Entity::defined() const
	{
		return _id != id_undef;
	}
}
