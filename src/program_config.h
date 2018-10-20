#ifndef VEEAM_PROGRAM_CONFIG_H
#define VEEAM_PROGRAM_CONFIG_H

#include <string>
#include <boost/noncopyable.hpp>

struct ProgramConfig {
	static ProgramConfig fromCommandLine(int argc, char* argv[]);

	std::string input_file;
	std::string output_file;
	size_t chunk_size;
};


#endif //VEEAM_PROGRAM_CONFIG_H
