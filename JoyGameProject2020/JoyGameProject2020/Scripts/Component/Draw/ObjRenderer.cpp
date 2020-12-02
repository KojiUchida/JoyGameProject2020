#include "ObjRenderer.h"
#include "Base/DirectXManager.h"
#include "Graphics/GraphicsManager.h"
#include "GameObject/GameObject.h"
#include "Graphics/ObjModel.h"

ObjRenderer::ObjRenderer(const std::string& modelName) :
	m_dxManager(DirectXManager::Instance()),
	m_graphicsManager(GraphicsManager::Instance()) {
	
	m_model = std::make_shared<ObjModel>(*m_graphicsManager.GetObjModel(modelName));
	CreateRootSignature();
	CreatePipeline();
}

ObjRenderer::~ObjRenderer() {
}

void ObjRenderer::Update() {
	Draw();
}

void ObjRenderer::Draw() {
	m_dxManager.GetCommandList()->SetGraphicsRootSignature(m_rootSignature.Get());
	m_dxManager.GetCommandList()->SetPipelineState(m_pipelineState.Get());

	m_model->Draw(gameObject.lock().get());
}

void ObjRenderer::CreateRootSignature() {
	CD3DX12_DESCRIPTOR_RANGE descTblRanges[3]{};
	descTblRanges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	descTblRanges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);
	descTblRanges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 4, 0);

	CD3DX12_ROOT_PARAMETER rootParams[2]{};
	rootParams[0].InitAsDescriptorTable(1, &descTblRanges[0]);
	rootParams[1].InitAsDescriptorTable(2, &descTblRanges[1]);

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
	auto result = D3D12SerializeRootSignature(&rootSignatureDesc, 
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSignatureBlob, &errorBlob);
	_ASSERT_EXPR(SUCCEEDED(result), L"ルートシグネチャのシリアル化に失敗しました");

	result = m_dxManager.GetDevice()->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(), 
		rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(m_rootSignature.ReleaseAndGetAddressOf()));
	_ASSERT_EXPR(SUCCEEDED(result), L"ルートシグネチャの生成に失敗しました");
}

void ObjRenderer::CreatePipeline() {
	ComPtr<ID3DBlob> vsBlob = m_graphicsManager.GetShader("ObjVS");
	ComPtr<ID3DBlob> psBlob = m_graphicsManager.GetShader("ObjPS");

	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
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
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

	gpipeline.BlendState.RenderTarget[0] = blendDesc;

	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	gpipeline.SampleDesc.Count = 1;

	auto result = m_dxManager.GetDevice()->CreateGraphicsPipelineState(&gpipeline, 
		IID_PPV_ARGS(m_pipelineState.ReleaseAndGetAddressOf()));
	_ASSERT_EXPR(SUCCEEDED(result), L"パイプラインの生成に失敗しました");
}
