#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>
#include <map>
#include <vector>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <string>
#include <memory>

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Matrix4.h"
#include "GameObject/Component.h"

using namespace Microsoft::WRL;

class DirectXManager;
class GraphicsManager;
class GameObject;
class Texture;
class ModelData {
public:
	struct Vertex {
		Vector3 position;
		Vector3 normal;
		Vector2 uv;
	};

	struct Subset {
		unsigned int materialIndex;
		unsigned int faceStart;
		unsigned int faceCount;
	};

	struct Transform {
		Matrix4 world;
		Matrix4 rot;
		Matrix4 view;
		Matrix4 proj;
		Matrix4 mvp;
		Vector3 cameraPos;
		float alignment;
		Vector3 cameraDir;
		float alignment2;
		Vector3 lightDir;
		float alignment3;
		Vector3 lightColor;
	};

	struct SurfaceMaterial {
		Vector3 ambientColor = Vector3(1, 1, 1);
		float alpha = 1.0f;
		Vector3 diffuseColor = Vector3(1, 1, 1);
		float shininess = 1.0f;
		Vector3 specularColor = Vector3(1, 1, 1);
	};

	struct AdditionalMaterial {
		std::string materialName;
		std::string ambientMapName;
		std::string diffuseMapName;
		std::string specularMapName;
		std::string bumpMapName;
	};

	struct Material {
		SurfaceMaterial material;
		AdditionalMaterial additional;
	};

public:
	ModelData(const std::string& filePath, bool smooth = false);
	ModelData(const ModelData& other);
	~ModelData();

	void UpdateConstantBuffer(GameObject* gameObject);
	void LoadObj(const std::string& filePath, bool smooth);
	void LoadMtl(const std::string& filePath);
	void CreateVertexBuffer();
	void CreateIndexBuffer();
	void CreateTransformBuffer();
	void CreateMaterialBuffer();

	DirectXManager& m_dxManager;
	GraphicsManager& m_graphicsManager;

	/* 頂点 */
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	ComPtr<ID3D12Resource> m_vertexBuffer;
	ComPtr<ID3D12Resource> m_indexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_vbView;
	D3D12_INDEX_BUFFER_VIEW m_ibView;
	unsigned int vertexCount;
	unsigned int indexCount;

	std::vector<Subset> m_subsets;

	/* 定数バッファ */
	ComPtr<ID3D12Resource> m_transformBuffer;
	ComPtr<ID3D12DescriptorHeap> m_transformDescHeap;

	std::string m_directory;
	std::string m_matName;
	std::vector<Material> m_materials;
	ComPtr<ID3D12Resource> m_materialBuffer;
	ComPtr<ID3D12DescriptorHeap> m_materialDescHeap;

	std::map<std::string, Texture> m_textureMap;
	Transform* transformMap = nullptr;
};

class Renderer;
class Model : public Component {
public:
	Model(const std::string& modelname);
	~Model();

	virtual void Update() override;
	void SetTexture(int matNum, const std::string& texName);

	std::shared_ptr<ModelData> modeldata;
	Renderer& renderer;
};