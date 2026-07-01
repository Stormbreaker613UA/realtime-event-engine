// session.cpp
#include "session.hpp"

#include <utility>

namespace network {

Session::Session(std::string id)
	: id_(std::move(id))
{
}

void Session::on_receive(const uint8_t* data, size_t len)
{
	if (data == nullptr || len == 0) return;
	framer_.append_data(data, len);
	auto messages = framer_.extract_messages();
	if (handler_) {
		for (auto& m : messages) {
			handler_(m);
		}
	}
}

Session::ByteVec Session::prepare_send(const ByteVec& payload)
{
	return Framing::encode(payload);
}

} // namespace network
