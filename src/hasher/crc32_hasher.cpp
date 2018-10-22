#include "crc32_hasher.h"

hash_result_t Crc32Hasher::calculateHash(const char* data, size_t length) const {
	return std::vector<uint8_t>();
}

unsigned int Crc32Hasher::hashSize() const {
	return 4;
}
