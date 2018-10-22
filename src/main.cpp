#include <iostream>
#include <omp.h>

#include "file_chunker.h"
#include "hasher/base_hasher.h"
#include "program_config.h"
#include "result_writer/m_mapped_result_writer.h"


int main(int argc, char* argv[]) {
	try {
		auto config = ProgramConfig::fromCommandLine(argc, argv);

		auto backend = BaseHasher::getBackendByName(config.backend_name);
		FileChunker input(config.input_file, config.chunk_size);
		MMappedResultWriter writer(config.output_file, input.chunkCount(), backend->hashSize());

		if (config.thread_count != 0) {
			omp_set_num_threads(config.thread_count);
		}

		std::atomic<size_t> processed_blocks;
		processed_blocks = 0;
		const size_t chunk_count = input.chunkCount();

		#pragma omp parallel
		{
			hash_result_t hash(backend->hashSize());

			#pragma omp for schedule(static)
			for (size_t i = 0; i < chunk_count; i++) {
				auto chunk = input.getChunk(i);
				backend->calculateHash(chunk.data, chunk.size, hash);
				writer.writeResult(hash, i);

				processed_blocks++;

				if (omp_get_thread_num() == 0) {
					std::cout << "processed " << processed_blocks << " of " << chunk_count << "\r";
					std::cout.flush();
				};
			}
		}

		std::cout << "processed " << processed_blocks << " of " << chunk_count << std::endl;
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}


	return 0;
}
