// auth_handler.hpp
#pragma once

#include "ihandler.hpp"

#include <string>

namespace pipeline { struct PipelineContext; }

namespace handlers {

class AuthHandler : public IHandler {
public:
	explicit AuthHandler(std::string realm = "default") : realm_(std::move(realm)) {}
	void handle(const core::Event& ev, pipeline::PipelineContext& ctx) override;

private:
	std::string realm_;
};

} // namespace handlers
