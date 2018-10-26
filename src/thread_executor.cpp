#include <functional>
#include <thread>
#include <vector>

void parallelRun(unsigned thread_count,
				 std::function<void(unsigned, unsigned)> func) {
	if (thread_count == 0) {
		thread_count = std::thread::hardware_concurrency();
	}

	if (thread_count == 1) {
		func(0, 1);
	} else {
		std::vector<std::thread> threads;

		for (unsigned i = 0; i < thread_count; i++) {
			threads.emplace_back(std::bind(func, i, thread_count));
		}

		for (auto& th : threads) {
			th.join();
		}
	}
}
