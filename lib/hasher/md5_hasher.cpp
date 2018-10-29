#include "md5_hasher.h"

#include <cassert>

unsigned int Md5Hasher::hashSize() const {
	return MD5_DIGEST_LENGTH;
}

void Md5Hasher::init() {
	MD5_Init(&ctx_);
}

void Md5Hasher::processBlock(const char* data, size_t length) {
	MD5_Update(&ctx_, data, length);
}

void Md5Hasher::finalize(hash_result_t* out) {
	assert(out->size() >= hashSize());

	MD5_Final(out->data(), &ctx_);
}

BaseHasher::ptr_t Md5Hasher::clone() const {
	return BaseHasher::ptr_t(new Md5Hasher());
}

namespace {
	int registerBackend() {
		BaseHasher::registerBackend("md5", BaseHasher::ptr_t(new Md5Hasher()));
		return 0;
	}

	int dummy = registerBackend();
}

