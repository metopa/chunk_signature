#ifndef VEEAM_SHA1_HASHER_H
#define VEEAM_SHA1_HASHER_H


#include <openssl/sha.h>

#include "hasher/base_hasher.h"
/**
 * SHA1 implementation of `BaseHasher` interface
 */
class Sha1Hasher : public BaseHasher {
public:

	void init() override;

	void processBlock(const char* data, size_t length) override;

	void finalize(hash_result_t* out) override;

	unsigned int hashSize() const override;

	ptr_t clone() const override;

private:
	SHA_CTX ctx_;
};


#endif //VEEAM_SHA1_HASHER_H
