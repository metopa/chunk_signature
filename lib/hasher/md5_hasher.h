#ifndef VEEAM_MD_5_HASHER_H
#define VEEAM_MD_5_HASHER_H

#include "hasher/base_hasher.h"

#include <openssl/md5.h>

/**
 * MD5 implementation of `BaseHasher` interface
 */
class Md5Hasher : public BaseHasher {
public:
	void init() override;

	void processBlock(const char* data, size_t length) override;

	void finalize(hash_result_t* out) override;

	unsigned int hashSize() const override;

	ptr_t clone() const override;

private:
	MD5_CTX ctx_;
};


#endif //VEEAM_MD_5_HASHER_H
