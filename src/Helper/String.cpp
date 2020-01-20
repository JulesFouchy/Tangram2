#include "String.hpp"

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