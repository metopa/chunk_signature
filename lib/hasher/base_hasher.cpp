#include "hasher/base_hasher.h"

#include <algorithm>
#include <iterator>
#include <map>
#include <hasher/base_hasher.h>


namespace {
	std::map<std::string, BaseHasher::ptr_t>& getBackends() {
		static std::map<std::string, BaseHasher::ptr_t> backends;
		return backends;
	}
}

std::vector<std::string> BaseHasher::getSupportedBackends() {
	std::vector<std::string> names;
	transform(begin(getBackends()), end(getBackends()), back_inserter(names),
			  [](auto x) { return x.first; });
	return names;
}

BaseHasher::ptr_t BaseHasher::getStaticBackendByName(const std::string& name) {
	auto it = getBackends().find(name);
	if (it == end(getBackends())) {
		throw std::runtime_error("Unknown backend");
	}
	return it->second;
}

BaseHasher::ptr_t BaseHasher::createBackendByName(const std::string& name) {
	return getStaticBackendByName(name)->clone();
}

void BaseHasher::registerBackend(const std::string& name, const ptr_t& static_instance) {
	getBackends()[name] = static_instance;
}
