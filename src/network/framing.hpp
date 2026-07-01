// framing.hpp
#pragma once

#include <cstdint>
#include <vector>

namespace network {

// Simple length-prefixed framer.
// Prefixes each message with a 4-byte big-endian length.
class Framing {
public:
	using ByteVec = std::vector<uint8_t>;

	// Encode a single message with a 4-byte length prefix.
	static ByteVec encode(const ByteVec& payload);

	// Append raw bytes received from network into the internal buffer.
	void append_data(const uint8_t* data, size_t len);

	// Extract complete messages from the internal buffer. Returns zero or more
	// complete payloads (without length prefixes).
	std::vector<ByteVec> extract_messages();

private:
	ByteVec buffer_;
};

} // namespace network
