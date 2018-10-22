#include <iostream>

#include "file_chunker.h"
#include "hasher/base_hasher.h"
#include "program_config.h"
#include "result_writer/m_mapped_result_writer.h"


int main(int argc, char* argv[]) {
	try {
		auto config = ProgramConfig::fromCommandLine(argc, argv);
		std::cout << config << std::endl;

		auto backend = BaseHasher::getBackendByName(config.backend_name);
		FileChunker input(config.input_file, config.chunk_size);
		MMappedResultWriter writer(config.output_file, input.chunkCount(), backend->hashSize());

		for (size_t i = 0; i < input.chunkCount(); i++) {
			auto chunk = input.getChunk(i);
			auto hash = backend->calculateHash(chunk.data, chunk.size);
			writer.writeResult(hash, i);
		}
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}


	return 0;
}
