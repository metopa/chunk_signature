#include <iostream>

#include "program_config.h"
#include "result_writer/m_mapped_result_writer.h"
#include "file_chunker.h"


int main(int argc, char* argv[]) {
	try {
		auto config = ProgramConfig::fromCommandLine(argc, argv);
		std::cout << config << std::endl;

		FileChunker input(config.input_file, config.chunk_size);
		MMappedResultWriter writer(config.output_file, input.chunkCount(), config.chunk_size);

		for (size_t i = 0; i < input.chunkCount(); i++) {
			auto chunk = input.getChunk(i);
			if (chunk.size == config.chunk_size) {
				hash_result_t h((unsigned char*) chunk.data,
								(unsigned char*) chunk.data + chunk.size);

				writer.writeResult(h, i);
			}
		}
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}


	return 0;
}
