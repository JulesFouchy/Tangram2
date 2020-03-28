#include "ShaderSystem.hpp"

#include "Helper/String.hpp"

#include "Components/GUI/SliderFloat.hpp"
#include "Components/GUI/SliderFloat2.hpp"
#include "Components/GUI/ColorPicker3.hpp"

entt::entity ShaderSystem::Create(entt::registry& R, const std::string& vertexFilepath, const std::string& fragmentFilepath) {
	entt::entity e = R.create();
	R.assign<Cmp::Shader>(e, vertexFilepath, fragmentFilepath);
	return e;
}

void ShaderSystem::FillParametersList(entt::registry& R, entt::entity shaderEntity, std::vector<entt::entity>& parametersList) {
	// Open fragment shader file
	const std::string& filepath = GetShaderCmp(R, shaderEntity).fragmentFilepath;
	std::ifstream stream(filepath);
	if (!stream.is_open()) {
		spdlog::warn("Failed to open file |{}|", filepath);
	}
	// Find "struct Parameters"
	GoToFirstLineOfStructParameters(stream);
	// Loop through struct Parameters' lines
	std::string line;
	while (getline(stream, line)) {
		// stop when reaching "}"
		if (line.find("}") != std::string::npos)
			break;
		// Create parameter
		entt::entity param = CreateParameterFromLine(R, line);
		if (R.valid(param))
			parametersList.push_back(param);
	}
}

void ShaderSystem::GoToFirstLineOfStructParameters(std::ifstream& stream) {
	std::string line;
	while (getline(stream, line)) {
		// look for struct
		size_t pos = line.find("struct");
		if (pos != std::string::npos) {
			// skip "struct"
			MyString::GetNextWord(line, &pos);
			// check for "Parameters"
			if (!MyString::GetNextWord(line, &pos).compare("Parameters")) {
				break;
			}
		}
	}
}

entt::entity ShaderSystem::CreateParameterFromLine(entt::registry& R, const std::string& line) {
	entt::entity e = R.create();
	size_t pos = 0;
	std::string type = MyString::GetNextWord(line, &pos);
	std::string name = MyString::GetNextWord(line, &pos);
	if (!type.compare("float"))
		R.assign<Cmp::SliderFloat>(e, name, ReadValue<float>(line, "default"), ReadValue<float>(line, "min"), ReadValue<float>(line, "max"));
	else if (!type.compare("vec2"))
		R.assign<Cmp::SliderFloat2>(e, name, ReadValue<glm::vec2>(line, "default"), ReadValue<float>(line, "min"), ReadValue<float>(line, "max"));
	else if (!type.compare("vec3"))
		R.assign<Cmp::ColorPicker3>(e, name, glm::vec3(0.0f));
	else {
		spdlog::error("[ShaderSystem::CreateParameterFromLine] Couldn't parse parameter from line : \"{}\"", line);
		return entt::null;
	}
	return e;
}

template <>
float ShaderSystem::ConvertStringTo<float>(const std::string& str, size_t pos) {
	return std::stof(MyString::GetNextWord(str, &pos));
}
template <>
glm::vec2 ShaderSystem::ConvertStringTo<glm::vec2>(const std::string& str, size_t pos) {
	float x = std::stof(MyString::GetNextWord(str, &pos));
	float y = std::stof(MyString::GetNextWord(str, &pos));
	return glm::vec2(x, y);
}
template <>
glm::vec3 ShaderSystem::ConvertStringTo<glm::vec3>(const std::string& str, size_t pos) {
	float x = std::stof(MyString::GetNextWord(str, &pos));
	float y = std::stof(MyString::GetNextWord(str, &pos));
	float z = std::stof(MyString::GetNextWord(str, &pos));
	return glm::vec3(x, y, z);
}
template <>
glm::vec4 ShaderSystem::ConvertStringTo<glm::vec4>(const std::string& str, size_t pos) {
	float x = std::stof(MyString::GetNextWord(str, &pos));
	float y = std::stof(MyString::GetNextWord(str, &pos));
	float z = std::stof(MyString::GetNextWord(str, &pos));
	float w = std::stof(MyString::GetNextWord(str, &pos));
	return glm::vec4(x, y, z, w);
}
template <>
int ShaderSystem::ConvertStringTo<int>(const std::string& str, size_t pos) {
	return std::stoi(MyString::GetNextWord(str, &pos));
}