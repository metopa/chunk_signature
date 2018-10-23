#ifndef VEEAM_BASE_RESULT_WRITER_H
#define VEEAM_BASE_RESULT_WRITER_H


#include "hasher/base_hasher.h"

/**
 * Base class for writing calculated hashes to an output file in random order.
 *
 * Hashes can be added in any order and from multiple threads without blocking.
 */
class BaseResultWriter {
public:
	/**
	 * Constructor.
	 * @param hash_count - total number of hashes to be added
	 * @param hash_length - fixed byte length of a hash
	 */
	BaseResultWriter(size_t hash_count, size_t hash_length) :
		hash_count_(hash_count), hash_length_(hash_length) {}

	/**
	 * Virtual dtor
	 */
	virtual ~BaseResultWriter() = default;

	/**
	 * Add new hash to the result.
	 *
	 * THis function is can be called concurrently as long as
	 * different threads invoke it with different `index` values.
	 *
	 * @param hash - hash bytes
	 * @param index - index of the hash in the output file
	 */
	void writeResult(const hash_result_t& hash, size_t index) const;

protected:
	/**
	 * Virtual implementation for `writeResult`.
	 *
	 * @param hash - hash bytes
	 * @param index - index of the hash in the output file
	 */
	virtual void doWriteResult(const hash_result_t& hash, size_t index) const = 0;

	size_t hash_count_;
	size_t hash_length_;
};


#endif //VEEAM_BASE_RESULT_WRITER_H
