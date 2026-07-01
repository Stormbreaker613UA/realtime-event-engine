// broadcast_handler.hpp
#pragma once

#include "ihandler.hpp"

namespace pipeline { struct PipelineContext; }

namespace handlers {

class BroadcastHandler : public IHandler {
public:
	BroadcastHandler() = default;
	void handle(const core::Event& ev, pipeline::PipelineContext& ctx) override;
};

} // namespace handlers
