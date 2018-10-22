#ifndef VEEAM_BASE_HASHER_H
#define VEEAM_BASE_HASHER_H

#include <cstdint>
#include <memory>
#include <vector>

#include "../common.h"

class BaseHasher {
public:
	using ptr_t = std::shared_ptr<BaseHasher>;

	static std::vector<std::string> getSupportedBackends();
	static ptr_t getBackendByName(const std::string& name);


	virtual ~BaseHasher() = default;
	virtual hash_result_t calculateHash(const char* data, size_t length) const = 0;
	virtual unsigned hashSize() const = 0;
};


#endif //VEEAM_BASE_HASHER_H
