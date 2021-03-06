#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include <vector>
#include <map>
#include <d3dcompiler.h>
#include <string>
#include "Model.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;

class DirectXManager;
class Texture;
class GraphicsManager {
private:
	GraphicsManager();
	~GraphicsManager();
	GraphicsManager(const GraphicsManager&) = delete;
	void operator=(const GraphicsManager&) = delete;

public:
	static GraphicsManager& Instance();

	void Init();

	void LoadTexture(const std::string& filePath, const std::string& textureName);
	std::shared_ptr<Texture> GetTexture(const std::string& textureName);

	void LoadShader(const std::string& filePath, const std::string& entryPoint, const std::string& target, const std::string& shaderName);
	ComPtr<ID3DBlob> GetShader(const std::string& shaderName);
	void CheckShaderCompileResult(HRESULT result, ID3DBlob* error);

	void LoadModel(const std::string& filePath, const std::string& modelName, bool smooth = false);
	std::shared_ptr<ModelData> GetModel(const std::string& modelName);

public:
	std::vector<ModelData::Vertex> vertices;
	std::vector<unsigned short> indices;

private:
	std::map<std::string, std::shared_ptr<Texture>> m_textureMap;
	std::map<std::string, std::shared_ptr<ModelData>> m_modelMap;
	std::map<std::string, ComPtr<ID3DBlob>> m_shaderMap;
};

