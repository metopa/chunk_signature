#ifndef VEEAM_M_MAPPED_RESULT_WRITER_H
#define VEEAM_M_MAPPED_RESULT_WRITER_H

#include "base_result_writer.h"

#include <memory>

#include <boost/iostreams/device/mapped_file.hpp>


/**
 * ResultWriter implementation using a memory-mapped file
 * for a fast random order output.
 */
class MMappedResultWriter : public BaseResultWriter {
public:
	MMappedResultWriter(const std::string& filename, size_t hash_count, size_t hash_length);
	~MMappedResultWriter() override;
protected:
	void doWriteResult(const hash_result_t& hash, size_t index) const override;

private:
	boost::iostreams::mapped_file_sink file_;
	size_t line_length_;
};


#endif //VEEAM_M_MAPPED_RESULT_WRITER_H
