// test_event_router.cpp
#include "../pipeline/event_router.hpp"
#include "../core/event.hpp"

#include <cassert>
#include <memory>
#include <string>

struct DummyHandler : handlers::IHandler {
	bool called = false;
	void handle(const core::Event& ev, pipeline::PipelineContext& ctx) override
	{
		(void)ev; (void)ctx; called = true;
	}
};

int main()
{
	pipeline::EventRouter r;
	auto h = std::make_shared<DummyHandler>();
	r.add_handler("test", h);

	core::Event ev; ev.type = "test";
	pipeline::PipelineContext ctx;
	r.route(ev, ctx);
	assert(h->called);
	return 0;
}
