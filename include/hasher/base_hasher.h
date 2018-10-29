#ifndef VEEAM_BASE_HASHER_H
#define VEEAM_BASE_HASHER_H

#include <cstdint>
#include <functional>
#include <memory>
#include <vector>


using hash_result_t = std::vector<uint8_t>;

/**
 * BaseHasher defines hashing algorithm API and
 * a static factory of these algorithms.
 *
 * Each hashing algorithm has to invoke
 * `registerBackend` with it's name and a function,
 * that creates an instance of the class.
 */
class BaseHasher : public std::enable_shared_from_this<BaseHasher> {
public:
	using ptr_t = std::shared_ptr<BaseHasher>;

	/**
	 * Get list of registered backend names.
	 */
	static std::vector<std::string> getSupportedBackends();

	/**
	 * Get a specific backend.
	 *
	 * @param name - backend name
	 * @return pointer to a BaseHasher derived class
	 */
	static ptr_t getStaticBackendByName(const std::string& name);

	/**
	 * Get a new instance of a specific backend.
	 *
	 * @param name - backend name
	 * @return pointer to a BaseHasher derived class
	 */
	static ptr_t createBackendByName(const std::string& name);

	/**
	 * Add register new backend in the hasher factory.
	 *
	 * This function is intended to be called on during
	 * application static initialization time.
	 *
	 * @param name - new backend name
	 * @param static_instance - instance of the backend
	 */
	static void registerBackend(const std::string& name, const ptr_t& static_instance);

	/**
	 * Virtual dtor.
	 */
	virtual ~BaseHasher() = default;

	/**
	 * Initialize new hash computation.
	 */
	virtual void init() = 0;

	/**
	 * Calculate hash for a given block of data.
	 *
	 * @param data - pointer to data
	 * @param length - data length
	 */
	virtual void processBlock(const char* data, size_t length) = 0;

	/**
	 * Get resulting hash for previously processed blocks.
	 *
	 * NOTE: `out` size must be at least hashSize()
	 *
	 * @param out - output hash storage
	 */
	virtual void finalize(hash_result_t* out) = 0;

	virtual ptr_t clone() const = 0;

	/**
	 * Get the number of bytes returned by the given hash backend.
	 */
	virtual unsigned hashSize() const = 0;
};


#endif //VEEAM_BASE_HASHER_H
