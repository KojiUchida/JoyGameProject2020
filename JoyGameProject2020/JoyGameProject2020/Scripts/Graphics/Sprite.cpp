#include "Sprite.h"
#include "Base/DirectXManager.h"
#include "Graphics/GraphicsManager.h"
#include "Graphics/Texture.h"
#include "GameObject/GameObject.h"
#include "Device/Camera.h"

Sprite::Sprite(const std::string& name) :
	m_graphicsManager(GraphicsManager::Instance()),
	m_dxManager(DirectXManager::Instance()),
	m_vertexBuffer(nullptr),
	m_indexBuffer(nullptr),
	m_vbView(),
	m_ibView(),
	m_rootSignature(nullptr),
	m_pipelineState(nullptr),
	m_constBuffer(nullptr),
	m_basicDescHeap(nullptr),
	m_constMap(nullptr),
	m_textureName(name),
	m_texture(nullptr),
	m_isVisible(true),
	m_flipX(false),
	m_flipY(false) {
}

Sprite::~Sprite() {
}

void Sprite::Init() {
	InitRootSignature();
	InitPipeline();
	InitVertexBuffer();
	InitIndexBuffer();
	CreateConstantBuffer();
}

void Sprite::Update() {
	UpdateConstantBuffer();
	Draw();
}

void Sprite::Shutdown() {
}

void Sprite::Draw() {

	m_dxManager.GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_dxManager.GetCommandList()->SetPipelineState(m_pipelineState.Get());
	m_dxManager.GetCommandList()->SetGraphicsRootSignature(m_rootSignature.Get());

	m_dxManager.GetCommandList()->IASetVertexBuffers(0, 1, &m_vbView);
	m_dxManager.GetCommandList()->IASetIndexBuffer(&m_ibView);

	if (!m_isVisible) return;

	ComPtr<ID3D12DescriptorHeap> ppHeaps[] = { m_basicDescHeap };
	m_dxManager.GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps->GetAddressOf());

	auto handle = m_basicDescHeap->GetGPUDescriptorHandleForHeapStart();
	m_dxManager.GetCommandList()->SetGraphicsRootDescriptorTable(0, handle);
	handle.ptr += m_dxManager.GetDevice()->GetDescriptorHandleIncrementSize(
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	m_dxManager.GetCommandList()->SetGraphicsRootDescriptorTable(1, handle);

	m_dxManager.GetCommandList()->DrawIndexedInstanced(m_indices.size(), 1, 0, 0, 0);
}

void Sprite::SetColor(const Color4& color) {
	m_color = color;
}

Color4 Sprite::GetColor() {
	return m_color;
}

void Sprite::SetPivot(const Vector2& pivot) {
	m_pivot = pivot;
}

void Sprite::SetVisible(bool isVisible) {
	m_isVisible = isVisible;
}

void Sprite::SetFlipX(bool isFlip) {
	m_flipX = isFlip;
}

void Sprite::SetFlipY(bool isFlip) {
	m_flipY = isFlip;
}

void Sprite::InitRootSignature() {
	CD3DX12_DESCRIPTOR_RANGE descTblRangeCBV(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

	CD3DX12_DESCRIPTOR_RANGE descTblRangeSRV(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_ROOT_PARAMETER rootParams[2]{};
	rootParams[0].InitAsDescriptorTable(1, &descTblRangeCBV);
	rootParams[1].InitAsDescriptorTable(1, &descTblRangeSRV);

	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc(
		_countof(rootParams),
		rootParams, 1,
		&samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	);

	ComPtr<ID3DBlob> rootSignatureBlob;
	ComPtr<ID3DBlob> errorBlob;
	auto result = D3D12SerializeRootSignature(
		&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSignatureBlob, &errorBlob
	);

	result = m_dxManager.GetDevice()->CreateRootSignature(
		0,
		rootSignatureBlob->GetBufferPointer(),
		rootSignatureBlob->GetBufferSize(),
		IID_PPV_ARGS(m_rootSignature.ReleaseAndGetAddressOf())
	);
}

void Sprite::InitPipeline() {
	ComPtr<ID3DBlob> vsBlob = m_graphicsManager.GetShader("SpriteVS");
	ComPtr<ID3DBlob> psBlob = m_graphicsManager.GetShader("SpritePS");

	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
	blendDesc.BlendEnable = true;
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.pRootSignature = m_rootSignature.Get();

	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	gpipeline.BlendState.RenderTarget[0] = blendDesc;

	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	gpipeline.SampleDesc.Count = 1;

	auto result = m_dxManager.GetDevice()->CreateGraphicsPipelineState(
		&gpipeline, IID_PPV_ARGS(m_pipelineState.ReleaseAndGetAddressOf())
	);
}

void Sprite::InitVertexBuffer() {
	m_vertices = {
		{Vector3(-0.5f,0.5f,0.0f),Vector2(0,0)},
		{Vector3(0.5f,0.5f,0.0f),Vector2(1,0)},
		{Vector3(0.5f,-0.5f,0.0f),Vector2(1,1)},
		{Vector3(-0.5f,-0.5f,0.0f),Vector2(0,1)},
	};

	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = m_vertices.size() * sizeof(m_vertices[0]);
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	auto result = m_dxManager.GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_vertexBuffer.ReleaseAndGetAddressOf())
	);

	Vertex* vertexMap = nullptr;
	m_vertexBuffer->Map(0, nullptr, (void**)&vertexMap);
	for (int i = 0; i < m_vertices.size(); ++i) {
		vertexMap[i] = m_vertices[i];
	}
	m_vertexBuffer->Unmap(0, nullptr);

	m_vbView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	m_vbView.SizeInBytes = m_vertices.size() * sizeof(m_vertices[0]);
	m_vbView.StrideInBytes = sizeof(m_vertices[0]);
}

void Sprite::InitIndexBuffer() {
	m_indices = {
		0,1,2,
		0,2,3
	};

	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = m_indices.size() * sizeof(m_indices[0]);
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	auto result = m_dxManager.GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_indexBuffer.ReleaseAndGetAddressOf())
	);

	unsigned short* indexMap = nullptr;
	m_indexBuffer->Map(0, nullptr, (void**)&indexMap);
	for (int i = 0; i < m_indices.size(); ++i) {
		indexMap[i] = m_indices[i];
	}
	m_indexBuffer->Unmap(0, nullptr);

	m_ibView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
	m_ibView.Format = DXGI_FORMAT_R16_UINT;
	m_ibView.SizeInBytes = m_indices.size() * sizeof(m_indices[0]);
}

void Sprite::CreateConstantBuffer() {

	D3D12_RESOURCE_DESC cbResDesc{};
	cbResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResDesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;
	cbResDesc.Height = 1;
	cbResDesc.DepthOrArraySize = 1;
	cbResDesc.MipLevels = 1;
	cbResDesc.SampleDesc.Count = 1;
	cbResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	auto result = m_dxManager.GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&cbResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_constBuffer.ReleaseAndGetAddressOf()));

	m_constBuffer->Map(0, nullptr, (void**)&m_constMap);

	D3D12_DESCRIPTOR_HEAP_DESC basicDescHeapDesc{};
	basicDescHeapDesc.NumDescriptors = 2;
	basicDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	basicDescHeapDesc.NodeMask = 0;
	basicDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	result = m_dxManager.GetDevice()->CreateDescriptorHeap(
		&basicDescHeapDesc, IID_PPV_ARGS(m_basicDescHeap.ReleaseAndGetAddressOf()));

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = m_constBuffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)m_constBuffer->GetDesc().Width;

	D3D12_CPU_DESCRIPTOR_HANDLE basicDescHandle = m_basicDescHeap->GetCPUDescriptorHandleForHeapStart();
	m_dxManager.GetDevice()->CreateConstantBufferView(&cbvDesc, basicDescHandle);
	basicDescHandle.ptr += m_dxManager.GetDevice()->GetDescriptorHandleIncrementSize(
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	m_texture = m_graphicsManager.GetTexture(m_textureName);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = m_texture->GetBuffer()->GetDesc().Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	m_dxManager.GetDevice()->CreateShaderResourceView(m_texture->GetBuffer(), &srvDesc, basicDescHandle);
}

void Sprite::UpdateConstantBuffer() {
	auto obj = gameObject.lock();

	auto world =
		Matrix4::Translate(Vector3(0.5f - m_pivot.x, m_pivot.y - 0.5f, 0)) *
		Matrix4::Scale(Vector3(m_flipX ? -obj->GetScale().x : obj->GetScale().x, m_flipY ? obj->GetScale().y : -obj->GetScale().y, 0)) *
		Matrix4::RotateZ(-obj->GetRotation().z) *
		Matrix4::Translate(Vector3(obj->GetPosition().x, obj->GetPosition().y, obj->GetPosition().z));

	auto mat = world * Camera::Instance().GetOrthoMatrix();

	m_constMap->color = GetColor();
	m_constMap->mat = mat;
}
