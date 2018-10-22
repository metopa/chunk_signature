#include "base_hasher.h"

#include <algorithm>
#include <functional>
#include <iterator>
#include <map>

#include "crc32_hasher.h"
#include "md5_hasher.h"

std::map<std::string, std::function<BaseHasher::ptr_t()>> BACKENDS = {
		{"crc32", [] { return BaseHasher::ptr_t(new Crc32Hasher()); }},
		{"md5",   [] { return BaseHasher::ptr_t(new Md5Hasher()); }}
};

std::vector<std::string> BaseHasher::getSupportedBackends() {
	std::vector<std::string> names;
	transform(begin(BACKENDS), end(BACKENDS), back_inserter(names),
			  [](auto x) { return x.first; });
	return names;
}

std::shared_ptr<BaseHasher> BaseHasher::getBackendByName(const std::string& name) {
	auto it = BACKENDS.find(name);
	if (it == end(BACKENDS)) {
		throw std::runtime_error("Unknown backend");
	}
	return it->second();
}
