#include "String.hpp"

#include <algorithm>

#include <glm/glm.hpp>
#include "Debugging/Log.hpp"

size_t MyString::BeginningOfNextWord(const std::string& myString, size_t beginPos, const std::string& charsToIgnore) {
	return myString.find_first_not_of(charsToIgnore, beginPos);
}

size_t MyString::EndOfNextWord(const std::string& myString, size_t beginPos, const std::string& charsToIgnore) {
	size_t pos = myString.find_first_of(charsToIgnore, beginPos);
	return pos == std::string::npos ? myString.size() : pos;
}

std::string MyString::GetNextWord(const std::string& myString, size_t* currentPosition, const std::string& charsToIgnore) {
	size_t beginWord = BeginningOfNextWord(myString, *currentPosition, charsToIgnore);
	size_t endWord = EndOfNextWord(myString, beginWord, charsToIgnore);
	*currentPosition = endWord;
	if (beginWord != std::string::npos) {
		return myString.substr(beginWord, endWord - beginWord);
	}
	else
		return "";
}

void MyString::ReplaceAll(std::string& str, const std::string& from, const std::string& to) {
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

std::string MyString::GetFileExtension(const std::string& myString) {
	return myString.substr(myString.find_last_of(".")+1);
}

std::string MyString::RemoveFolderHierarchy(const std::string& myString) {
	auto pos = myString.find_last_of("/\\") + 1;
	return myString.substr(pos, myString.size() - pos);
}

std::string MyString::GetFolderHierarchy(const std::string& myString) {
	auto pos = myString.find_last_of("/\\") + 1;
	return myString.substr(0, pos-1);
}

std::string MyString::RemoveFileExtension(const std::string& myString) {
	auto pos = myString.find_last_of(".");
	return myString.substr(0, pos);
}

size_t MyString::FindCaseInsensitive(std::string myString, std::string toSearch, size_t pos) {
	std::transform(myString.begin(), myString.end(), myString.begin(), ::tolower);
	std::transform(toSearch.begin(), toSearch.end(), toSearch.begin(), ::tolower);
	return myString.find(toSearch, pos);
}



template <>
float MyString::ReadNumbersAt<float>(const std::string& str, size_t pos) {
	try {
		return std::stof(MyString::GetNextWord(str, &pos));
	}
	catch (...) {
		spdlog::error("[MyString::ReadNumbersAt<float>] Unable to read 1 number at \"{}\"", str);
		return 0.0f;
	}
}
template <>
glm::vec2 MyString::ReadNumbersAt<glm::vec2>(const std::string& str, size_t pos) {
	try {
		float x = std::stof(MyString::GetNextWord(str, &pos));
		float y = std::stof(MyString::GetNextWord(str, &pos));
		return glm::vec2(x, y);
	}
	catch (...) {
		spdlog::error("[MyString::ReadNumbersAt<glm::vec2>] Unable to read 2 numbers at \"{}\"", str);
		return glm::vec2(0.0f);
	}
}
template <>
glm::vec3 MyString::ReadNumbersAt<glm::vec3>(const std::string& str, size_t pos) {
	try {
		float x = std::stof(MyString::GetNextWord(str, &pos));
		float y = std::stof(MyString::GetNextWord(str, &pos));
		float z = std::stof(MyString::GetNextWord(str, &pos));
		return glm::vec3(x, y, z);
	}
	catch (...) {
		spdlog::error("[ShaderSystem::ConvertStringTo<glm::vec3>] Unable to read 3 numbers at \"{}\"", str);
		return glm::vec3(0.0f);
	}
}
template <>
glm::vec4 MyString::ReadNumbersAt<glm::vec4>(const std::string& str, size_t pos) {
	try {
		float x = std::stof(MyString::GetNextWord(str, &pos));
		float y = std::stof(MyString::GetNextWord(str, &pos));
		float z = std::stof(MyString::GetNextWord(str, &pos));
		float w = std::stof(MyString::GetNextWord(str, &pos));
		return glm::vec4(x, y, z, w);
	}
	catch (...) {
		spdlog::error("[MyString::ReadNumbersAt<glm::vec4>] Unable to read 4 numbers at \"{}\"", str);
		return glm::vec4(0.0f);
	}
}
template <>
int MyString::ReadNumbersAt<int>(const std::string& str, size_t pos) {
	try {
		return std::stoi(MyString::GetNextWord(str, &pos));
	}
	catch (...) {
		spdlog::error("[MyString::ReadNumbersAt<int>] Unable to read 1 number at \"{}\"", str);
		return 0;
	}
}
template <>
glm::ivec2 MyString::ReadNumbersAt<glm::ivec2>(const std::string& str, size_t pos) {
	try {
		int x = std::stoi(MyString::GetNextWord(str, &pos));
		int y = std::stoi(MyString::GetNextWord(str, &pos));
		return glm::ivec2(x, y);
	}
	catch (...) {
		spdlog::error("[MyString::ReadNumbersAt<glm::ivec2>] Unable to read 2 numbers at \"{}\"", str);
		return glm::ivec2(0);
	}
}
template <>
glm::ivec3 MyString::ReadNumbersAt<glm::ivec3>(const std::string& str, size_t pos) {
	try {
		int x = std::stoi(MyString::GetNextWord(str, &pos));
		int y = std::stoi(MyString::GetNextWord(str, &pos));
		int z = std::stoi(MyString::GetNextWord(str, &pos));
		return glm::ivec3(x, y, z);
	}
	catch (...) {
		spdlog::error("[MyString::ReadNumbersAt<glm::ivec3>] Unable to read 3 numbers at \"{}\"", str);
		return glm::ivec3(0);
	}
}
template <>
glm::ivec4 MyString::ReadNumbersAt<glm::ivec4>(const std::string& str, size_t pos) {
	try {
		int x = std::stoi(MyString::GetNextWord(str, &pos));
		int y = std::stoi(MyString::GetNextWord(str, &pos));
		int z = std::stoi(MyString::GetNextWord(str, &pos));
		int w = std::stoi(MyString::GetNextWord(str, &pos));
		return glm::ivec4(x, y, z, w);
	}
	catch (...) {
		spdlog::error("[MyString::ReadNumbersAt<glm::ivec4>] Unable to read 4 numbers at \"{}\"", str);
		return glm::ivec4(0);
	}
}