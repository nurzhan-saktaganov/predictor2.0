#pragma once

#include <cstdint>

namespace dvmpredictor {
	class Entity {
	public:
		Entity(uint32_t id = id_undef);

		uint32_t id() const;
		bool defined() const;

		bool operator==(const Entity &with) const;
		bool operator!=(const Entity &with) const;

		static const uint32_t id_undef = UINT32_MAX;

	private:
		uint32_t _id;
	};
}
