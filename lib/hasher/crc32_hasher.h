#ifndef VEEAM_CRC_HASHER_H
#define VEEAM_CRC_HASHER_H


#include <boost/crc.hpp>

#include "hasher/base_hasher.h"

/**
 * CRC32 implementation of `BaseHasher` interface
 */
class Crc32Hasher : public BaseHasher {
public:
	void init() override;

	void processBlock(const char* data, size_t length) override;

	void finalize(hash_result_t* out) override;

	unsigned int hashSize() const override;

	ptr_t clone() const override;

private:
	boost::crc_32_type ctx_;
};


#endif //VEEAM_CRC_HASHER_H
