#ifndef VEEAM_PROGRAM_CONFIG_H
#define VEEAM_PROGRAM_CONFIG_H

#include <string>

/**
 * Program configuration variables.
 */
struct ProgramConfig {
	/**
	 * Init ProgramConfig from commandline parameters.
	 *
	 * Supported options:
  	 *   -h [ --help ]                 produce help message
  	 *   -b [ --backend ] arg          set hash backend, supported [crc32, md5, sha1]
  	 *   -i [ --input ] arg            input file
  	 *   -o [ --output ] arg           output file
  	 *   -s [ --chunk-size ] arg (=1m) set chunk size
  	 *   -j [ --workers ] arg (=0)     set worker count, default: number of HW cores
	 *
	 * @param argc - number of cl parameters
	 * @param argv - cl parameters
	 */
	static ProgramConfig fromCommandLine(int argc, char* argv[]);

	std::string input_file;
	std::string output_file;
	std::string backend_name;
	uint64_t chunk_size;
	unsigned thread_count;
};

/**
 * Print ProgramConfig.
 */
std::ostream& operator<<(std::ostream& out, const ProgramConfig& config);

/**
 * Convert size literal to the corresponding multiplier.
 *
 * Examples:
 *   ""   => 1
 *   "b"  => 1
 *   "B"  => 1
 *   "k"  => 1024
 *   "kb" => 1024
 *   "mb" => 1024^2
 *   "gb" => 1024^3
 */
uint64_t sizeLiteralToNum(std::string size_literal);


/**
 * Convert either an integral or a float size
 * with possible size literal to the number of bytes.
 *
 * Examples:
 * 	"100" => 100
 * 	"100 b" => 100
 * 	"100 kb" => 102400
 * 	"0.5 kb" => 512
 *
 * @see sizeLiteralToNum()
 */
uint64_t sizeStringToNum(const std::string& size_str);


#endif //VEEAM_PROGRAM_CONFIG_H
