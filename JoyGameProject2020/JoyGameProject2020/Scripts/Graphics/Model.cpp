#include "Model.h"
#include <fstream>
#include "Base/DirectXManager.h"
#include "Graphics/GraphicsManager.h"
#include "Utility/FileUtility.h"
#include "Device/Camera.h"
#include "GameObject/GameObject.h"
#include "Texture.h"
#include "Light.h"
#include "Renderer.h"
#include "Math/MathUtil.h"
#include "Device/Camera.h"

ModelData::ModelData(const std::string& filePath, bool smooth) :
	m_dxManager(DirectXManager::Instance()),
	m_graphicsManager(GraphicsManager::Instance()),
	m_camera(Camera::Instance()),
	vertexoffset(0),
	indexoffset(0){

	m_directory = FileUtility::GetDirectory(filePath);

	LoadObj(filePath, smooth);
	LoadMtl(m_directory + m_matName);

	CreateMaterialBuffer();
}

ModelData::ModelData(const ModelData& other) :
	m_dxManager(other.m_dxManager),
	m_graphicsManager(other.m_graphicsManager),
	m_camera(other.m_camera),
	vertexoffset(other.vertexoffset),
	indexoffset(other.indexoffset),
	m_subsets(other.m_subsets),
	m_materialDescHeap(other.m_materialDescHeap) {
	CreateTransformBuffer();
}

ModelData::~ModelData() {
	if (m_transformBuffer) {
		m_transformBuffer->Unmap(0, nullptr);
	}
}

void ModelData::UpdateConstantBuffer(GameObject* gameObject) {

	auto world = Matrix4::Scale(gameObject->GetScale()) *
		Matrix4::RotationFromQuaternion(gameObject->transform.rotation) *
		gameObject->billboard *
		Matrix4::Translate(gameObject->GetPosition());
	auto view = m_camera.GetViewMatrix();
	auto proj = m_camera.GetProjectionMatrix();
	auto mvp = world * view * proj;

	transformMap->world = world;

	transformMap->rot = Matrix4::RotationFromQuaternion(gameObject->transform.rotation);
	transformMap->view = view;
	transformMap->proj = proj;
	transformMap->mvp = mvp;
	transformMap->color = m_color;
	transformMap->cameraPos = m_camera.GetPosition();
	transformMap->cameraDir = Vector3(0, 1, 0) * Matrix4::RotationFromQuaternion(m_camera.rotation);
	transformMap->lightDir = Light::Instance().GetLightVec().Normalize();
	transformMap->lightColor = Light::Instance().lightColor;
}

void ModelData::LoadObj(const std::string& filePath, bool smooth) {
	std::vector<Vector3> tmpVertices;
	std::vector<Vector3> tmpNormals;
	std::vector<Vector2> tmpUVs;
	std::map<std::string, UINT> tmpMatIndexMap;
	std::map<USHORT, std::vector<USHORT>> smoothData;
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
				if (smooth) {
					smoothData[positionIndex - 1].push_back(m_vertices.size() - 1);
				}

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

	for (int i = 0; i < tmpVertices.size(); ++i) {
		auto vertexIndices = smoothData[i];

		Vector3 average;
		for (int j = 0; j < vertexIndices.size(); ++j) {
			average += m_vertices[vertexIndices[j]].normal;
		}
		average /= vertexIndices.size();
		for (int j = 0; j < vertexIndices.size(); ++j) {
			m_vertices[vertexIndices[j]].normal = average;
		}
	}

	vertexCount = m_vertices.size();
	indexCount = m_indices.size();
}

void ModelData::LoadMtl(const std::string& filePath) {
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

void ModelData::CreateTransformBuffer() {
	UINT transformBufferSize = (sizeof(Transform) + 0xff) & ~0xff;

	auto heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto resdesc = CD3DX12_RESOURCE_DESC::Buffer(transformBufferSize);
	auto result = m_dxManager.GetDevice()->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_transformBuffer.ReleaseAndGetAddressOf())
	);
	_ASSERT_EXPR(SUCCEEDED(result), L"バッファの生成に失敗しました。");

	/* 割り当てだけしておく */
	m_transformBuffer->Map(0, nullptr, (void**)&transformMap);

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

void ModelData::CreateMaterialBuffer() {

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

Model::Model(const std::string& modelname) :
	renderer(Renderer::Instance()),
	m_dxManager(DirectXManager::Instance()),
	m_graphicsManager(GraphicsManager::Instance()) {
	modeldata = GraphicsManager::Instance().GetModel(modelname);
}

Model::~Model() {
}

void Model::Update() {
	modeldata->UpdateConstantBuffer(gameObject.lock().get());
	renderer.AddModel(modeldata);
}

void Model::SetTexture(int matNum, const std::string& texName) {
	auto textureBuffer = GraphicsManager::Instance().GetTexture(texName).get()->GetBuffer();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	auto descHandle = modeldata->m_materialDescHeap->GetCPUDescriptorHandleForHeapStart();
	auto incSize = m_dxManager.GetDevice()->GetDescriptorHandleIncrementSize(
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	descHandle.ptr += (matNum * 5 + 2) * incSize;
	m_dxManager.GetDevice()->CreateShaderResourceView(
		textureBuffer, &srvDesc, descHandle);
}

void Model::SetColor(const Color4& color) {
	modeldata->m_color = color;
}
