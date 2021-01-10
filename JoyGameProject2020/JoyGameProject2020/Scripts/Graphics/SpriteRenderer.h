#pragma once

#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>
#include <vector>
#include <d3dcompiler.h>
#include <memory>
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "GameObject/Component.h"

using namespace Microsoft::WRL;

class DirectXManager;
class GraphicsManager;
class Sprite;
class SpriteRenderer : public Component {
	struct Vertex {
		Vector3 pos;
		Vector2 uv;
	};

public:
	SpriteRenderer(const std::string& name = "");
	~SpriteRenderer();

	virtual void Init() override;
	virtual void Update() override;
	void Draw();

private:
	bool InitRootSignature();
	bool InitPipeline();
	bool InitVertexBuffer();
	bool InitIndexBuffer();

private:
	DirectXManager& m_dxManager;
	GraphicsManager& m_renderManager;

	//頂点情報
	ComPtr<ID3D12Resource> m_vertexBuffer;
	ComPtr<ID3D12Resource> m_indexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_vbView;
	D3D12_INDEX_BUFFER_VIEW m_ibView;
	std::vector<Vertex> m_vertices;
	std::vector<unsigned  short> m_indices;
	//パイプライン
	ComPtr<ID3D12RootSignature> m_rootSignature;
	ComPtr<ID3D12PipelineState> m_pipelineState;

	std::shared_ptr<Sprite> m_sprite;
};

