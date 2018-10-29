#ifndef VEEAM_SIGNATURE_PREFETCH_FILE_READER_H
#define VEEAM_SIGNATURE_PREFETCH_FILE_READER_H


#include <cstdio>
#include <string>
#include <vector>

class PrefetchFileReader {
public:
	PrefetchFileReader(const std::string& filename, size_t max_prefetch_size);

	~PrefetchFileReader();

	void advance();

	const char* data() const;

	size_t pos() const;

	size_t size() const;

	bool isEof() const;

private:
	size_t fetch(char* data, size_t len);

	const std::string filename_;
	const size_t max_prefetch_size_;

	std::FILE* file_;
	size_t current_pos_;
	size_t current_size_;
	std::vector<char> buffer_;
};


#endif //VEEAM_SIGNATURE_PREFETCH_FILE_READER_H
