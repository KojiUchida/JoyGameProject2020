#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>
#include <vector>
#include <string>
#include <forward_list>

#include "GameObject/Component.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Matrix4.h"
#include "Utility/Color4.h"

using namespace Microsoft::WRL;

class DirectXManager;
class GraphicsManager;
class Camera;
class Texture;

class ParticleManager {

private:

	struct Particle {
		Vector3  position;
		Vector3  velocity;
		Vector3  accel;
		int currentFrame = 0;
		int maxFrame = 0;

		float currentScale = 1.0f;
		float startScale = 1.0f;
		float endScale = 0.0f;

		Color4 currentColor;
		Color4 startColor;
		Color4 endColor;
	};

	struct Vertex {
		Vector3 position;
		float scale = 1.0f;
		Color4 color;
	};

	struct Transform {
		Matrix4 mat;
		Matrix4 billboard;
	};

public:
	ParticleManager();
	~ParticleManager();

	void Init();
	void Update();
	void Draw();

	void CreateVertexBuffer();
	void CreateTransformBuffer();
	void CreateRootSignature();
	void CreatePipeline();

	void Add(int life, const Vector3& position, const Vector3& velocity, const Vector3& accel, float startScale, float endScale, const Color4& startColor, const Color4& endColor);

private:
	DirectXManager& m_dxManager;
	GraphicsManager& m_graphicsManager;
	Camera& m_camera;

	/* 頂点 */
	std::vector<Vertex> m_vertices;
	ComPtr<ID3D12Resource> m_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_vbView;

	/* 定数バッファ */
	ComPtr<ID3D12Resource> m_transformBuffer;
	ComPtr<ID3D12DescriptorHeap> m_transformDescHeap;
	ComPtr<ID3D12DescriptorHeap> m_textureDescHeap;

	/* パイプライン */
	ComPtr<ID3D12RootSignature> m_rootSignature;
	ComPtr<ID3D12PipelineState> m_pipelineState;

	std::forward_list<Particle> m_particles;

	const int MaxParticle = 1024;
};

