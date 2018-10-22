#ifndef VEEAM_BASE_HASHER_H
#define VEEAM_BASE_HASHER_H

#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

#include "../../src/common.h"

class BaseHasher {
public:
	using ptr_t = std::shared_ptr<BaseHasher>;

	static std::vector<std::string> getSupportedBackends();
	static ptr_t getBackendByName(const std::string& name);
	static void registerBackend(const std::string& name,
								const std::function<ptr_t()>& creator);

	virtual ~BaseHasher() = default;
	virtual void calculateHash(const char* data, size_t length, hash_result_t& out) const = 0;
	virtual unsigned hashSize() const = 0;
};


#endif //VEEAM_BASE_HASHER_H
