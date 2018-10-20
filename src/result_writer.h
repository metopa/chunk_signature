#ifndef VEEAM_RESULT_WRITER_H
#define VEEAM_RESULT_WRITER_H


#include "common.h"

class BaseResultWriter {
public:
	BaseResultWriter(size_t hash_count, size_t hash_length) :
		hash_count_(hash_count), hash_length_(hash_length) {}
	virtual ~BaseResultWriter() = default;

	void writeResult(const hash_result_t& hash, size_t index);

	virtual void finalize() {}
	
protected:
	virtual void doWriteResult(const hash_result_t& hash, size_t index) const = 0;

	size_t hash_count_;
	size_t hash_length_;
};


#endif //VEEAM_RESULT_WRITER_H
