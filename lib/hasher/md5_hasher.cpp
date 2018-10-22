#include "md5_hasher.h"

hash_result_t Md5Hasher::calculateHash(const char* data, size_t length) const {
	return {};
}

unsigned int Md5Hasher::hashSize() const {
	return 16;
}

namespace {
	int registerBackend() {
		BaseHasher::registerBackend("md5", [] { return BaseHasher::ptr_t(new Md5Hasher()); });
		return 0;
	}

	static int dummy = registerBackend();
}

