// pipeline_context.hpp
#pragma once

#include <memory>

namespace domain { class SessionRegistry; class AuthService; }
namespace network { class Server; }

namespace pipeline {

struct PipelineContext {
	domain::SessionRegistry* sessions = nullptr;
	domain::AuthService* auth = nullptr;
	network::Server* server = nullptr;
};

} // namespace pipeline
