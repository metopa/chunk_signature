#include "md5_hasher.h"

#include <openssl/md5.h>

hash_result_t Md5Hasher::calculateHash(const char* data, size_t length) const {
	hash_result_t hash(hashSize());
	MD5(reinterpret_cast<const unsigned char*>(data), length, hash.data());
	return hash;
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

