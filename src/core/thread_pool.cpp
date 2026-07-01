// thread_pool.cpp
#include "thread_pool.hpp"

#include <cassert>

namespace core {

ThreadPool::ThreadPool(size_t threads)
{
	if (threads == 0) threads = 1;
	workers_.reserve(threads);
	for (size_t i = 0; i < threads; ++i) {
		workers_.emplace_back([this] { this->worker_loop(); });
	}
}

ThreadPool::~ThreadPool()
{
	stopping_.store(true, std::memory_order_release);
	// wake up all workers by pushing empty tasks (sentinels)
	for (size_t i = 0; i < workers_.size(); ++i) {
		tasks_.push(std::function<void()>());
	}

	for (auto& t : workers_) {
		if (t.joinable()) t.join();
	}
}

void ThreadPool::worker_loop()
{
	while (true) {
		auto task = tasks_.wait_and_pop();
		// empty task used as shutdown sentinel
		if (!task) {
			if (stopping_.load(std::memory_order_acquire)) break;
			else continue;
		}

		try {
			task();
		} catch (...) {
			// swallow exceptions - tasks should handle their own errors
		}
	}
}

} // namespace core
