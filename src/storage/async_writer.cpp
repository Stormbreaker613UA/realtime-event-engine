// async_writer.cpp
#include "async_writer.hpp"

#include <chrono>

namespace storage {

AsyncWriter::AsyncWriter(IStoragePtr storage)
	: storage_(std::move(storage))
{
	worker_ = std::thread([this] { run(); });
}

AsyncWriter::~AsyncWriter()
{
	stopping_.store(true, std::memory_order_release);
	// push empty sentinel
	queue_.push(core::Event{});
	if (worker_.joinable()) worker_.join();
}

void AsyncWriter::push_event(const core::Event& ev)
{
	queue_.push(ev);
}

void AsyncWriter::run()
{
	while (!stopping_.load(std::memory_order_acquire)) {
		core::Event ev = queue_.wait_and_pop();
		// sentinel: empty event with id==0 and empty type could be used to stop
		if (stopping_.load(std::memory_order_acquire)) break;
		if (storage_) {
			storage_->save_event(ev);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

} // namespace storage
