#include "program_config.h"

#include <iostream>
#include <map>
#include <regex>

#include <boost/program_options.hpp>
#include <boost/spirit/include/qi.hpp>


namespace po = boost::program_options;
namespace qi = boost::spirit::qi;

class SizeStringToNumError : public std::runtime_error {
public:
	SizeStringToNumError(const std::string& size_str,
						 const std::string& parameter_name) :
			std::runtime_error(
					parameter_name +
					": unknown parameter format. "
					"Expected format: <number> <one of {b, kb, mb, gb}>. "
					"Got: " +
					size_str) {}
};

size_t sizeLiteralToNum(std::string size_literal,
						const std::string& parameter_name) {
	static std::map <std::string, size_t> LITERAL2MULTIPLIER{
			{"",  1},
			{"k", 1 << 10},
			{"m", 1 << 20},
			{"g", 1 << 30}
	};

	std::transform(begin(size_literal), end(size_literal),
				   begin(size_literal), ::tolower);

	if (!size_literal.empty() && size_literal.back() == 'b') {
		size_literal.pop_back();
	}

	auto it = LITERAL2MULTIPLIER.find(size_literal);
	if (it == end(LITERAL2MULTIPLIER)) {
		throw SizeStringToNumError(size_literal, parameter_name);
	}

	return it->second;

}

size_t sizeStringToNum(const std::string& size_str,
					   const std::string& parameter_name) {
	size_t int_val = 0;
	double double_val = 0;
	std::string literal;

	auto exact_parser = qi::ulong_long[std::ref(int_val) = qi::_1] >> -qi::string;
	auto float_parser = qi::double_ >> -qi::string;

	auto b = begin(size_str);
	auto e = end(size_str);

	bool success = qi::phrase_parse(b, e, exact_parser,
									boost::spirit::ascii::space);

	if (b == e && success) {

	}




	// split to <number> <literal>
	static std::regex split_regex(R"((\d+(?:\.\d*)?)\s*(\w*))");
	std::cmatch match;
	if (!std::regex_match(size_str, match, split_regex)) {
		throw SizeStringToNumError(size_str, parameter_name);
	}


	auto literal_str = match[2].str();
	std::transform(begin(literal_str), end(literal_str),
				   begin(literal_str), tolower);

	if (!literal_str.empty() && literal_str.back() == 'b') {
		literal_str.pop_back();
	}

	auto it = LITERAL2MULTIPLIER.find(literal_str);
	if (it == end(LITERAL2MULTIPLIER)) {
		throw SizeStringToNumError(size_str, parameter_name);
	}

	size_t multiplier = it->second;

	auto num_str = match[1].str();

	if (num_str.find('.') == std::string::npos) {
		// precise mode
		size_t num;

		return num * multiplier;
	} else {
		// rounding mode
		auto num = std::atof(num_str.c_str());
	}


}

ProgramConfig ProgramConfig::fromCommandLine(int argc, char** argv) {
	po::options_description desc("Allowed options");

	ProgramConfig config;
	std::string chunk_str;

	desc.add_options()
			("help,h", "produce help message")
			("chunk-size,s",
			 po::value<std::string>(&chunk_str)->default_value("1m"),
			 "set chunk size")
			("input,i",
			 po::value<std::string>(&config.input_file)->required(),
			 "input file")
			("output,o",
			 po::value<std::string>(&config.output_file)->required(),
			 "output file");

	po::variables_map vm;

	po::store(po::parse_command_line(argc, argv, desc), vm);

	if (vm.count("help")) {
		std::cout << desc << "\n";
		exit(0);
	}

	po::notify(vm);


	return config;
}
