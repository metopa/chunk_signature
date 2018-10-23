#ifndef VEEAM_FILE_CHUNKER_H
#define VEEAM_FILE_CHUNKER_H


#include <boost/iostreams/device/mapped_file.hpp>

/**
 * FileChunker opens a file and allows to iterate over fix-sized chunks
 * (last chunk can be shorter, than others.
 */
class FileChunker {
public:
	struct Chunk {
		const char* data;
		size_t size;
	};

	/**
	 * Constructor.
	 *
	 * @param filename - path to a file to be opened.
	 * @param chunk_size - max size of a chunk.
	 */
	FileChunker(const std::string& filename, size_t chunk_size);

	/**
	 * Destructor.
	 */
	~FileChunker();

	/**
	 * Max size of a chunk.
	 */
	size_t maxChunkSize() const;

	/**
	 * Total number of chunks.
	 * Equals to ceil(file size / chunk size)
	 */
	size_t chunkCount() const;

	/**
	 * Get i-th chunk.
	 * This function can be called concurrently.
	 *
	 * @param index - chunk index
	 * @return Chunk - pointer to the chunk and its size
	 */
	Chunk getChunk(size_t index) const;

private:
	size_t chunk_size_;
	boost::iostreams::mapped_file_source file_;
};


#endif //VEEAM_FILE_CHUNKER_H
