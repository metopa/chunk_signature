#ifndef VEEAM_CRC_HASHER_H
#define VEEAM_CRC_HASHER_H


#include "hasher/base_hasher.h"

/**
 * CRC32 implementation of `BaseHasher` interface
 */
class Crc32Hasher : public BaseHasher {
public:
	void calculateHash(const char* data, size_t length, hash_result_t* out) const override;
	unsigned int hashSize() const override;
};


#endif //VEEAM_CRC_HASHER_H
