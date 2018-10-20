#include <iostream>


int main(int argc, char* argv[]) {
	po::options_description desc("Allowed options");

	desc.add_options()
			("help,h", "produce help message")
			("chunk-size,s", po::value<std::string>()->default_value("1m"), "set chunk size")
			("input,i", po::value<std::string>()->required(), "input file")
			("output,o", po::value<std::string>()->required(), "output file");






	if (vm.count("compression")) {
		std::cout << "Compression level was set to "
			 << vm["compression"].as<int>() << ".\n";
	} else {
		std::cout << "Compression level was not set.\n";
	}

	return 0;
}
