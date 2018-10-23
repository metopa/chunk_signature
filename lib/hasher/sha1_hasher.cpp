#include "sha1_hasher.h"

#include <cassert>

#include <openssl/sha.h>

void Sha1Hasher::calculateHash(const char* data, size_t length, hash_result_t* out) const {
	assert(out->size() >= hashSize());
	SHA1(reinterpret_cast<const unsigned char*>(data), length, out->data());
}

unsigned int Sha1Hasher::hashSize() const {
	return SHA_DIGEST_LENGTH;
}

namespace {
	int registerBackend() {
		BaseHasher::registerBackend("sha1", [] { return BaseHasher::ptr_t(new Sha1Hasher()); });
		return 0;
	}

	int dummy = registerBackend();
}

