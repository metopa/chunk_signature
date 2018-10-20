#include "program_config.h"

#include <iostream>
#include <map>
#include <regex>

#define BOOST_SPIRIT_USE_PHOENIX_V3 1
#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include <boost/spirit/include/qi.hpp>

size_t sizeLiteralToNum(std::string size_literal) {
	static std::map<std::string, size_t> LITERAL2MULTIPLIER{
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
		throw std::runtime_error("can't decode size literal: "
								 "expected one of {b, kb, mb, gb}, "
								 "got: " + size_literal);
	}

	return it->second;

}

size_t sizeStringToNum(const std::string& size_str) {
	namespace qi = boost::spirit::qi;
	namespace ph = boost::phoenix;

	size_t int_val = 0;
	double double_val = 0;
	std::string literal;

	// First try to decode an exact integral number
	// to avoid float point error
	auto exact_parser =
			qi::ulong_long[ph::ref(int_val) = qi::_1]
					>> -qi::string[ph::ref(literal) = qi::_1];

	auto b = begin(size_str);
	auto e = end(size_str);

	bool success = qi::phrase_parse(b, e, exact_parser,
									boost::spirit::ascii::space);

	if (b == e && success) {
		return int_val * sizeLiteralToNum(literal);
	}

	// Try to decode a real number, we don't care about rounding
	auto float_parser =
			qi::double_[ph::ref(double_val) = qi::_1]
					>> -qi::string[ph::ref(literal) = qi::_1];

	b = begin(size_str);
	e = end(size_str);

	success = qi::phrase_parse(b, e, float_parser,
							   boost::spirit::ascii::space);

	if (b == e && success && double_val >= 0) {
		return round(double_val * sizeLiteralToNum(literal));
	} else {
		throw std::runtime_error(
				"can't decode size: "
				"expected format: <number> <one of {b, kb, mb, gb}>, "
				"got: " + size_str);
	}

}

ProgramConfig ProgramConfig::fromCommandLine(int argc, char** argv) {
	namespace po = boost::program_options;

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

	try {
		config.chunk_size = sizeStringToNum(chunk_str);
		if (config.chunk_size == 0) {
			throw std::runtime_error("--chunk-size: chunk size must be positive");
		}
	} catch (std::runtime_error& e) {
		throw std::runtime_error("--chunk-size: " + std::string(e.what()));
	}

	return config;
}

std::ostream& operator<<(std::ostream& out, const ProgramConfig& config) {
	return out << boost::format("Config{chunk_size=%1%, input=%2%, output=%3%}") %
				  config.chunk_size % config.input_file % config.output_file;
}
