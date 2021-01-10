#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12.h>
#include <wrl.h>

#include "Math/Matrix4.h"
#include "Math/Vector3.h"
#include "Math/Vector2.h"
#include "Graphics/GraphicsManager.h"

using namespace Microsoft::WRL;

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class Moe_Sprite
{
public:
	Moe_Sprite(const Vector3& pos, const Vector2& size, const std::string& texturename);
	~Moe_Sprite();

	void initialize();
	void update();
	void draw();

	void MatrixClear();
	void SetBuffer();
	void CreatePipeline();

	void AddTexture();


protected:
	ComPtr<ID3D12Device> dev;
	ComPtr<ID3D12GraphicsCommandList> cmdList;

	struct SpriteVertex
	{
		Vector3 pos;
	};
	std::vector<SpriteVertex> vertices;
	//頂点関係
	ComPtr<ID3D12Resource> vertBuff;
	D3D12_VERTEX_BUFFER_VIEW vbView;

	//定数バッファ関係
	ComPtr<ID3D12Resource> constBuff;
	struct ConstBufferData {
		Matrix4 mat;
		Matrix4 billboard;
	};

	ComPtr<ID3D12PipelineState> pipelinestate;
	ComPtr<ID3D12RootSignature> rootsignature;

	std::string texturename;

	Matrix4 matProjection;

	Matrix4 matWorld;
	Matrix4 matScale;
	Matrix4 matRot;
	Matrix4 matTrans;

	int width;
	int height;

	Vector3 position;
	Vector2 size;
};