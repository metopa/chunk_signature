#include "m_mapped_result_writer.h"

#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/iostreams/filter/newline.hpp>
namespace bio = boost::iostreams;


class MMappedResultWriter::Impl {
public:
	Impl(const std::string& filename, size_t hash_count, size_t hash_length);

	~Impl();
	void doWriteResult(const hash_result_t& hash, size_t index) const;

private:
	bio::mapped_file_sink file_;
	size_t line_length_;
};


MMappedResultWriter::MMappedResultWriter(const std::string& filename,
										 size_t hash_count,
										 size_t hash_length) :
		BaseResultWriter(hash_count, hash_length),
		pimpl_(new Impl(filename, hash_count, hash_length)) {}


MMappedResultWriter::Impl::Impl(const std::string& filename,
								size_t hash_count,
								size_t hash_length) {
	std::string nl = "\n";
	line_length_ = hash_length * 2 + nl.size();
	bio::mapped_file_params params;
	params.path = filename;
	params.new_file_size = line_length_ * hash_count;
	params.flags = bio::mapped_file::mapmode::readwrite;
	file_.open(params);
	if (!file_.is_open()) {
		throw std::runtime_error("Can't open " + filename + " for writing");
	}
}

MMappedResultWriter::Impl::~Impl() {
	file_.close();
}

MMappedResultWriter::~MMappedResultWriter() = default;

void MMappedResultWriter::doWriteResult(const hash_result_t& hash, size_t index) const {
	pimpl_->doWriteResult(hash, index);
}


void writeByteHex(char* out, unsigned char byte) {
	constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7',
							   '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

	out[0] = hexmap[(byte & 0xF0) >> 4];
	out[1] = hexmap[byte & 0x0F];
}

void MMappedResultWriter::Impl::doWriteResult(const hash_result_t& hash, size_t index) const {
	auto data = file_.data() + line_length_ * index;
	for (auto b : hash) {
		writeByteHex(data, b);
		data += 2;
	}
	*data = '\n';
}
