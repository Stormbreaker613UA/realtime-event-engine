// server.hpp
#pragma once

#include "session.hpp"

#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace network {

class Server {
public:
	using ByteVec = Session::ByteVec;
	using SessionPtr = std::shared_ptr<Session>;

	Server() = default;

	// Create or return existing session with given id
	SessionPtr create_session(const std::string& id);

	// Remove session by id, returns true if removed
	bool remove_session(const std::string& id);

	// Feed raw bytes received from the transport into the session's framer.
	// Returns true if the session exists and bytes were delivered.
	bool deliver_bytes(const std::string& id, const uint8_t* data, size_t len);

	// Prepare framed bytes to send to a single session. Returns nullopt if session
	// doesn't exist.
	std::optional<ByteVec> prepare_send(const std::string& id, const ByteVec& payload);

	// Prepare framed bytes for broadcast. Returns vector of (session id, bytes)
	std::vector<std::pair<std::string, ByteVec>> prepare_broadcast(const ByteVec& payload);

	// Set a default message handler that will be applied to new sessions and
	// applied to existing sessions immediately.
	void set_default_handler(Session::MessageHandler handler);

	// Number of active sessions
	size_t session_count() const;

private:
	mutable std::mutex mutex_;
	std::unordered_map<std::string, SessionPtr> sessions_;
	Session::MessageHandler default_handler_;
};

} // namespace network
