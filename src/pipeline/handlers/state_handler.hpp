// state_handler.hpp
#pragma once

#include "ihandler.hpp"

namespace pipeline { struct PipelineContext; }

namespace handlers {

class StateHandler : public IHandler {
public:
	StateHandler() = default;
	void handle(const core::Event& ev, pipeline::PipelineContext& ctx) override;
};

} // namespace handlers
