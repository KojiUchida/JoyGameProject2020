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

ModelData::ModelData(const std::string& filePath, bool smooth) :
	m_dxManager(DirectXManager::Instance()),
	m_graphicsManager(GraphicsManager::Instance()),
	m_vbView(),
	m_ibView() {

	m_directory = FileUtility::GetDirectory(filePath);

	LoadObj(filePath, smooth);
	LoadMtl(m_directory + m_matName);

	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateTransformBuffer();
}

ModelData::ModelData(const ModelData& other) :
	m_dxManager(other.m_dxManager),
	m_graphicsManager(other.m_graphicsManager),
	m_vertices(other.m_vertices),
	m_indices(other.m_indices),
	m_vbView(other.m_vbView),
	m_ibView(other.m_ibView),
	vertexCount(other.vertexCount),
	indexCount(other.indexCount),
	m_subsets(other.m_subsets),
	m_textureMap(other.m_textureMap),
	m_materials(other.m_materials),
	m_materialDescHeap(other.m_materialDescHeap) {
	CreateTransformBuffer();
}

ModelData::~ModelData() {
	m_transformBuffer->Unmap(0, nullptr);
}

void ModelData::UpdateConstantBuffer(GameObject* gameObject) {

	auto world = Matrix4::Scale(gameObject->GetScale()) *
		Matrix4::RotationFromQuaternion(gameObject->transform.rotation) *
		gameObject->billboard *
		Matrix4::Translate(gameObject->GetPosition());
	auto view = Camera::Instance().GetViewMatrix();
	auto proj = Camera::Instance().GetProjectionMatrix();
	auto mvp = world * view * proj;

	transformMap->world = world;

	transformMap->rot = Matrix4::RotationFromQuaternion(gameObject->transform.rotation);
	transformMap->view = view;
	transformMap->proj = proj;
	transformMap->mvp = mvp;
	transformMap->cameraPos = Camera::Instance().GetPosition();
	transformMap->cameraDir = Vector3(0, 1, 0) * Camera::Instance().GetRotationMatrix();
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
	_ASSERT_EXPR(!fin.fail(), L"�t�@�C���̓ǂݍ��݂Ɏ��s���܂���");

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
	_ASSERT_EXPR(!fin.fail(), L"�t�@�C���̓ǂݍ��݂Ɏ��s���܂���");

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

void ModelData::CreateVertexBuffer() {
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
	_ASSERT_EXPR(SUCCEEDED(result), L"���_�o�b�t�@�̐����Ɏ��s���܂���");

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

void ModelData::CreateIndexBuffer() {
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
	_ASSERT_EXPR(SUCCEEDED(result), L"�C���f�b�N�X�o�b�t�@�̐����Ɏ��s���܂���");

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

void ModelData::CreateTransformBuffer() {
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
	_ASSERT_EXPR(SUCCEEDED(result), L"�o�b�t�@�̐����Ɏ��s���܂����B");

	/* ���蓖�Ă������Ă��� */
	m_transformBuffer->Map(0, nullptr, (void**)&transformMap);

	/* NumDescriptors�̌����̃q�[�v���m�� */
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.NumDescriptors = 1;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	result = m_dxManager.GetDevice()->CreateDescriptorHeap(&descHeapDesc,
		IID_PPV_ARGS(m_transformDescHeap.ReleaseAndGetAddressOf()));
	_ASSERT_EXPR(SUCCEEDED(result), L"�f�B�X�N���v�^�q�[�v�̐����Ɏ��s���܂����B");

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = m_transformBuffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = transformBufferSize;

	/* �m�ۂ����q�[�v�Ɏ��̐��� */
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
	_ASSERT_EXPR(SUCCEEDED(result), L"�o�b�t�@�̐����Ɏ��s���܂����B");

	char* mapMaterial = nullptr;
	result = m_materialBuffer->Map(0, nullptr, (void**)&mapMaterial);
	_ASSERT_EXPR(SUCCEEDED(result), L"�o�b�t�@�̃}�b�v�Ɏ��s���܂����B");
	for (auto& m : m_materials) {
		*((SurfaceMaterial*)mapMaterial) = m.material;//�f�[�^�R�s�[
		mapMaterial += materialBufferSize;//���̃A���C�����g�ʒu�܂Ői�߂�
	}
	m_materialBuffer->Unmap(0, nullptr);

	/* NumDescriptors�̌����̃q�[�v���m�� */
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.NumDescriptors = (UINT)m_materials.size() * 5; /* �萔�o�b�t�@1�� + �e�N�X�`��4�� */
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	result = m_dxManager.GetDevice()->CreateDescriptorHeap(&descHeapDesc,
		IID_PPV_ARGS(m_materialDescHeap.ReleaseAndGetAddressOf()));
	_ASSERT_EXPR(SUCCEEDED(result), L"�f�B�X�N���v�^�q�[�v�̐����Ɏ��s���܂����B");

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = m_materialBuffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = materialBufferSize;

	/* �e�N�X�`���p */
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	/* �m�ۂ����q�[�v�Ɏ��̐��� */
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
	renderer(Renderer::Instance()) {
	modeldata = GraphicsManager::Instance().GetModelData(modelname);
	modeldata->CreateMaterialBuffer();
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

	auto descHandle = modeldata.get()->m_materialDescHeap->GetCPUDescriptorHandleForHeapStart();
	auto incSize = modeldata.get()->m_dxManager.GetDevice()->GetDescriptorHandleIncrementSize(
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	descHandle.ptr += (matNum * 5 + 2) * incSize;
	modeldata.get()->m_dxManager.GetDevice()->CreateShaderResourceView(
		textureBuffer, &srvDesc, descHandle);
}
