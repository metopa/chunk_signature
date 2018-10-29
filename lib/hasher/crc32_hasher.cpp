#include "crc32_hasher.h"

#include <cassert>

#include <boost/crc.hpp>
#include <boost/endian/conversion.hpp>

unsigned int Crc32Hasher::hashSize() const {
	return 4;
}

void Crc32Hasher::init() {
	ctx_.reset();
}

void Crc32Hasher::processBlock(const char* data, size_t length) {
	ctx_.process_bytes(data, length);
}

void Crc32Hasher::finalize(hash_result_t* out) {
	assert(out->size() >= hashSize());

	uint32_t value = static_cast<uint32_t>(ctx_.checksum());

	value = boost::endian::endian_reverse(value);
	char* bytes = reinterpret_cast<char*>(&value);
	std::copy(bytes, bytes + 4, out->data());
}

BaseHasher::ptr_t Crc32Hasher::clone() const {
	return ptr_t(new Crc32Hasher());
}

namespace {
	int registerBackend() {
		BaseHasher::registerBackend("crc32", BaseHasher::ptr_t(new Crc32Hasher()));
		return 0;
	}

	int dummy = registerBackend();
}
