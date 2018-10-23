#ifndef VEEAM_MD_5_HASHER_H
#define VEEAM_MD_5_HASHER_H

#include "hasher/base_hasher.h"

/**
 * MD5 implementation of `BaseHasher` interface
 */
class Md5Hasher : public BaseHasher {
public:
	void calculateHash(const char* data, size_t length, hash_result_t* out) const override;

	unsigned int hashSize() const override;
};


#endif //VEEAM_MD_5_HASHER_H
