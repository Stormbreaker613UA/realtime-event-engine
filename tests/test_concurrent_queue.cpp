// test_concurrent_queue.cpp
#include "core/concurrent_queue.hpp"

#include <cassert>
#include <thread>

int main()
{
	core::concurrent_queue<int> q;
	q.push(1);
	auto v = q.try_pop();
	assert(v && *v == 1);

	q.push(2);
	int val = q.wait_and_pop();
	assert(val == 2);
	return 0;
}
