// istorage.hpp
#pragma once

#include "core/event.hpp"

#include <memory>

namespace storage {

class IStorage {
public:
	virtual ~IStorage() = default;
	virtual bool save_event(const core::Event& ev) = 0;
};

using IStoragePtr = std::shared_ptr<IStorage>;

} // namespace storage
