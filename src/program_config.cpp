#include "program_config.h"

#include <iostream>
#include <map>

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <boost/xpressive/xpressive_static.hpp>

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


uint64_t sizeStringToNum(const std::string& size_str) {
	using namespace boost::xpressive;

	mark_tag num_tag(1), literal_tag(2);
	sregex cre_num_literal = *_s >> (num_tag = +_d >> !('.' >> +_d)) >>
								 *_s >> (literal_tag = *_w) >> *_s;
	smatch matches;

	if (!regex_match(size_str, matches, cre_num_literal)) {
		throw std::runtime_error(
				"can't decode size: "
				"expected format: <number> <one of {b, kb, mb, gb}>, "
				"got: " + size_str);
	}

	auto multiplier = sizeLiteralToNum(matches[literal_tag]);
	auto num_str = matches[num_tag];

	try {
		return boost::lexical_cast<size_t>(num_str) * multiplier;
	} catch (boost::bad_lexical_cast&) {
		// ignore it, try as double
	}

	double result = 0;
	try {
		result = boost::lexical_cast<double>(num_str) * multiplier;
	} catch (boost::bad_lexical_cast&) {
		throw std::runtime_error(
				"can't decode size: "
				"expected format: <number> <one of {b, kb, mb, gb}>, "
				"got: " + size_str);
	}

	if (result < 0) {
		throw std::runtime_error("can't decode size: float size < 0");
	}
	if (result >= std::numeric_limits<size_t>::max()) {
		throw std::runtime_error("can't decode size: float size is too big");
	}
	return size_t(std::round(result));
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
	std::string backend_param_desc = "set hash backend, supported " + backends_str;

	desc.add_options()
			("help,h", "produce help message")
			("backend,b",
			 po::value(&config.backend_name)->required(),
			 backend_param_desc.c_str())
			("input,i",
			 po::value(&config.input_file)->required(),
			 "input file")
			("output,o",
			 po::value(&config.output_file)->required(),
			 "output file")
			("chunk-size,s",
			 po::value(&chunk_str)->default_value("1m"),
			 "set chunk size")
			("workers,j",
			 po::value(&config.thread_count)->default_value(0),
			 "set worker count, default: number of HW cores");

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
		throw std::runtime_error("--backend: unknown backend: supported " + backends_str);
	}

	return config;
}

std::ostream& operator<<(std::ostream& out, const ProgramConfig& config) {
	return out << boost::format("Config{chunk_size=%1%, backend=%2%, input=%3%, output=%4%}") %
				  config.chunk_size % config.backend_name % config.input_file % config.output_file;
}
