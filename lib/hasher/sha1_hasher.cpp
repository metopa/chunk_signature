#include "sha1_hasher.h"

#include <cassert>

#include <openssl/sha.h>

unsigned int Sha1Hasher::hashSize() const {
	return SHA_DIGEST_LENGTH;
}

void Sha1Hasher::init() {
	SHA1_Init(&ctx_);
}

void Sha1Hasher::processBlock(const char* data, size_t length) {
	SHA1_Update(&ctx_, data, length);
}

void Sha1Hasher::finalize(hash_result_t* out) {
	assert(out->size() >= hashSize());

	SHA1_Final(out->data(), &ctx_);
}

BaseHasher::ptr_t Sha1Hasher::clone() const {
	return BaseHasher::ptr_t(new Sha1Hasher());
}

namespace {
	int registerBackend() {
		BaseHasher::registerBackend("sha1", BaseHasher::ptr_t(new Sha1Hasher()));
		return 0;
	}

	int dummy = registerBackend();
}

