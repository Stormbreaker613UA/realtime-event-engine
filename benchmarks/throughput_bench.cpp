// throughput_bench.cpp
#include "../core/event.hpp"

#include <chrono>
#include <iostream>

int main()
{
	const int N = 100000;
	core::Event ev;
	ev.type = "bench";
	ev.payload = std::vector<uint8_t>(128, 0);

	auto t0 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; ++i) {
		// pretend to process event
		volatile auto s = ev.payload.size();
		(void)s;
	}
	auto t1 = std::chrono::high_resolution_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
	std::cout << "Processed " << N << " events in " << ms << " ms\n";
	return 0;
}
