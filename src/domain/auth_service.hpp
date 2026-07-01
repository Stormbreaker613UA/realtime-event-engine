// auth_service.hpp
#pragma once

#include <string>

namespace domain {

class AuthService {
public:
	AuthService() = default;
	~AuthService() = default;

	// Minimal authentication check for session id
	bool authenticate(const std::string& session_id) const
	{
		// placeholder: allow non-empty ids
		return !session_id.empty();
	}
};

} // namespace domain
