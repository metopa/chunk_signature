#include <iostream>

#include "file_chunker.h"
#include "hasher/base_hasher.h"
#include "program_config.h"
#include "result_writer/m_mapped_result_writer.h"
#include "thread_executor.h"

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
			 const std::string& hasher_name,
			 FileChunker& input,
			 MMappedResultWriter& output,
			 bool verbose) {
	std::atomic<size_t> processed_blocks;
	processed_blocks = 0;
	const size_t chunk_count = input.chunkCount();

	parallelRun(
			config.thread_count,
			[&](unsigned thread_id, unsigned thread_count) {
				auto hasher = BaseHasher::createBackendByName(hasher_name);
				hash_result_t hash(hasher->hashSize());

				for (size_t i = thread_id;
					 i < chunk_count;
					 i += thread_count) {

					auto chunk = input.getChunk(i);
					hasher->init();
					hasher->processBlock(chunk.data, chunk.size);
					hasher->finalize(&hash);
					output.writeResult(hash, i);

					if (verbose) {
						processed_blocks++;

						if (thread_id == 0) {
							std::cout << "processed " <<
									  processed_blocks << " of "
									  << chunk_count << "\r";
							std::cout.flush();
						};
					}
				}
			});

	if (verbose) {
		std::cout << "processed " << processed_blocks << " of " << chunk_count << std::endl;
	}
}


int main(int argc, char* argv[]) {
	try {
		auto config = ProgramConfig::fromCommandLine(argc, argv);

		auto hash_size = BaseHasher::getStaticBackendByName(config.backend_name)->hashSize();
		FileChunker input(config.input_file, config.chunk_size);
		MMappedResultWriter output(config.output_file, input.chunkCount(), hash_size);

		process(config, config.backend_name, input, output, true);
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
