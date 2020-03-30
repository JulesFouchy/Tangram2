#include "ShaderSystem.hpp"

#include "Helper/String.hpp"

#include "Components/ParametersList.hpp"
#include "Components/ShaderReference.hpp"

#include "Parameters/ConcreteParameters.hpp"

#include "Core/MustRecomputeTexture.hpp"

entt::entity ShaderSystem::Create(entt::registry& R, const std::string& vertexFilepath, const std::string& fragmentFilepath) {
	entt::entity e = R.create();
	R.assign<Cmp::Shader>(e, vertexFilepath, fragmentFilepath);
	return e;
}

void ShaderSystem::UpdateParametersList(entt::registry& R, entt::entity parentLayer, entt::entity shaderEntity, std::vector<std::shared_ptr<Parameter>>& parametersList) {
	Cmp::Shader shaderCmp = GetShaderCmp(R, shaderEntity);
	// Open fragment shader file
	const std::string& filepath = shaderCmp.fragmentFilepath;
	std::ifstream stream(filepath);
	if (!stream.is_open()) {
		spdlog::warn("Failed to open file |{}|", filepath);
	}
	// Find "struct Parameters"
	GoToFirstLineOfStructParameters(stream);
	// Clear list
	std::vector<std::shared_ptr<Parameter>> prevList = parametersList;
	parametersList.clear();
	// Loop through struct Parameters' lines
	std::string line;
	while (getline(stream, line)) {
		// stop when reaching "}"
		if (line.find("}") != std::string::npos)
			break;
		// Create parameter
		std::shared_ptr<Parameter> param = CreateParameterFromLine(R, parentLayer, line, shaderCmp.id, prevList);
		if (param) {
			parametersList.push_back(std::move(param));
		}
	}
}

void ShaderSystem::TryReloadShader(entt::registry& R, entt::entity layer) {
	Cmp::ShaderReference* shaderRef = R.try_get<Cmp::ShaderReference>(layer);
	if (shaderRef) {
		Cmp::Parameters& params = R.get<Cmp::Parameters>(layer);
		params.history.clear();
		Cmp::Shader& shader = R.get<Cmp::Shader>(shaderRef->entityID);
		R.replace<Cmp::Shader>(shaderRef->entityID, shader.vertexFilepath, shader.fragmentFilepath);
		UpdateParametersList(R, layer, shaderRef->entityID, params.list);
		TNG::MustRecomputeTexture(R, layer);
	}
}

void ShaderSystem::ComputeUniformLocations(entt::registry& R, entt::entity layerWithAShader) {
	Cmp::Parameters& params = R.get<Cmp::Parameters>(layerWithAShader);
	Cmp::Shader& shader = R.get<Cmp::Shader>(R.get<Cmp::ShaderReference>(layerWithAShader).entityID);
	glUseProgram(shader.id);
	for (const auto& param : params.list) {
		param->m_glUniformLocation = GetUniformLocation(shader.id, param->m_name);
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

std::shared_ptr<Parameter> ShaderSystem::CreateParameterFromLine(entt::registry& R, entt::entity parentLayer, const std::string& line, int glShaderID, const std::vector<std::shared_ptr<Parameter>>& prevList) {
	size_t pos = 0;
	// Read type and name
	std::string type = MyString::GetNextWord(line, &pos);
	std::string name = MyString::GetNextWord(line, &pos);
	// Get uniform location
	int glUniformLocation = GetUniformLocation(glShaderID, name);
	// Check if the parameter already existed and grab the value
	size_t hashOfParam = Parameter::GetHash(name, type);
	auto it = std::find_if(prevList.begin(), prevList.end(), [hashOfParam](const std::shared_ptr<Parameter>& p) {
		return p->getHash() == hashOfParam;
	});
	void* val = nullptr;
	if (it != prevList.end()) {
		val = (**it).getValuePtr();
	}
	//
	if (!type.compare("float"))
		return std::make_shared<FloatParameter>(glUniformLocation, name, val ? *(float*)val : ReadValue<float>(line, "default"), ReadValue<float>(line, "min"), ReadValue<float>(line, "max"));
	else if (!type.compare("vec2")) {
		if (MyString::FindCaseInsensitive(line, "NOT_A_POINT2D") != std::string::npos)
			return std::make_shared<Float2Parameter>(glUniformLocation, name, val ? *(glm::vec2*)val : ReadValue<glm::vec2>(line, "default"), ReadValue<float>(line, "min"), ReadValue<float>(line, "max"));
		else
			return std::make_shared<Point2DParameter>(R, parentLayer, glUniformLocation, name, val ? *(glm::vec2*)val : ReadValue<glm::vec2>(line, "default"));
	}
	else if (!type.compare("vec3")) {
		if (MyString::FindCaseInsensitive(line, "NOT_A_COLOR") != std::string::npos)
			return std::make_shared<Float3Parameter>(glUniformLocation, name, val ? *(glm::vec3*)val : ReadValue<glm::vec3>(line, "default"), ReadValue<float>(line, "min"), ReadValue<float>(line, "max"));
		else
			return std::make_shared<Color3Parameter>(glUniformLocation, name, val ? *(glm::vec3*)val : ReadValue<glm::vec3>(line, "default"));
	}
	else if (!type.compare("vec4")) {
		if (MyString::FindCaseInsensitive(line, "NOT_A_COLOR") != std::string::npos)
			return std::make_shared<Float4Parameter>(glUniformLocation, name, val ? *(glm::vec4*)val : ReadValue<glm::vec4>(line, "default"), ReadValue<float>(line, "min"), ReadValue<float>(line, "max"));
		else
			return std::make_shared<Color4Parameter>(glUniformLocation, name, val ? *(glm::vec4*)val : ReadValue<glm::vec4>(line, "default"));
	}
	else if (!type.compare("bool"))
		return std::make_shared<BoolParameter>(glUniformLocation, name, val ? *(bool*)val : MyString::FindCaseInsensitive(line, "true") != std::string::npos);
	else if (!type.compare("int"))
		return std::make_shared<IntParameter>(glUniformLocation, name, val ? *(int*)val : ReadValue<int>(line, "default"), ReadValue<int>(line, "min"), ReadValue<int>(line, "max"));
	else if (!type.compare("ivec2"))
		return std::make_shared<Int2Parameter>(glUniformLocation, name, val ? *(glm::ivec2*)val : ReadValue<glm::ivec2>(line, "default"), ReadValue<int>(line, "min"), ReadValue<int>(line, "max"));
	else if (!type.compare("ivec3"))
		return std::make_shared<Int3Parameter>(glUniformLocation, name, val ? *(glm::ivec3*)val : ReadValue<glm::ivec3>(line, "default"), ReadValue<int>(line, "min"), ReadValue<int>(line, "max"));
	else if (!type.compare("ivec4"))
		return std::make_shared<Int4Parameter>(glUniformLocation, name, val ? *(glm::ivec4*)val : ReadValue<glm::ivec4>(line, "default"), ReadValue<int>(line, "min"), ReadValue<int>(line, "max"));
	else {
		spdlog::error("[ShaderSystem::CreateParameterFromLine] Unknown type : \"{}\"", type);
		return nullptr;
	}
}

int ShaderSystem::GetUniformLocation(int glShaderID, const std::string& parameterName) {
	return glGetUniformLocation(glShaderID, ("u." + parameterName).c_str());
}

template <>
float ShaderSystem::ConvertStringTo<float>(const std::string& str, size_t pos) {
	try {
		return std::stof(MyString::GetNextWord(str, &pos));
	}
	catch (...) {
		spdlog::error("[ShaderSystem::ConvertStringTo<float>] Unable to read 1 number at \"{}\"", str);
		return 0.0f;
	}
}
template <>
glm::vec2 ShaderSystem::ConvertStringTo<glm::vec2>(const std::string& str, size_t pos) {
	try {
		float x = std::stof(MyString::GetNextWord(str, &pos));
		float y = std::stof(MyString::GetNextWord(str, &pos));
		return glm::vec2(x, y);
	}
	catch (...) {
		spdlog::error("[ShaderSystem::ConvertStringTo<glm::vec2>] Unable to read 2 numbers at \"{}\"", str);
		return glm::vec2(0.0f);
	}
}
template <>
glm::vec3 ShaderSystem::ConvertStringTo<glm::vec3>(const std::string& str, size_t pos) {
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
glm::vec4 ShaderSystem::ConvertStringTo<glm::vec4>(const std::string& str, size_t pos) {
	try {
		float x = std::stof(MyString::GetNextWord(str, &pos));
		float y = std::stof(MyString::GetNextWord(str, &pos));
		float z = std::stof(MyString::GetNextWord(str, &pos));
		float w = std::stof(MyString::GetNextWord(str, &pos));
		return glm::vec4(x, y, z, w);
	}
	catch (...) {
		spdlog::error("[ShaderSystem::ConvertStringTo<glm::vec4>] Unable to read 4 numbers at \"{}\"", str);
		return glm::vec4(0.0f);
	}
}
template <>
int ShaderSystem::ConvertStringTo<int>(const std::string& str, size_t pos) {
	try {
		return std::stoi(MyString::GetNextWord(str, &pos));
	}
	catch (...) {
		spdlog::error("[ShaderSystem::ConvertStringTo<int>] Unable to read 1 number at \"{}\"", str);
		return 0;
	}
}
template <>
glm::ivec2 ShaderSystem::ConvertStringTo<glm::ivec2>(const std::string& str, size_t pos) {
	try {
		int x = std::stoi(MyString::GetNextWord(str, &pos));
		int y = std::stoi(MyString::GetNextWord(str, &pos));
		return glm::ivec2(x, y);
	}
	catch (...) {
		spdlog::error("[ShaderSystem::ConvertStringTo<glm::ivec2>] Unable to read 2 numbers at \"{}\"", str);
		return glm::ivec2(0);
	}
}
template <>
glm::ivec3 ShaderSystem::ConvertStringTo<glm::ivec3>(const std::string& str, size_t pos) {
	try {
		int x = std::stoi(MyString::GetNextWord(str, &pos));
		int y = std::stoi(MyString::GetNextWord(str, &pos));
		int z = std::stoi(MyString::GetNextWord(str, &pos));
		return glm::ivec3(x, y, z);
	}
	catch (...) {
		spdlog::error("[ShaderSystem::ConvertStringTo<glm::ivec3>] Unable to read 3 numbers at \"{}\"", str);
		return glm::ivec3(0);
	}
}
template <>
glm::ivec4 ShaderSystem::ConvertStringTo<glm::ivec4>(const std::string& str, size_t pos) {
	try {
		int x = std::stoi(MyString::GetNextWord(str, &pos));
		int y = std::stoi(MyString::GetNextWord(str, &pos));
		int z = std::stoi(MyString::GetNextWord(str, &pos));
		int w = std::stoi(MyString::GetNextWord(str, &pos));
		return glm::ivec4(x, y, z, w);
	}
	catch (...) {
		spdlog::error("[ShaderSystem::ConvertStringTo<glm::ivec4>] Unable to read 4 numbers at \"{}\"", str);
		return glm::ivec4(0);
	}
}