#ifndef VEEAM_FILE_CHUNKER_H
#define VEEAM_FILE_CHUNKER_H


#include <boost/iostreams/device/mapped_file.hpp>

class FileChunker {
public:
	struct Chunk {
		const char* data;
		size_t size;
	};

	FileChunker(const std::string& filename, size_t chunk_size);
	~FileChunker();

	size_t maxChunkSize() const;
	size_t chunkCount() const;
	Chunk getChunk(size_t index) const;

private:
	size_t chunk_size_;
	boost::iostreams::mapped_file_source file_;
};


#endif //VEEAM_FILE_CHUNKER_H
