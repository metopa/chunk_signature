#include <iostream>
#include "program_config.h"


int main(int argc, char* argv[]) {
	try {
		auto config = ProgramConfig::fromCommandLine(argc, argv);
		std::cout << config << std::endl;
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
