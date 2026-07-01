// concurrent_queue.hpp
#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <optional>

namespace core {

template <typename T>
class concurrent_queue {
public:
	concurrent_queue() = default;
	concurrent_queue(const concurrent_queue&) = delete;
	concurrent_queue& operator=(const concurrent_queue&) = delete;

	void push(T value)
	{
		{
			std::lock_guard<std::mutex> lk(m_);
			q_.push(std::move(value));
		}
		cv_.notify_one();
	}

	// Blocks until an element is available and pops it.
	T wait_and_pop()
	{
		std::unique_lock<std::mutex> lk(m_);
		cv_.wait(lk, [this] { return !q_.empty(); });
		T val = std::move(q_.front());
		q_.pop();
		return val;
	}

	// Try to pop without blocking. Returns optional with value if present.
	std::optional<T> try_pop()
	{
		std::lock_guard<std::mutex> lk(m_);
		if (q_.empty()) return std::nullopt;
		T val = std::move(q_.front());
		q_.pop();
		return val;
	}

	bool empty() const
	{
		std::lock_guard<std::mutex> lk(m_);
		return q_.empty();
	}

	size_t size() const
	{
		std::lock_guard<std::mutex> lk(m_);
		return q_.size();
	}

private:
	mutable std::mutex m_;
	std::condition_variable cv_;
	std::queue<T> q_;
};

} // namespace core
