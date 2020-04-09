#pragma once

namespace MyString {
	size_t BeginningOfNextWord(const std::string& myString, size_t beginPos, const std::string& charsToIgnore = " ,;/()|\t");
	size_t EndOfNextWord(const std::string& myString, size_t beginPos, const std::string& charsToIgnore = " ,;/()|\t");
	std::string GetNextWord(const std::string& myString, size_t* currentPosition, const std::string& charsToIgnore = " ,;/()|\t");
	void ReplaceAll(std::string& str, const std::string& from, const std::string& to);
	std::string GetFileExtension(const std::string& myString);
	std::string RemoveFolderHierarchy(const std::string& myString);
	std::string GetFolderHierarchy(const std::string& myString);
	std::string RemoveFileExtension(const std::string& myString);
	size_t FindCaseInsensitive(std::string myString, std::string toSearch, size_t pos = 0);

	template <typename T>
	T ReadNumbersAt(const std::string& str, size_t pos = 0);
}