#include "file_chunker.h"

#include <boost/iostreams/device/mapped_file.hpp>
namespace bio = boost::iostreams;

FileChunker::FileChunker(const std::string& filename, size_t chunk_size) {
	chunk_size_ = chunk_size;

	bio::mapped_file_params params;
	params.path = filename;
	params.flags = bio::mapped_file::mapmode::readonly;

	file_.open(params);
	if (!file_.is_open()) {
		throw std::runtime_error("Can't open " + filename + " for writing");
	}
}

FileChunker::~FileChunker() {
	file_.close();
}

size_t FileChunker::maxChunkSize() const {
	return chunk_size_;
}

size_t FileChunker::chunkCount() const {
	return (file_.size() + chunk_size_ - 1) / chunk_size_;
}

FileChunker::Chunk FileChunker::getChunk(size_t index) const {
	size_t start = index * chunk_size_;
	if (start >= file_.size()) {
		throw std::runtime_error("FileChunker: index out of bounds");
	}

	size_t this_chunk_size = std::min(chunk_size_, file_.size() - start);
	return Chunk{file_.data() + start, this_chunk_size};
}
