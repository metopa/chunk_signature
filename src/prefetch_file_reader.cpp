#include "prefetch_file_reader.h"

#include <stdexcept>
#include <cstring>

PrefetchFileReader::PrefetchFileReader(const std::string& filename,
									   size_t max_prefetch_size) :
		filename_(filename), max_prefetch_size_(max_prefetch_size) {
	file_ = std::fopen(filename.c_str(), "rb");

	if (!file_) {
		throw std::runtime_error(filename + ": file opening failed: " + std::strerror(errno));
	}

	current_pos_ = 0;
	try {
		buffer_.resize(max_prefetch_size);

		current_size_ = fetch(buffer_.data(), buffer_.size());
	} catch (...) {
		std::fclose(file_);
	}
}

PrefetchFileReader::~PrefetchFileReader() {
	std::fclose(file_);
}

void PrefetchFileReader::advance() {
	if (isEof()) {
		return;
	}

	current_pos_ += current_size_;
	current_size_ = fetch(buffer_.data(), buffer_.size());
}

const char* PrefetchFileReader::data() const {
	return buffer_.data();
}

size_t PrefetchFileReader::pos() const {
	return current_pos_;
}

size_t PrefetchFileReader::size() const {
	return current_size_;
}

bool PrefetchFileReader::isEof() const {
	return current_size_ == 0 && std::feof(file_) != 0;
}

size_t PrefetchFileReader::fetch(char* data, size_t len) {
	auto actual_len = std::fread(data, 1, len, file_);
	if (std::ferror(file_)) {
		throw std::runtime_error(filename_ + ": io error:");
	}
	return actual_len;

}

