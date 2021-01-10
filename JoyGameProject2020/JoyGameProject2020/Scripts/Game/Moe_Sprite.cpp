#include "Moe_Sprite.h"
#include "Base/DirectXManager.h"
#include "Def/Screen.h"
#include "Device/Camera.h"
#include "Graphics/GraphicsManager.h"

Moe_Sprite::Moe_Sprite(const Vector3& pos, const Vector2& size, const std::string& texturename)
	:position(pos),size(size),texturename(texturename)
{
	dev = DirectXManager::Instance().GetDevice();
	cmdList = DirectXManager::Instance().GetCommandList();

	//GraphicsManager::Instance().GetTexture(texturename);
}

Moe_Sprite::~Moe_Sprite()
{
}

void Moe_Sprite::initialize()
{
	MatrixClear();
	SetBuffer();
	CreatePipeline();
}

void Moe_Sprite::update()
{
	HRESULT result;

	matWorld = Matrix4::Identity();
	matTrans = Matrix4::Translate(position);
	matScale = Matrix4::Scale(Vector3(width,height,0));

	matRot = Matrix4::Identity();
	matRot = Matrix4::RotateRollPitchYaw(Vector3(0));

	matWorld = matWorld * matScale * matRot * matTrans;

	//�f�[�^�]��
	ConstBufferData*constMap;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = matWorld * Camera::Instance().GetViewMatrix()*matProjection;
	constMap->billboard = Camera::Instance().GetBillboard();
	constBuff->Unmap(0, nullptr);
}

void Moe_Sprite::draw()
{
	//cmdList.Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	//cmdList.Get()->SetPipelineState(pipelinestate.Get());
	//cmdList.Get()->SetGraphicsRootSignature(rootsignature.Get());

	//ID3D12DescriptorHeap* CBVHeaps[] = { CBVHeap->GetDescriptorHeap().Get() };
	//cmdList.Get()->SetDescriptorHeaps(_countof(CBVHeaps), CBVHeaps);

	//cmdList.Get()->SetGraphicsRootDescriptorTable(0, gcbvH);//�s��̒萔�o�b�t�@

	////*******************************�e  �N  �X  �`  ��******************************************
	//ID3D12DescriptorHeap*SRVHeaps[] = { TextureData::Instance().getspriteDescHeap().Get() };
	//cmdList.Get()->SetDescriptorHeaps(_countof(SRVHeaps), SRVHeaps);

	//cmdList.Get()->SetGraphicsRootDescriptorTable(i + constBuffs.size(),
	//	CD3DX12_GPU_DESCRIPTOR_HANDLE(
	//		TextureData::Instance().getspriteDescHeap()->GetGPUDescriptorHandleForHeapStart(),
	//		TextureData::Instance().gettexbuffcount(texnames[i]),
	//		dev.Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	////*******************************************************************************************

	//cmdList.Get()->IASetVertexBuffers(0, 1, &vbView);

	//cmdList.Get()->DrawIndexedInstanced(vertices.size(), 1, 0, 0, 0); //�}�e���A���ɂ���ăC���f�b�N�X�𕪂���
}

void Moe_Sprite::MatrixClear()
{
	matProjection = Matrix4::Perspective(60, Screen::WIDTH / Screen::HEIGHT, 0.1f, 1000.0f);

	//�s��B
	matScale = Matrix4::Identity();
	matRot = Matrix4::Identity();

	matWorld = Matrix4::Identity();
	matWorld = matWorld * matScale * matRot;
}

void Moe_Sprite::SetBuffer()
{
	HRESULT result;

	vertices =
	{
		{Vector3(0.0f,height,0.0f)},//����
	};

	result = dev.Get()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(vertices.size() * sizeof(vertices[0])),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));

	SpriteVertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);

	for (int i = 0; i < vertices.size(); i++)
	{
		vertMap[i] = vertices.at(i);
	}
	vertBuff->Unmap(0, nullptr);

	//���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = vertices.size() * sizeof(vertices[0]);
	vbView.StrideInBytes = sizeof(vertices[0]);

	//�萔�o�b�t�@�̍쐬
	result = dev.Get()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));

	//�f�[�^�]��
	ConstBufferData*constMap;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = matWorld * Camera::Instance().GetViewMatrix()*matProjection;
	constMap->billboard = Camera::Instance().GetBillboard();
	constBuff->Unmap(0, nullptr);

	////�萔�o�b�t�@�r���[�쐬(�萔�o�b�t�@���Z�b�g)
	//D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	//cbvDesc.BufferLocation = constBuff->GetGPUVirtualAddress();
	//cbvDesc.SizeInBytes = (UINT)constBuff->GetDesc().Width;
	//cbvH = CBVHeap->GetCPUHandle();
	//dev.Get()->CreateConstantBufferView(&cbvDesc, cbvH);

}

void Moe_Sprite::CreatePipeline()
{
	HRESULT result;

	CD3DX12_DESCRIPTOR_RANGE descRangeCBV{};
	descRangeCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsDescriptorTable(1, &descRangeCBV, D3D12_SHADER_VISIBILITY_ALL);

	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	//�e�N�X�`���T���v���[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT); // s0 ���W�X�^

	ComPtr<ID3DBlob> errorBlob;//�G���[�I�u�W�F�N�g

	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);



	ComPtr<ID3DBlob> rootSigBlob{};
	//�o�[�W������������ł̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);

	//���[�g�V�O�l�`���̐���
	result = dev.Get()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));


	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	//���[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = rootsignature.Get();

	//�V�F�[�_�[���p�C�v���C���ɃZ�b�g
	ComPtr<ID3DBlob> vsBlob = GraphicsManager::Instance().GetShader("SpriteVS");
	ComPtr<ID3DBlob> gsBlob = GraphicsManager::Instance().GetShader("SpriteGS");
	ComPtr<ID3DBlob> psBlob = GraphicsManager::Instance().GetShader("SpritePS");


	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	//�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	//�W���I�Ȑݒ�(�w�ʃJ�����O�A�h��Ԃ��A�[�x�N���b�s���O�L��)
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;

	//�W���I�Ȑݒ�(�[�x�e�X�g���s���A�������݋��A�[�x����������΍��i)
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	//�u�����h�X�e�[�g�̐ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.BlendEnable = true; // �u�����h��L���ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD; // ���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE; // �\�[�X�̒l�� 100% �g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO; // �f�X�g�̒l�� 0% �g��
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//�\�[�X�̃A���t�@�l
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-�\�[�X�̃A���t�@�l

	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	//���_���C�A�E�g�̐ݒ�
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);


	//�}�`�̌`����O�p�`�̐ݒ�
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//���̑��̐ݒ�
	gpipeline.NumRenderTargets = 1;//�`��Ώ�
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0~255�w���RGBA
	gpipeline.SampleDesc.Count = 1;//1�s�N�Z���ɂ�1��T���v�����O

	//�p�C�v���C���X�e�[�g�̐���
	result = dev.Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));

}

void Moe_Sprite::AddTexture()
{

}

