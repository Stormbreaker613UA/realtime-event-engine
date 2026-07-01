// async_writer.hpp
#pragma once

#include "istorage.hpp"

#include "../core/concurrent_queue.hpp"

#include <atomic>
#include <memory>
#include <thread>

namespace storage {

class AsyncWriter {
public:
	explicit AsyncWriter(IStoragePtr storage);
	~AsyncWriter();

	AsyncWriter(const AsyncWriter&) = delete;
	AsyncWriter& operator=(const AsyncWriter&) = delete;

	void push_event(const core::Event& ev);

private:
	void run();

	IStoragePtr storage_;
	core::concurrent_queue<core::Event> queue_;
	std::thread worker_;
	std::atomic<bool> stopping_{false};
};

} // namespace storage
