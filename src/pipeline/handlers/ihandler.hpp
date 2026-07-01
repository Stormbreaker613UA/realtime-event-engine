// ihandler.hpp
#pragma once

#include <memory>

namespace core { struct Event; }
namespace pipeline { struct PipelineContext; }

namespace handlers {

class IHandler {
public:
	virtual ~IHandler() = default;
	virtual void handle(const core::Event& ev, pipeline::PipelineContext& ctx) = 0;
};

using IHandlerPtr = std::shared_ptr<IHandler>;

} // namespace handlers
