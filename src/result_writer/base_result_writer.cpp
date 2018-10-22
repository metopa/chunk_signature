#include <stdexcept>
#include "base_result_writer.h"

#include <boost/format.hpp>

void BaseResultWriter::writeResult(const hash_result_t& hash, size_t index) const {
	if (hash.size() != hash_length_) {
		throw std::runtime_error((boost::format("BaseResultWriter: bad hash length: "
												"expected %1%, got %2%")
								  % hash_length_ % hash.size()).str());
	}

	if (index >= hash_count_) {
		throw std::runtime_error((boost::format("BaseResultWriter: bad hash index: "
												"expected %1%, got %2%")
								  % hash_count_ % index).str());
	}

	doWriteResult(hash, index);
}
