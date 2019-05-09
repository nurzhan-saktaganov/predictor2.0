#pragma once

#include <cstdint>

namespace dvmpredictor {
	using namespace std;

	class Entity {
	public:
		Entity(uint32_t id = id_undef);

		uint32_t id() const;
		bool defined() const;

		static const uint32_t id_undef = UINT32_MAX;

	private:
		uint32_t _id;
	};
}
