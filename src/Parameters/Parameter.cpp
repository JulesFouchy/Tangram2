#include "Parameter.hpp"


#include <functional>

size_t Parameter::GetHash(const std::string& name, const std::string& type) {
	return std::hash<std::string>{}(name + type);
}