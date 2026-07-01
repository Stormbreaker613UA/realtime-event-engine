// session_registry.hpp
#pragma once

#include <string>
#include <unordered_set>
#include <vector>

namespace domain {

class SessionRegistry {
public:
	bool add(const std::string& id)
	{
		return ids_.insert(id).second;
	}

	bool remove(const std::string& id)
	{
		return ids_.erase(id) > 0;
	}

	bool contains(const std::string& id) const
	{
		return ids_.find(id) != ids_.end();
	}

	std::vector<std::string> list() const
	{
		return std::vector<std::string>(ids_.begin(), ids_.end());
	}

	size_t size() const { return ids_.size(); }

private:
	std::unordered_set<std::string> ids_;
};

} // namespace domain
