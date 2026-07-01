// framing.cpp
#include "framing.hpp"

#include <cstddef>
#include <cstring>

namespace network {

Framing::ByteVec Framing::encode(const ByteVec& payload)
{
	Framing::ByteVec out;
	uint32_t len = static_cast<uint32_t>(payload.size());
	// big-endian length prefix
	out.push_back(static_cast<uint8_t>((len >> 24) & 0xFF));
	out.push_back(static_cast<uint8_t>((len >> 16) & 0xFF));
	out.push_back(static_cast<uint8_t>((len >> 8) & 0xFF));
	out.push_back(static_cast<uint8_t>((len) & 0xFF));
	out.insert(out.end(), payload.begin(), payload.end());
	return out;
}

void Framing::append_data(const uint8_t* data, size_t len)
{
	if (len == 0) return;
	buffer_.insert(buffer_.end(), data, data + len);
}

std::vector<Framing::ByteVec> Framing::extract_messages()
{
	std::vector<ByteVec> messages;

	size_t offset = 0;
	while (true) {
		// need at least 4 bytes for length
		if (buffer_.size() - offset < 4) break;

		// read big-endian length
		uint32_t len = 0;
		len |= static_cast<uint32_t>(buffer_[offset]) << 24;
		len |= static_cast<uint32_t>(buffer_[offset + 1]) << 16;
		len |= static_cast<uint32_t>(buffer_[offset + 2]) << 8;
		len |= static_cast<uint32_t>(buffer_[offset + 3]);

		if (buffer_.size() - offset < 4 + len) break; // wait for full payload

		ByteVec msg;
		if (len > 0) {
			msg.insert(msg.end(), buffer_.begin() + offset + 4, buffer_.begin() + offset + 4 + len);
		}
		messages.push_back(std::move(msg));

		offset += 4 + len;
	}

	if (offset > 0) {
		// erase consumed bytes
		buffer_.erase(buffer_.begin(), buffer_.begin() + static_cast<std::ptrdiff_t>(offset));
	}

	return messages;
}

} // namespace network
