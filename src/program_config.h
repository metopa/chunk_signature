#ifndef VEEAM_PROGRAM_CONFIG_H
#define VEEAM_PROGRAM_CONFIG_H

#include <string>


struct ProgramConfig {
	static ProgramConfig fromCommandLine(int argc, char* argv[]);

	std::string input_file;
	std::string output_file;
	uint64_t chunk_size;
	unsigned thread_count;
};


std::ostream& operator<<(std::ostream& out, const ProgramConfig& config);

uint64_t sizeLiteralToNum(std::string size_literal);

uint64_t exactSizeStringToNum(const std::string& size_str);

uint64_t floatSizeStringToNum(const std::string& size_str);

uint64_t sizeStringToNum(const std::string& size_str);


#endif //VEEAM_PROGRAM_CONFIG_H
