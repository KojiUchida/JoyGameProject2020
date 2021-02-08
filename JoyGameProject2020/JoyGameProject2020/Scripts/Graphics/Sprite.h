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
class Texture;
class GameObject;
class SpriteData {
	struct Vertex {
		Vector3 pos;
		Vector2 uv;
	};

	struct ConstBufferData {
		Color4 color;
		Matrix4 mat;
	};

	struct SpriteOption {
		Color4 color = Color4(1, 1, 1, 1);
		Vector2 pivot = Vector2::Zero();
		bool isVisible = true;
		bool flipX = false;
		bool flipY = false;
	};

public:
	SpriteData(const std::string& name);
	~SpriteData();

	void Draw();
	void SetTexture(const std::string& name);

	void CreateConstantBuffer();
	void UpdateConstantBuffer(GameObject* gameObject);

	SpriteOption option;

private:
	DirectXManager& m_dxManager;
	GraphicsManager& m_graphicsManager;

	//定数バッファ
	ComPtr<ID3D12Resource> m_constBuffer;
	ComPtr<ID3D12DescriptorHeap> m_basicDescHeap;
	ConstBufferData* m_constMap;

	//テクスチャ
	std::string m_textureName;
	std::shared_ptr<Texture> m_texture;
};

class SpriteRenderer;
class Sprite : public Component {

public:
	Sprite(const std::string& name = "");
	~Sprite();

	virtual void Update() override;


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
	SpriteRenderer& m_spriteRenderer;
	std::shared_ptr<SpriteData> m_spriteData;
};

