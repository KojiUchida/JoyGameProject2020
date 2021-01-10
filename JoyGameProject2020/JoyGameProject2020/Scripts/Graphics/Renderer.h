#pragma once
#include <vector>
#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>
#include <memory>

using namespace Microsoft::WRL;

class DirectXManager;
class GraphicsManager;
class ModelData;
class Renderer {
private:
	Renderer();
	~Renderer();
	Renderer(const Renderer&) = delete;
	void operator=(const Renderer&) = delete;

public:
	static Renderer& Instance();
	void Init();
	void Update();
	void Draw();
	void Shutdown();

	void AddModel(std::shared_ptr<ModelData> model);

	void CreateRootSignature();
	void CreatePipeline();

private:
	DirectXManager& m_dxManager;
	GraphicsManager& m_graphicsManager;
	ComPtr<ID3D12RootSignature> m_rootSignature;
	ComPtr<ID3D12PipelineState> m_pipelineState;
	std::vector<std::shared_ptr<ModelData>> models;
};

