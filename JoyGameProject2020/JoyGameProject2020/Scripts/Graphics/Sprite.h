#pragma once

#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>
#include <vector>
#include <d3dcompiler.h>
#include <memory>
#include "GameObject/Component.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Matrix4.h"
#include "Utility/Color4.h"

using namespace Microsoft::WRL;

class DirectXManager;
class GraphicsManager;
class SpriteRenderer;
class Texture;
class Sprite : public Component {
	friend SpriteRenderer;

	struct Vertex {
		Vector3 pos;
		Vector2 uv;
	};

	struct ConstBufferData {
		Color4 color;
		Matrix4 mat;
	};

public:
	Sprite(const std::string& name = "");
	~Sprite();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Shutdown() override;
	void Draw();


	//色設定
	void SetColor(const Color4& color);
	//色取得
	Color4 GetColor();
	//中心位置設定
	void SetPivot(const Vector2& pivot);
	//表示するか
	void SetVisible(bool isVisible);
	//左右反転
	void SetFlipX(bool isFlip);
	//上下反転
	void SetFlipY(bool isFlip);
	void SetTexture(const std::string& name);

private:
	void InitRootSignature();
	void InitPipeline();
	void InitVertexBuffer();
	void InitIndexBuffer();
	void CreateConstantBuffer();
	void UpdateConstantBuffer();

private:
	DirectXManager& m_dxManager;
	GraphicsManager& m_graphicsManager;

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


	//定数バッファ
	ComPtr<ID3D12Resource> m_constBuffer;
	ComPtr<ID3D12DescriptorHeap> m_basicDescHeap;
	ConstBufferData* m_constMap;
	//テクスチャ
	std::string m_textureName;
	std::shared_ptr<Texture> m_texture;

	Color4 m_color;
	Vector2 m_pivot;
	bool m_isVisible;
	bool m_flipX;
	bool m_flipY;
};

