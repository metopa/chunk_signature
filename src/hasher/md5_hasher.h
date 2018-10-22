#ifndef VEEAM_MD_5_HASHER_H
#define VEEAM_MD_5_HASHER_H


#include "base_hasher.h"

class Md5Hasher : public BaseHasher {
public:
	hash_result_t calculateHash(const char* data, size_t length) const override;
	unsigned int hashSize() const override;
};


#endif //VEEAM_MD_5_HASHER_H
