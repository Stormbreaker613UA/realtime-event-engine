// test_thread_pool.cpp
#include "../core/thread_pool.hpp"

#include <cassert>
#include <chrono>

int main()
{
	core::ThreadPool tp(2);
	auto f1 = tp.submit([] { return 42; });
	auto f2 = tp.submit([](int a, int b) { return a + b; }, 2, 3);
	assert(f1.get() == 42);
	assert(f2.get() == 5);
	return 0;
}
