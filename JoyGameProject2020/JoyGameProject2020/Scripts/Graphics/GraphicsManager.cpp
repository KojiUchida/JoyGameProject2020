#include "GraphicsManager.h"
#include "Texture.h"
#include "Base/DirectXManager.h"
#include "Def/Screen.h"
#include "Utility/FileUtility.h"
#include "Graphics/Model.h"

GraphicsManager::GraphicsManager() {
}

GraphicsManager::~GraphicsManager() {
}

GraphicsManager& GraphicsManager::Instance() {
	static GraphicsManager instance;
	return instance;
}

void GraphicsManager::Init() {
	/* 高速化のために入力無しならデフォルトのテクスチャにする */
	m_textureMap.emplace("", std::make_shared<Texture>());
}

void GraphicsManager::LoadTexture(const std::string& filePath, const std::string& textureName) {
	m_textureMap.emplace(textureName, std::make_shared<Texture>(filePath));
}

std::shared_ptr<Texture> GraphicsManager::GetTexture(const std::string& textureName) {
	_ASSERT_EXPR(m_textureMap.count(textureName) != 0, L"テクスチャ名が間違っているか、読み込まれていません");
	return m_textureMap[textureName];
}

void GraphicsManager::LoadShader(const std::string& filePath, const std::string& entryPoint, const std::string& target, const std::string& shaderName) {
	ComPtr<ID3DBlob> shaderBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;

	auto result = D3DCompileFromFile(
		FileUtility::GetWideStringFromString(filePath).c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint.c_str(),
		target.c_str(),
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&shaderBlob, &errorBlob
	);
	CheckShaderCompileResult(result, errorBlob.Get());

	m_shaderMap.emplace(shaderName, shaderBlob);
}

ComPtr<ID3DBlob> GraphicsManager::GetShader(const std::string& shaderName) {
	_ASSERT_EXPR(m_shaderMap.count(shaderName) != 0, L"シェーダー名が間違っているか、読み込まれていません");
	return m_shaderMap[shaderName];
}

void GraphicsManager::CheckShaderCompileResult(HRESULT result, ID3DBlob* error) {
	if (FAILED(result)) {
		if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)) {
			_ASSERT_EXPR(false, L"ファイルが見当たりません");
		} else {
			std::wstring errstr;
			errstr.resize(error->GetBufferSize());
			std::copy_n((char*)error->GetBufferPointer(), error->GetBufferSize(), errstr.begin());
			errstr += L"\n";
			_ASSERT_EXPR(false, errstr.c_str());
		}
	}
}

void GraphicsManager::LoadModel(const std::string& filePath, const std::string& modelName, bool smooth) {
	auto model =  std::make_shared<ModelData>(filePath, smooth);
	m_modelMap.emplace(modelName, model);

	model->vertexoffset = vertices.size();
	for (int i = 0; i < model->vertexCount; ++i) {
		vertices.push_back(model->m_vertices[i]);
	}
	model->indexoffset = indices.size();
	for (int i = 0; i < model->indexCount; ++i) {
		indices.push_back(model->m_indices[i]);
	}
}

std::shared_ptr<ModelData> GraphicsManager::GetModel(const std::string& modelName) {
	_ASSERT_EXPR(m_modelMap.count(modelName) != 0, L"モデル名が間違っているか、読み込まれていません");
	return std::make_shared<ModelData>(*m_modelMap[modelName]);
}

