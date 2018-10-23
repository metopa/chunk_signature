#include "md5_hasher.h"

#include <cassert>

#include <openssl/md5.h>

void Md5Hasher::calculateHash(const char* data, size_t length, hash_result_t* out) const {
	assert(out->size() >= hashSize());
	MD5(reinterpret_cast<const unsigned char*>(data), length, out->data());
}

unsigned int Md5Hasher::hashSize() const {
	return MD5_DIGEST_LENGTH;
}

namespace {
	int registerBackend() {
		BaseHasher::registerBackend("md5", [] { return BaseHasher::ptr_t(new Md5Hasher()); });
		return 0;
	}

	int dummy = registerBackend();
}

