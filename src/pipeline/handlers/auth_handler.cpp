// auth_handler.cpp
#include "auth_handler.hpp"
#include "core/logger.hpp"
#include "core/event.hpp"

namespace handlers {

void AuthHandler::handle(const core::Event& ev, pipeline::PipelineContext& ctx)
{
	(void)ctx;
	auto& lg = core::global_logger();
	lg.info("AuthHandler handling event type: " + ev.type);
	// Minimal placeholder: real implementation would consult auth service
}

} // namespace handlers
