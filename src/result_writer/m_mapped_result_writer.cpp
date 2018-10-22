#include "m_mapped_result_writer.h"

#include <boost/iostreams/device/mapped_file.hpp>
namespace bio = boost::iostreams;

#ifdef _WIN32
const std::string NEWLINE = "\r\n";
#else
const std::string NEWLINE = "\n";
#endif

MMappedResultWriter::MMappedResultWriter(const std::string& filename,
										 size_t hash_count,
										 size_t hash_length) :
		BaseResultWriter(hash_count, hash_length) {
	line_length_ = hash_length * 2 + NEWLINE.size();

	bio::mapped_file_params params;
	params.path = filename;
	params.new_file_size = line_length_ * hash_count;
	params.flags = bio::mapped_file::mapmode::readwrite;

	file_.open(params);
	if (!file_.is_open()) {
		throw std::runtime_error("Can't open " + filename + " for writing");
	}
}

MMappedResultWriter::~MMappedResultWriter() {
	file_.close();
}


void writeByteHex(char* out, unsigned char byte) {
	constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7',
							   '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

	out[0] = hexmap[(byte & 0xF0) >> 4];
	out[1] = hexmap[byte & 0x0F];
}

void MMappedResultWriter::doWriteResult(const hash_result_t& hash, size_t index) const {
	auto data = file_.data() + line_length_ * index;
	for (auto b : hash) {
		writeByteHex(data, b);
		data += 2;
	}

	std::copy(begin(NEWLINE), end(NEWLINE), data);
}
