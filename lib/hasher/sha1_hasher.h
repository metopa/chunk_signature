#ifndef VEEAM_SHA1_HASHER_H
#define VEEAM_SHA1_HASHER_H


#include "hasher/base_hasher.h"

class Sha1Hasher : public BaseHasher {
public:
	void calculateHash(const char* data, size_t length, hash_result_t& out) const override;
	unsigned int hashSize() const override;
};


#endif //VEEAM_SHA1_HASHER_H
