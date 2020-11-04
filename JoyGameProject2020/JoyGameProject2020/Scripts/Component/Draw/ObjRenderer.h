#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>
#include <string>
#include <memory>

#include "Component/Component.h"

using namespace Microsoft::WRL;

class DirectXManager;
class GraphicsManager;
class ObjModel;
class ObjRenderer : public Component {

public:
	ObjRenderer(const std::string& modelName);
	~ObjRenderer();

	virtual void Update() override;
	void Draw();

	void CreateRootSignature();
	void CreatePipeline();

private:
	DirectXManager& m_dxManager;
	GraphicsManager& m_graphicsManager;

	/* パイプライン */
	ComPtr<ID3D12RootSignature> m_rootSignature;
	ComPtr<ID3D12PipelineState> m_pipelineState;

	std::shared_ptr<ObjModel> m_model;

};

