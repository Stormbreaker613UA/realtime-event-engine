// event.hpp
#pragma once

#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

namespace core {

struct Event {
	using ByteVec = std::vector<uint8_t>;

	uint64_t id = 0; // application-assigned identifier
	std::string type; // event type/name
	ByteVec payload; // binary payload
	std::chrono::system_clock::time_point timestamp = std::chrono::system_clock::now();
};

} // namespace core
