#include <iostream>

#include "file_chunker.h"
#include "hasher/base_hasher.h"
#include "program_config.h"
#include "result_writer/m_mapped_result_writer.h"

/**
 * Calculate hashes for each chunk of an input file and write result to an output file.
 *
 * @param config - program configuration
 * @param hasher - hashing algorithm
 * @param input - input file chunk provider
 * @param output - result writer
 * @param verbose - enables progress logging
 */
void process(const ProgramConfig& config,
			 const BaseHasher::ptr_t& hasher,
			 FileChunker& input,
			 MMappedResultWriter& output,
			 bool verbose) {
	std::atomic<size_t> processed_blocks;
	processed_blocks = 0;
	const size_t chunk_count = input.chunkCount();

	#pragma omp parallel
	{
		hash_result_t hash(hasher->hashSize());

		#pragma omp for schedule(static)
		for (size_t i = 0; i < chunk_count; i++) {
			auto chunk = input.getChunk(i);
			hasher->calculateHash(chunk.data, chunk.size, &hash);
			output.writeResult(hash, i);

			if (verbose) {
				processed_blocks++;

				if (true) {
					std::cout << "processed " << processed_blocks << " of " << chunk_count << "\r";
					std::cout.flush();
				};
			}
		}
	}

	if (verbose) {
		std::cout << "processed " << processed_blocks << " of " << chunk_count << std::endl;
	}
}


int main(int argc, char* argv[]) {
	try {
		auto config = ProgramConfig::fromCommandLine(argc, argv);

		auto backend = BaseHasher::getBackendByName(config.backend_name);
		FileChunker input(config.input_file, config.chunk_size);
		MMappedResultWriter output(config.output_file, input.chunkCount(), backend->hashSize());

		process(config, backend, input, output, true);
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
