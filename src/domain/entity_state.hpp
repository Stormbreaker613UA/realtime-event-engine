// entity_state.hpp
#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

namespace domain {

struct EntityState {
	uint64_t id = 0;
	std::unordered_map<std::string, std::string> properties;
};

} // namespace domain
