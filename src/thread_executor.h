#ifndef VEEAM_SIGNATURE_THREAD_EXECUTOR_H
#define VEEAM_SIGNATURE_THREAD_EXECUTOR_H

#include <functional>

/**
 * Run `func` in parallel
 * @param thread_count - thread count,
 * 					     if 0, number of hw threads is used
 * @param func - user function with signature
 * 				 void (*)(unsigned thread_id, unsigned thread_count)
 */
void parallelRun(unsigned thread_count,
				 std::function<void(unsigned, unsigned)> func);


#endif //VEEAM_SIGNATURE_THREAD_EXECUTOR_H
