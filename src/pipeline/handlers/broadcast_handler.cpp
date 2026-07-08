// broadcast_handler.cpp
#include "broadcast_handler.hpp"
#include "core/logger.hpp"
#include "core/event.hpp"
#include "network/server.hpp"
#include "pipeline/pipeline_context.hpp" 

namespace handlers 
{

	void BroadcastHandler::handle(const core::Event& ev, pipeline::PipelineContext& ctx)
	{
		auto& lg = core::global_logger();
		lg.info("BroadcastHandler broadcasting event: " + ev.type);

		if (ctx.server) 
		{
			// prepare a simple payload copy and broadcast
			network::Server::ByteVec payload(ev.payload.begin(), ev.payload.end());
			auto msgs = ctx.server->prepare_broadcast(payload);
			(void)msgs; // caller/transport will send
		}
	}

} // namespace handlers
