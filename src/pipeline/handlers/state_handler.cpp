// state_handler.cpp
#include "state_handler.hpp"
#include "../core/logger.hpp"
#include "../core/event.hpp"

namespace handlers {

void StateHandler::handle(const core::Event& ev, pipeline::PipelineContext& ctx)
{
	(void)ctx;
	auto& lg = core::global_logger();
	lg.debug("StateHandler received event: " + ev.type);
	// placeholder: update entity state in domain
}

} // namespace handlers
