#include "program_config.h"

#include <iostream>
#include <map>
#include <regex>

#include <boost/format.hpp>
#include <boost/phoenix/core.hpp>
#include <boost/program_options.hpp>
#include <boost/spirit/include/qi.hpp>

#include "hasher/base_hasher.h"

uint64_t sizeLiteralToNum(std::string size_literal) {
	static std::map<std::string, uint64_t> LITERAL2MULTIPLIER{
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

uint64_t exactSizeStringToNum(const std::string& size_str) {
	namespace qi = boost::spirit::qi;
	namespace ph = boost::phoenix;

	uint64_t val = 0;
	std::string literal;

	auto assign_val = [&](auto a) { val = a; };
	auto append_literal = [&](auto a) { literal.push_back(a); };

	auto parser = qi::ulong_long[assign_val] >> *qi::alpha[append_literal];
	auto b = begin(size_str);
	auto e = end(size_str);

	bool success = qi::phrase_parse(b, e, parser,
									boost::spirit::ascii::space);

	if (b == e && success) {
		return val * sizeLiteralToNum(literal);
	}

	throw std::runtime_error(
			"can't decode size: "
			"expected format: <number> <one of {b, kb, mb, gb}>, "
			"got: " + size_str);
}

uint64_t floatSizeStringToNum(const std::string& size_str) {
	namespace qi = boost::spirit::qi;
	namespace ph = boost::phoenix;

	double val = 0;
	std::string literal;

	auto assign_val = [&](auto a) { val = a; };
	auto append_literal = [&](auto a) { literal.push_back(a); };

	auto parser = qi::double_[assign_val] >> *qi::alpha[append_literal];
	auto b = begin(size_str);
	auto e = end(size_str);

	bool success = qi::phrase_parse(b, e, parser,
									boost::spirit::ascii::space);

	if (b == e && success) {
		if (val < 0) {
			throw std::runtime_error("can't decode size: negative number");
		}
		return size_t(round(val * sizeLiteralToNum(literal)));
	}

	throw std::runtime_error(
			"can't decode size: "
			"expected format: <number> <one of {b, kb, mb, gb}>, "
			"got: " + size_str);
}


uint64_t sizeStringToNum(const std::string& size_str) {
	try {
		// First try to decode an exact integral number to avoid floating point error
		return exactSizeStringToNum(size_str);
	} catch (std::runtime_error&) {
		// Try to decode a real number, we don't care about rounding
		return floatSizeStringToNum(size_str);
	}
}


std::string vectorToString(const std::vector<std::string>& v) {
	std::stringstream out;

	out << '[';
	bool first = true;
	for (auto& x : v) {
		if (first) {
			first = false;
		} else {
			out << ", ";
		}

		out << x;
	}

	out << ']';
	return out.str();
}


ProgramConfig ProgramConfig::fromCommandLine(int argc, char** argv) {
	namespace po = boost::program_options;

	po::options_description desc("Allowed options");

	ProgramConfig config;
	std::string chunk_str;

	auto backends = BaseHasher::getSupportedBackends();
	if (backends.empty()) {
		throw std::runtime_error("No backends supported!");
	}
	auto backends_str = vectorToString(backends);
	std::string backend_param_desc = "set hash backend, available options: " + backends_str;

	desc.add_options()
			("help,h", "produce help message")
			("chunk-size,s",
			 po::value(&chunk_str)->default_value("1m"),
			 "set chunk size")
			("workers,j",
			 po::value(&config.thread_count)->default_value(0),
			 "set worker count, default: number of HW cores")
			("backend,b", po::value(&config.backend_name)->default_value(backends[0]),
			 backend_param_desc.c_str())
			("input,i",
			 po::value(&config.input_file)->required(),
			 "input file")
			("output,o",
			 po::value(&config.output_file)->required(),
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
	} catch (std::runtime_error& e) {
		throw std::runtime_error("--chunk-size: " + std::string(e.what()));
	}

	if (config.chunk_size == 0) {
		throw std::runtime_error("--chunk-size: chunk size must be positive");
	}

	if (std::find(begin(backends), end(backends), config.backend_name) == end(backends)) {
		throw std::runtime_error("--backend: unknown backend: supported: " + backends_str);
	}

	return config;
}

std::ostream& operator<<(std::ostream& out, const ProgramConfig& config) {
	return out << boost::format("Config{chunk_size=%1%, backend=%2%, input=%3%, output=%4%}") %
				  config.chunk_size % config.backend_name % config.input_file % config.output_file;
}
