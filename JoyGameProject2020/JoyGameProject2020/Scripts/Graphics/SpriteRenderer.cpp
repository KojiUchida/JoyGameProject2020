#include "SpriteRenderer.h"
#include "Base/DirectXManager.h"
#include "GraphicsManager.h"
#include "Sprite.h"

SpriteRenderer::SpriteRenderer() :
	m_dxManager(DirectXManager::Instance()),
	m_renderManager(GraphicsManager::Instance()),
	m_vertexBuffer(nullptr),
	m_indexBuffer(nullptr),
	m_vbView(),
	m_ibView(),
	m_rootSignature(nullptr),
	m_pipelineState(nullptr) {
}

SpriteRenderer::~SpriteRenderer() {
}

SpriteRenderer& SpriteRenderer::Instance() {
	static SpriteRenderer instance;
	return instance;
}

void SpriteRenderer::Init() {
	InitVertexBuffer();
	InitIndexBuffer();
	InitRootSignature();
	InitPipeline();
}

void SpriteRenderer::Draw() {
	m_dxManager.GetCommandList()->SetGraphicsRootSignature(m_rootSignature.Get());
	m_dxManager.GetCommandList()->SetPipelineState(m_pipelineState.Get());

	m_dxManager.GetCommandList()->IASetVertexBuffers(0, 1, &m_vbView);
	m_dxManager.GetCommandList()->IASetIndexBuffer(&m_ibView);
	m_dxManager.GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (auto s : m_sprites) {
		s->Draw();
	}
}

void SpriteRenderer::Add(std::shared_ptr<SpriteData> sprite) {
	m_sprites.push_back(sprite);
}

bool SpriteRenderer::InitRootSignature() {
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
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSignatureBlob;
	ComPtr<ID3DBlob> errorBlob;
	auto result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSignatureBlob, &errorBlob);
	if (FAILED(result)) {
		return false;
	}
	result = m_dxManager.GetDevice()->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(), rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(m_rootSignature.ReleaseAndGetAddressOf()));
	if (FAILED(result)) {
		return false;
	}

	return true;
}

bool SpriteRenderer::InitPipeline() {
	ComPtr<ID3DBlob> vsBlob = m_renderManager.GetShader("SpriteVS");
	ComPtr<ID3DBlob> psBlob = m_renderManager.GetShader("SpritePS");

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

	auto result = m_dxManager.GetDevice()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(m_pipelineState.ReleaseAndGetAddressOf()));
	if (FAILED(result)) {
		return false;
	}

	return true;
}

bool SpriteRenderer::InitVertexBuffer() {
	m_vertices =
	{
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

	auto heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto result = m_dxManager.GetDevice()->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_vertexBuffer.ReleaseAndGetAddressOf())
	);
	if (FAILED(result)) {
		return false;
	}

	Vertex* vertexMap = nullptr;
	m_vertexBuffer->Map(0, nullptr, (void**)&vertexMap);
	for (int i = 0; i < m_vertices.size(); ++i) {
		vertexMap[i] = m_vertices[i];
	}
	m_vertexBuffer->Unmap(0, nullptr);

	m_vbView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	m_vbView.SizeInBytes = m_vertices.size() * sizeof(m_vertices[0]);
	m_vbView.StrideInBytes = sizeof(m_vertices[0]);

	return true;
}

bool SpriteRenderer::InitIndexBuffer() {
	m_indices =
	{
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

	auto heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto result = m_dxManager.GetDevice()->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_indexBuffer.ReleaseAndGetAddressOf())
	);
	if (FAILED(result)) {
		return false;
	}

	unsigned short* indexMap = nullptr;
	m_indexBuffer->Map(0, nullptr, (void**)&indexMap);
	for (int i = 0; i < m_indices.size(); ++i) {
		indexMap[i] = m_indices[i];
	}
	m_indexBuffer->Unmap(0, nullptr);

	m_ibView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
	m_ibView.Format = DXGI_FORMAT_R16_UINT;
	m_ibView.SizeInBytes = m_indices.size() * sizeof(m_indices[0]);

	return true;
}
