#ifndef VEEAM_CRC_HASHER_H
#define VEEAM_CRC_HASHER_H


#include "hasher/base_hasher.h"

class Crc32Hasher : public BaseHasher {
public:
	hash_result_t calculateHash(const char* data, size_t length) const override;

	unsigned int hashSize() const override;
};


#endif //VEEAM_CRC_HASHER_H
