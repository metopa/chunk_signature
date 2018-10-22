#include "hasher/base_hasher.h"

#include <algorithm>
#include <functional>
#include <iterator>
#include <map>
#include <iostream>

#include "crc32_hasher.h"
#include "md5_hasher.h"

namespace {
	std::map<std::string, std::function<BaseHasher::ptr_t()>>& getBackends() {
		static std::map<std::string, std::function<BaseHasher::ptr_t()>> backends;
		return backends;
	}
}

std::vector<std::string> BaseHasher::getSupportedBackends() {
	std::vector<std::string> names;
	transform(begin(getBackends()), end(getBackends()), back_inserter(names),
			  [](auto x) { return x.first; });
	return names;
}

std::shared_ptr<BaseHasher> BaseHasher::getBackendByName(const std::string& name) {
	auto it = getBackends().find(name);
	if (it == end(getBackends())) {
		throw std::runtime_error("Unknown backend");
	}
	return it->second();
}

void BaseHasher::registerBackend(const std::string& name, const std::function<ptr_t()>& creator) {
	getBackends()[name] = creator;
}
