#include "crc32_hasher.h"

#include <boost/crc.hpp>
#include <boost/endian/conversion.hpp>

hash_result_t Crc32Hasher::calculateHash(const char* data, size_t length) const {
	boost::crc_32_type crc32;
	crc32.process_bytes(data, length);
	uint32_t value = static_cast<uint32_t>(crc32.checksum());

	value = boost::endian::endian_reverse(value);
	char* bytes = reinterpret_cast<char*>(&value);
	return {bytes, bytes + 4};

}

unsigned int Crc32Hasher::hashSize() const {
	return 4;
}

namespace {
	int registerBackend() {
		BaseHasher::registerBackend("crc32", [] { return BaseHasher::ptr_t(new Crc32Hasher()); });
		return 0;
	}

	int dummy = registerBackend();
}
