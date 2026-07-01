// session.hpp
#pragma once

#include "framing.hpp"

#include <functional>
#include <memory>
#include <string>

namespace network {

class Session {
public:
	using ByteVec = Framing::ByteVec;
	using MessageHandler = std::function<void(const ByteVec& payload)>;

	explicit Session(std::string id = {});

	// Called by networking code when raw bytes arrive for this session.
	void on_receive(const uint8_t* data, size_t len);

	// Prepare bytes to send (frames the payload). Caller is responsible for
	// writing returned bytes to the socket.
	static ByteVec prepare_send(const ByteVec& payload);

	void set_message_handler(MessageHandler h) { handler_ = std::move(h); }

	const std::string& id() const noexcept { return id_; }

private:
	std::string id_;
	Framing framer_;
	MessageHandler handler_;
};

} // namespace network
