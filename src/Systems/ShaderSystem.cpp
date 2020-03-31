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
	// Read values according to type
	std::shared_ptr<Parameter> paramPtr;
	if (!type.compare("float"))
		paramPtr = std::make_shared<FloatParameter>(glUniformLocation, name, ReadValue<float>(line, "default"), ReadValue<float>(line, "min"), ReadValue<float>(line, "max"));
	else if (!type.compare("vec2")) {
		if (MyString::FindCaseInsensitive(line, "NOT_A_POINT2D") != std::string::npos)
			paramPtr = std::make_shared<Float2Parameter>(glUniformLocation, name, ReadValue<glm::vec2>(line, "default"), ReadValue<float>(line, "min"), ReadValue<float>(line, "max"));
		else
			paramPtr = std::make_shared<Point2DParameter>(R, parentLayer, glUniformLocation, name, ReadValue<glm::vec2>(line, "default"));
	}
	else if (!type.compare("vec3")) {
		if (MyString::FindCaseInsensitive(line, "NOT_A_COLOR") != std::string::npos)
			paramPtr = std::make_shared<Float3Parameter>(glUniformLocation, name, ReadValue<glm::vec3>(line, "default"), ReadValue<float>(line, "min"), ReadValue<float>(line, "max"));
		else
			paramPtr = std::make_shared<Color3Parameter>(glUniformLocation, name, ReadValue<glm::vec3>(line, "default"));
	}
	else if (!type.compare("vec4")) {
		if (MyString::FindCaseInsensitive(line, "NOT_A_COLOR") != std::string::npos)
			paramPtr = std::make_shared<Float4Parameter>(glUniformLocation, name, ReadValue<glm::vec4>(line, "default"), ReadValue<float>(line, "min"), ReadValue<float>(line, "max"));
		else
			paramPtr = std::make_shared<Color4Parameter>(glUniformLocation, name, ReadValue<glm::vec4>(line, "default"));
	}
	else if (!type.compare("bool"))
		paramPtr = std::make_shared<BoolParameter>(glUniformLocation, name, MyString::FindCaseInsensitive(line, "true") != std::string::npos);
	else if (!type.compare("int"))
		paramPtr = std::make_shared<IntParameter>(glUniformLocation, name, ReadValue<int>(line, "default"), ReadValue<int>(line, "min"), ReadValue<int>(line, "max"));
	else if (!type.compare("ivec2"))
		paramPtr = std::make_shared<Int2Parameter>(glUniformLocation, name, ReadValue<glm::ivec2>(line, "default"), ReadValue<int>(line, "min"), ReadValue<int>(line, "max"));
	else if (!type.compare("ivec3"))
		paramPtr = std::make_shared<Int3Parameter>(glUniformLocation, name, ReadValue<glm::ivec3>(line, "default"), ReadValue<int>(line, "min"), ReadValue<int>(line, "max"));
	else if (!type.compare("ivec4"))
		paramPtr = std::make_shared<Int4Parameter>(glUniformLocation, name, ReadValue<glm::ivec4>(line, "default"), ReadValue<int>(line, "min"), ReadValue<int>(line, "max"));
	else {
		spdlog::error("[ShaderSystem::CreateParameterFromLine] Unknown type : \"{}\"", type);
		paramPtr = nullptr;
	}
	// Check if the parameter already existed and grab the value in this case
	size_t hashOfParam = Parameter::GetHash(name, type);
	auto it = std::find_if(prevList.begin(), prevList.end(), [hashOfParam](const std::shared_ptr<Parameter>& p) {
		return p->getHash() == hashOfParam;
		});
	if (it != prevList.end()) {
		(**it).copyValueTo(paramPtr.get());
	}
	return std::move(paramPtr);
}

int ShaderSystem::GetUniformLocation(int glShaderID, const std::string& parameterName) {
	return glGetUniformLocation(glShaderID, ("u." + parameterName).c_str());
}