#include "ParticleManager.h"
#include "Base/DirectXManager.h"
#include "Graphics/GraphicsManager.h"
#include "Device/Camera.h"
#include "GameObject/GameObject.h"
#include "Graphics/Texture.h"
#include "Math/MathUtil.h"

ParticleManager::ParticleManager() :
	m_dxManager(DirectXManager::Instance()),
	m_graphicsManager(GraphicsManager::Instance()),
	m_camera(Camera::Instance()),
	m_vbView() {
}

ParticleManager::~ParticleManager() {
}

void ParticleManager::Init() {
	CreateVertexBuffer();
	CreateTransformBuffer();
	CreateRootSignature();
	CreatePipeline();
}

void ParticleManager::Update() {

	for (auto&& p : m_particles) {
		p.currentFrame++;
		p.velocity += p.accel;
		p.velocity -= p.velocity * 0.01f;
		p.position += p.velocity;
		float t = (float)p.currentFrame / (float)p.maxFrame;
		p.currentScale = Math::Lerp(p.startScale, p.endScale, t);
		Color4 color;
		color.r = Math::Lerp(p.startColor.r, p.endColor.r, t);
		color.g = Math::Lerp(p.startColor.g, p.endColor.g, t);
		color.b = Math::Lerp(p.startColor.b, p.endColor.b, t);
		color.a = Math::Lerp(p.startColor.a, p.endColor.a, t);
		p.currentColor = color;
	}

	m_particles.remove_if([](Particle& x) {
		return x.currentFrame >= x.maxFrame; }
	);

	Vertex* vertMap = nullptr;
	m_vertexBuffer->Map(0, nullptr, (void**)&vertMap);
	for (auto&& p : m_particles) {
		vertMap->position = p.position;
		vertMap->scale = p.currentScale;
		vertMap->color = p.currentColor;
		vertMap++;
	}
	m_vertexBuffer->Unmap(0, nullptr);

	Transform* transformMap = nullptr;
	m_transformBuffer->Map(0, nullptr, (void**)&transformMap);
	transformMap->mat = m_camera.GetViewMatrix() * m_camera.GetProjectionMatrix();
	transformMap->billboard = Matrix4::RotateRollPitchYaw(m_camera.GetRotation() * Math::ToRadian(1.0f));
	m_transformBuffer->Unmap(0, nullptr);

	Draw();
}

void ParticleManager::Draw() {
	m_dxManager.GetCommandList()->SetGraphicsRootSignature(m_rootSignature.Get());
	m_dxManager.GetCommandList()->SetPipelineState(m_pipelineState.Get());

	m_dxManager.GetCommandList()->IASetVertexBuffers(0, 1, &m_vbView);
	m_dxManager.GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	ComPtr<ID3D12DescriptorHeap> transformHeaps[] = { m_transformDescHeap.Get() };
	m_dxManager.GetCommandList()->SetDescriptorHeaps(_countof(transformHeaps), transformHeaps->GetAddressOf());
	m_dxManager.GetCommandList()->SetGraphicsRootDescriptorTable(0,
		m_transformDescHeap->GetGPUDescriptorHandleForHeapStart());

	ComPtr<ID3D12DescriptorHeap> texHeaps[] = { m_textureDescHeap.Get() };
	m_dxManager.GetCommandList()->SetDescriptorHeaps(_countof(texHeaps), texHeaps->GetAddressOf());
	m_dxManager.GetCommandList()->SetGraphicsRootDescriptorTable(1,
		m_textureDescHeap->GetGPUDescriptorHandleForHeapStart());

	m_dxManager.GetCommandList()->DrawInstanced(
		(UINT)std::distance(m_particles.begin(), m_particles.end()), 1, 0, 0
	);
}

void ParticleManager::CreateVertexBuffer() {

	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = MaxParticle * sizeof(Vertex);
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	auto heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto result = DirectXManager::Instance().GetDevice()->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_vertexBuffer)
	);
	_ASSERT_EXPR(SUCCEEDED(result), L"頂点バッファの生成に失敗しました");

	m_vbView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	m_vbView.SizeInBytes = MaxParticle * sizeof(Vertex);
	m_vbView.StrideInBytes = sizeof(Vertex);
}

void ParticleManager::CreateTransformBuffer() {
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

	result = m_dxManager.GetDevice()->CreateDescriptorHeap(&descHeapDesc,
		IID_PPV_ARGS(m_textureDescHeap.ReleaseAndGetAddressOf()));
	_ASSERT_EXPR(SUCCEEDED(result), L"ディスクリプタヒープの生成に失敗しました。");

	/* テクスチャ用 */
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	/* 確保したヒープに実体生成 */
	auto descHandle = m_textureDescHeap->GetCPUDescriptorHandleForHeapStart();
	auto textureBuffer = m_graphicsManager.GetTexture("effect1")->GetBuffer();
	srvDesc.Format = textureBuffer->GetDesc().Format;
	m_dxManager.GetDevice()->CreateShaderResourceView(
		textureBuffer, &srvDesc, descHandle);
}

void ParticleManager::CreateRootSignature() {
	CD3DX12_DESCRIPTOR_RANGE descTblRanges[2]{};
	descTblRanges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	descTblRanges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_ROOT_PARAMETER rootParams[2]{};
	rootParams[0].InitAsDescriptorTable(1, &descTblRanges[0]);
	rootParams[1].InitAsDescriptorTable(1, &descTblRanges[1]);

	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc(
		_countof(rootParams),
		rootParams,
		1,
		&samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	);

	ComPtr<ID3DBlob> rootSignatureBlob;
	ComPtr<ID3DBlob> errorBlob;
	auto result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSignatureBlob, &errorBlob);
	_ASSERT_EXPR(SUCCEEDED(result), L"ルートシグネチャのシリアル化に失敗しました");

	result = m_dxManager.GetDevice()->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(), rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(m_rootSignature.ReleaseAndGetAddressOf()));
	_ASSERT_EXPR(SUCCEEDED(result), L"ルートシグネチャの生成に失敗しました");
}

void ParticleManager::CreatePipeline() {
	ComPtr<ID3DBlob> vsBlob = m_graphicsManager.GetShader("ParticleVS");
	ComPtr<ID3DBlob> gsBlob = m_graphicsManager.GetShader("ParticleGS");
	ComPtr<ID3DBlob> psBlob = m_graphicsManager.GetShader("ParticlePS");

	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "SCALE", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
	blendDesc.BlendEnable = true;
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.SrcBlend = D3D12_BLEND_ONE;
	blendDesc.DestBlend = D3D12_BLEND_ONE;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.pRootSignature = m_rootSignature.Get();

	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

	gpipeline.BlendState.RenderTarget[0] = blendDesc;

	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	gpipeline.NumRenderTargets = 1;
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	gpipeline.SampleDesc.Count = 1;

	auto result = m_dxManager.GetDevice()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(m_pipelineState.ReleaseAndGetAddressOf()));
	_ASSERT_EXPR(SUCCEEDED(result), L"パイプラインの生成に失敗しました");
}

void ParticleManager::Add(int life, const Vector3& position, const Vector3& velocity, const Vector3& accel,
	float startScale, float endScale, const Color4& startColor, const Color4& endColor) {
	m_particles.emplace_front();
	auto& p = m_particles.front();
	p.position = position;
	p.velocity = velocity;
	p.accel = accel;
	p.maxFrame = life;

	p.startScale = startScale;
	p.endScale = endScale;

	p.startColor = startColor;
	p.endColor = endColor;
}
