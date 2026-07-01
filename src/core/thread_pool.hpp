// thread_pool.hpp
#pragma once

#include "concurrent_queue.hpp"

#include <atomic>
#include <future>
#include <functional>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

namespace core {

class ThreadPool {
public:
	explicit ThreadPool(size_t threads = std::thread::hardware_concurrency());
	~ThreadPool();

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;

	// Submit a task to the pool and get a future for the result.
	template <class F, class... Args>
	auto submit(F&& f, Args&&... args)
		-> std::future<std::invoke_result_t<F, Args...>>
	{
		using R = std::invoke_result_t<F, Args...>;
		auto task = std::make_shared<std::packaged_task<R()>>(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...));

		std::future<R> res = task->get_future();

		tasks_.push([task]() { (*task)(); });
		return res;
	}

	// Number of worker threads
	size_t thread_count() const noexcept { return workers_.size(); }

private:
	void worker_loop();

	std::vector<std::thread> workers_;
	core::concurrent_queue<std::function<void()>> tasks_;
	std::atomic<bool> stopping_{false};
};

} // namespace core
