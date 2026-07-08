// event_router.hpp
#pragma once

#include "pipeline/handlers/ihandler.hpp"
#include "core/event.hpp" 

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace pipeline {

class EventRouter {
public:
	using HandlerPtr = handlers::IHandlerPtr;

	void add_handler(const std::string& event_type, HandlerPtr handler)
	{
		handlers_[event_type].push_back(std::move(handler));
	}

	template <class Context>
	void route(const core::Event& ev, Context& ctx) const
	{
		auto it = handlers_.find(ev.type);
		if (it == handlers_.end()) return;
		for (auto& h : it->second) {
			if (h) h->handle(ev, ctx);
		}
	}

private:
	std::unordered_map<std::string, std::vector<HandlerPtr>> handlers_;
};

} // namespace pipeline
