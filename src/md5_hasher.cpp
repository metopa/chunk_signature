#include "md5_hasher.h"

hash_result_t Md5Hasher::calculateHash(char* data, size_t length) const {
	return std::vector<uint8_t>();
}

unsigned int Md5Hasher::hashSize() const {
	return 16;
}
