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

using namespace Microsoft::WRL;

class DirectXManager;
class GraphicsManager;
class GameObject;
class Texture;
class ObjModel {
private:
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
		Vector3 lightDir;
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
	ObjModel(const ObjModel& other);
	ObjModel(const std::string& filePath);
	~ObjModel();

	void Draw(GameObject* gameObject);

private:
	void UpdateConstantBuffer(GameObject* gameObject);
	void LoadObj(const std::string& filePath);
	void LoadMtl(const std::string& filePath);
	void CreateVertexBuffer();
	void CreateIndexBuffer();
	void CreateTransformBuffer();
	void CreateMaterialBuffer();

private:
	DirectXManager& m_dxManager;
	GraphicsManager& m_graphicsManager;

	/* 頂点 */
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	ComPtr<ID3D12Resource> m_vertexBuffer;
	ComPtr<ID3D12Resource> m_indexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_vbView;
	D3D12_INDEX_BUFFER_VIEW m_ibView;

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
};

