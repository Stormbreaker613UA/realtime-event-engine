// server.cpp
#include "server.hpp"

#include <algorithm>

namespace network {

SessionPtr Server::create_session(const std::string& id)
{
	std::lock_guard<std::mutex> lk(mutex_);
	auto it = sessions_.find(id);
	if (it != sessions_.end()) return it->second;

	auto s = std::make_shared<Session>(id);
	if (default_handler_) s->set_message_handler(default_handler_);
	sessions_.emplace(id, s);
	return s;
}

bool Server::remove_session(const std::string& id)
{
	std::lock_guard<std::mutex> lk(mutex_);
	return sessions_.erase(id) > 0;
}

bool Server::deliver_bytes(const std::string& id, const uint8_t* data, size_t len)
{
	SessionPtr s;
	{
		std::lock_guard<std::mutex> lk(mutex_);
		auto it = sessions_.find(id);
		if (it == sessions_.end()) return false;
		s = it->second;
	}

	// deliver outside lock
	s->on_receive(data, len);
	return true;
}

std::optional<Server::ByteVec> Server::prepare_send(const std::string& id, const ByteVec& payload)
{
	std::lock_guard<std::mutex> lk(mutex_);
	auto it = sessions_.find(id);
	if (it == sessions_.end()) return std::nullopt;
	// framing is static and deterministic; use Session helper
	return Session::prepare_send(payload);
}

std::vector<std::pair<std::string, Server::ByteVec>> Server::prepare_broadcast(const ByteVec& payload)
{
	std::vector<std::pair<std::string, ByteVec>> out;
	std::vector<std::string> ids;
	{
		std::lock_guard<std::mutex> lk(mutex_);
		ids.reserve(sessions_.size());
		for (auto& kv : sessions_) ids.push_back(kv.first);
	}

	if (ids.empty()) return out;

	// prepare framed payload once
	ByteVec framed = Session::prepare_send(payload);

	out.reserve(ids.size());
	for (auto& id : ids) out.emplace_back(id, framed);
	return out;
}

void Server::set_default_handler(Session::MessageHandler handler)
{
	std::lock_guard<std::mutex> lk(mutex_);
	default_handler_ = handler;
	for (auto& kv : sessions_) {
		if (kv.second) kv.second->set_message_handler(default_handler_);
	}
}

size_t Server::session_count() const
{
	std::lock_guard<std::mutex> lk(mutex_);
	return sessions_.size();
}

} // namespace network
