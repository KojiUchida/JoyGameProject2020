#include "ObjModel.h"
#include <fstream>
#include "Base/DirectXManager.h"
#include "Graphics/GraphicsManager.h"
#include "Utility/FileUtility.h"
#include "Device/Camera.h"
#include "GameObject/GameObject.h"
#include "Graphics/Texture.h"

ObjModel::ObjModel(const ObjModel& other) :
	m_dxManager(other.m_dxManager),
	m_graphicsManager(other.m_graphicsManager),
	m_vbView(other.m_vbView),
	m_ibView(other.m_ibView),
	m_subsets(other.m_subsets),
	m_textureMap(other.m_textureMap),
	m_materials(other.m_materials),
	m_materialDescHeap(other.m_materialDescHeap) {

	CreateTransformBuffer();
}

ObjModel::ObjModel(const std::string& filePath) :
	m_dxManager(DirectXManager::Instance()),
	m_graphicsManager(GraphicsManager::Instance()),
	m_vbView(),
	m_ibView() {

	m_directory = FileUtility::GetDirectory(filePath);

	LoadObj(filePath);
	LoadMtl(m_directory + m_matName);

	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateTransformBuffer();
	CreateMaterialBuffer();
}

ObjModel::~ObjModel() {
}

void ObjModel::Draw(GameObject* gameObject) {
	UpdateConstantBuffer(gameObject);

	m_dxManager.GetCommandList()->IASetVertexBuffers(0, 1, &m_vbView);
	m_dxManager.GetCommandList()->IASetIndexBuffer(&m_ibView);
	m_dxManager.GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ComPtr<ID3D12DescriptorHeap> transformHeaps[] = { m_transformDescHeap.Get() };
	m_dxManager.GetCommandList()->SetDescriptorHeaps(_countof(transformHeaps), transformHeaps->GetAddressOf());
	m_dxManager.GetCommandList()->SetGraphicsRootDescriptorTable(0,
		m_transformDescHeap->GetGPUDescriptorHandleForHeapStart());

	ComPtr<ID3D12DescriptorHeap> materialHeaps[] = { m_materialDescHeap.Get() };
	m_dxManager.GetCommandList()->SetDescriptorHeaps(_countof(materialHeaps), materialHeaps->GetAddressOf());

	auto cbvsrvIncSize = m_dxManager.GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * 5;

	for (auto& s : m_subsets) {
		auto materialHeapHandle = m_materialDescHeap->GetGPUDescriptorHandleForHeapStart();
		materialHeapHandle.ptr += (UINT64)cbvsrvIncSize * s.materialIndex;
		m_dxManager.GetCommandList()->SetGraphicsRootDescriptorTable(1, materialHeapHandle);
		m_dxManager.GetCommandList()->DrawIndexedInstanced(s.faceCount, 1, s.faceStart, 0, 0);
	}
}

void ObjModel::SetTexture(const std::string & texturename)
{
	this->texturename=texturename;
}

void ObjModel::UpdateConstantBuffer(GameObject* gameObject) {
	Transform* transformMap = nullptr;
	m_transformBuffer->Map(0, nullptr, (void**)&transformMap);
	transformMap->world =
		Matrix4::Scale(gameObject->GetScale()) *
		Matrix4::RotateRollPitchYaw(gameObject->GetRotation()) *
		gameObject->GetBillboard() *
		Matrix4::Translate(gameObject->GetPosition());
	transformMap->rot = Matrix4::RotateRollPitchYaw(gameObject->GetRotation());
	transformMap->view = Camera::Instance().GetViewMatrix();
	transformMap->proj = Camera::Instance().GetProjectionMatrix();
	transformMap->mvp = transformMap->world * transformMap->view * transformMap->proj;
	transformMap->lightDir = Vector3(0, -1, 1).Normalize();
	m_transformBuffer->Unmap(0, nullptr);
}

void ObjModel::LoadObj(const std::string& filePath) {
	std::vector<Vector3> tmpVertices;
	std::vector<Vector3> tmpNormals;
	std::vector<Vector2> tmpUVs;
	std::map<std::string, UINT> tmpMatIndexMap;
	int subsetIndex = -1;
	int matIndex = -1;

	std::fstream fin;
	char buffer[128]{};

	fin.open(filePath);
	_ASSERT_EXPR(!fin.fail(), L"ファイルの読み込みに失敗しました");

	while (!fin.eof()) {
		fin >> buffer;
		if (strcmp(buffer, "mtllib") == 0) {
			fin >> m_matName;
		}
		if (strcmp(buffer, "v") == 0) {
			float x, y, z;
			fin >> x >> y >> z;
			tmpVertices.push_back(Vector3(x, y, z));
		}
		if (strcmp(buffer, "vt") == 0) {
			float x, y;
			fin >> x >> y;
			tmpUVs.push_back(Vector2(x, y));
		}
		if (strcmp(buffer, "vn") == 0) {
			float x, y, z;
			fin >> x >> y >> z;
			tmpNormals.push_back(Vector3(x, y, z));
		}
		if (strcmp(buffer, "usemtl") == 0) {
			std::string matName;
			fin >> matName;
			if (!tmpMatIndexMap.count(matName)) {
				++matIndex;
				tmpMatIndexMap.emplace(matName, matIndex);
			}

			++subsetIndex;
			m_subsets.push_back(Subset());
			m_subsets[subsetIndex].materialIndex = tmpMatIndexMap[matName];
			m_subsets[subsetIndex].faceStart = (UINT)m_indices.size();
			m_subsets[subsetIndex].faceCount = 0;
		}
		if (strcmp(buffer, "f") == 0) {
			auto count = 0;
			while (true) {
				++count;

				unsigned int positionIndex = 0;
				unsigned int normalIndex = 0;
				unsigned int uvIndex = 0;

				fin >> positionIndex;

				if (fin.peek() == '/') {
					fin.ignore();
					fin >> uvIndex;
				}
				if (fin.peek() == '/') {
					fin.ignore();
					fin >> normalIndex;
				}

				Vertex vertex;
				vertex.position = tmpVertices[positionIndex - 1];
				vertex.normal = tmpNormals[normalIndex - 1];
				vertex.uv = tmpUVs[uvIndex - 1];
				m_vertices.push_back(vertex);

				if (' ' == fin.peek()) {
					fin.get();
				}

				if ('\n' == fin.peek()) {
					break;
				}
			}

			auto size = (UINT)m_vertices.size() - count;

			for (auto i = 0; i < count - 2; ++i) {
				m_indices.push_back(size + 0);
				m_indices.push_back(size + i + 1);
				m_indices.push_back(size + i + 2);
				m_subsets[subsetIndex].faceCount += 3;
			}
		}
	}
}

void ObjModel::LoadMtl(const std::string& filePath) {
	std::fstream fin;
	char buffer[128]{};
	int matCount = -1;

	fin.open(filePath);
	_ASSERT_EXPR(!fin.fail(), L"ファイルの読み込みに失敗しました");

	while (!fin.eof()) {
		fin >> buffer;

		if (strcmp(buffer, "newmtl") == 0) {
			++matCount;
			m_materials.push_back(Material());
			fin >> m_materials[matCount].additional.materialName;
		}
		if (strcmp(buffer, "Ka") == 0) {
			float r, g, b;
			fin >> r >> g >> b;
			m_materials[matCount].material.ambientColor = Vector3(r, g, b);
		}
		if (strcmp(buffer, "Kd") == 0) {
			float r, g, b;
			fin >> r >> g >> b;
			m_materials[matCount].material.diffuseColor = Vector3(r, g, b);
		}
		if (strcmp(buffer, "Ks") == 0) {
			float r, g, b;
			fin >> r >> g >> b;
			m_materials[matCount].material.specularColor = Vector3(r, g, b);
		}
		if (strcmp(buffer, "Ni") == 0) {
			float shininess;
			fin >> shininess;
			m_materials[matCount].material.shininess = shininess;
		}
		if (strcmp(buffer, "d") == 0) {
			float alpha;
			fin >> alpha;
			m_materials[matCount].material.alpha = alpha;
		}
		if (strcmp(buffer, "map_Ka") == 0) {
			std::string name;
			fin >> name;
			m_materials[matCount].additional.ambientMapName = name;
			if (!m_textureMap.count(name)) {
				m_textureMap.emplace(name, Texture(m_directory + name));
			}
		}
		if (strcmp(buffer, "map_Kd") == 0) {
			std::string name;
			fin >> name;
			m_materials[matCount].additional.diffuseMapName = name;
			if (!m_textureMap.count(name)) {
				m_textureMap.emplace(name, Texture(m_directory + name));
			}
		}
		if (strcmp(buffer, "map_Ks") == 0) {
			std::string name;
			fin >> name;
			m_materials[matCount].additional.specularMapName = name;
			if (!m_textureMap.count(name)) {
				m_textureMap.emplace(name, Texture(m_directory + name));
			}
		}
		if (strcmp(buffer, "map_bump") == 0 || strcmp(buffer, "bump") == 0) {
			std::string name;
			fin >> name;
			m_materials[matCount].additional.bumpMapName = name;
			if (!m_textureMap.count(name)) {
				m_textureMap.emplace(name, Texture(m_directory + name));
			}
		}
	}
}

void ObjModel::CreateVertexBuffer() {
	auto heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto resdesc = CD3DX12_RESOURCE_DESC::Buffer(m_vertices.size() * sizeof(m_vertices[0]));
	auto result = DirectXManager::Instance().GetDevice()->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_vertexBuffer)
	);
	_ASSERT_EXPR(SUCCEEDED(result), L"頂点バッファの生成に失敗しました");

	Vertex* vertexMap = nullptr;
	m_vertexBuffer->Map(0, nullptr, (void**)&vertexMap);
	for (int i = 0; i < m_vertices.size(); ++i) {
		vertexMap[i] = m_vertices[i];
	}
	m_vertexBuffer->Unmap(0, nullptr);

	m_vbView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	m_vbView.SizeInBytes = (UINT)m_vertices.size() * sizeof(m_vertices[0]);
	m_vbView.StrideInBytes = sizeof(m_vertices[0]);
}

void ObjModel::CreateIndexBuffer() {
	auto heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto resdesc = CD3DX12_RESOURCE_DESC::Buffer(m_indices.size() * sizeof(m_indices[0]));
	auto result = DirectXManager::Instance().GetDevice()->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_indexBuffer)
	);
	_ASSERT_EXPR(SUCCEEDED(result), L"インデックスバッファの生成に失敗しました");

	unsigned short* indexMap = nullptr;
	m_indexBuffer->Map(0, nullptr, (void**)&indexMap);
	for (int i = 0; i < m_indices.size(); ++i) {
		indexMap[i] = m_indices[i];
	}
	m_indexBuffer->Unmap(0, nullptr);

	m_ibView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
	m_ibView.Format = DXGI_FORMAT_R16_UINT;
	m_ibView.SizeInBytes = (UINT)m_indices.size() * sizeof(m_indices[0]);
}

void ObjModel::CreateTransformBuffer() {
	UINT transformBufferSize = (sizeof(Transform) + 0xff) & ~0xff;

	auto heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto resdesc = CD3DX12_RESOURCE_DESC::Buffer(transformBufferSize * m_materials.size());
	auto result = m_dxManager.GetDevice()->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_transformBuffer.ReleaseAndGetAddressOf())
	);
	_ASSERT_EXPR(SUCCEEDED(result), L"バッファの生成に失敗しました。");

	/* NumDescriptorsの個数分のヒープを確保 */
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.NumDescriptors = 1;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	result = m_dxManager.GetDevice()->CreateDescriptorHeap(&descHeapDesc,
		IID_PPV_ARGS(m_transformDescHeap.ReleaseAndGetAddressOf()));
	_ASSERT_EXPR(SUCCEEDED(result), L"ディスクリプタヒープの生成に失敗しました。");

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = m_transformBuffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = transformBufferSize;

	/* 確保したヒープに実体生成 */
	m_dxManager.GetDevice()->CreateConstantBufferView(&cbvDesc,
		m_transformDescHeap->GetCPUDescriptorHandleForHeapStart());
}

void ObjModel::CreateMaterialBuffer() {

	UINT materialBufferSize = (sizeof(SurfaceMaterial) + 0xff) & ~0xff;

	auto heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto resdesc = CD3DX12_RESOURCE_DESC::Buffer(materialBufferSize * m_materials.size());
	auto result = m_dxManager.GetDevice()->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_materialBuffer.ReleaseAndGetAddressOf())
	);
	_ASSERT_EXPR(SUCCEEDED(result), L"バッファの生成に失敗しました。");

	char* mapMaterial = nullptr;
	result = m_materialBuffer->Map(0, nullptr, (void**)&mapMaterial);
	_ASSERT_EXPR(SUCCEEDED(result), L"バッファのマップに失敗しました。");
	for (auto& m : m_materials) {
		*((SurfaceMaterial*)mapMaterial) = m.material;//データコピー
		mapMaterial += materialBufferSize;//次のアライメント位置まで進める
	}
	m_materialBuffer->Unmap(0, nullptr);

	/* NumDescriptorsの個数分のヒープを確保 */
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.NumDescriptors = (UINT)m_materials.size() * 5; /* 定数バッファ1つ + テクスチャ4つ */
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	result = m_dxManager.GetDevice()->CreateDescriptorHeap(&descHeapDesc,
		IID_PPV_ARGS(m_materialDescHeap.ReleaseAndGetAddressOf()));
	_ASSERT_EXPR(SUCCEEDED(result), L"ディスクリプタヒープの生成に失敗しました。");

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = m_materialBuffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = materialBufferSize;

	/* テクスチャ用 */
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	/* 確保したヒープに実体生成 */
	auto descHandle = m_materialDescHeap->GetCPUDescriptorHandleForHeapStart();
	auto incSize = m_dxManager.GetDevice()->GetDescriptorHandleIncrementSize(
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	for (int i = 0; i < m_materials.size(); ++i) {
		m_dxManager.GetDevice()->CreateConstantBufferView(&cbvDesc, descHandle);
		descHandle.ptr += incSize;
		cbvDesc.BufferLocation += materialBufferSize;

		auto textureBuffer = m_textureMap[m_materials[i].additional.ambientMapName].GetBuffer();
		srvDesc.Format = textureBuffer->GetDesc().Format;
		m_dxManager.GetDevice()->CreateShaderResourceView(
			textureBuffer, &srvDesc, descHandle);
		descHandle.ptr += incSize;

		textureBuffer = m_textureMap[m_materials[i].additional.diffuseMapName].GetBuffer();
		srvDesc.Format = textureBuffer->GetDesc().Format;
		m_dxManager.GetDevice()->CreateShaderResourceView(
			textureBuffer, &srvDesc, descHandle);
		descHandle.ptr += incSize;

		textureBuffer = m_textureMap[m_materials[i].additional.specularMapName].GetBuffer();
		srvDesc.Format = textureBuffer->GetDesc().Format;
		m_dxManager.GetDevice()->CreateShaderResourceView(
			textureBuffer, &srvDesc, descHandle);
		descHandle.ptr += incSize;

		textureBuffer = m_textureMap[m_materials[i].additional.bumpMapName].GetBuffer();
		srvDesc.Format = textureBuffer->GetDesc().Format;
		m_dxManager.GetDevice()->CreateShaderResourceView(
			textureBuffer, &srvDesc, descHandle);
		descHandle.ptr += incSize;
	}
}
