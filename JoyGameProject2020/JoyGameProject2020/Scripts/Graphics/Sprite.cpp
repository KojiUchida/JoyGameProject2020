#include "Sprite.h"
#include "Base/DirectXManager.h"
#include "Graphics/GraphicsManager.h"
#include "Graphics/Texture.h"
#include "GameObject/GameObject.h"
#include "Device/Camera.h"
#include "SpriteRenderer.h"

SpriteData::SpriteData(const std::string& filePath) :
	m_dxManager(DirectXManager::Instance()),
	m_graphicsManager(GraphicsManager::Instance()),
	m_constBuffer(nullptr),
	m_basicDescHeap(nullptr),
	m_constMap(nullptr),
	m_textureName(filePath),
	m_texture(nullptr) {
	CreateConstantBuffer();
}

SpriteData::~SpriteData() {
}

void SpriteData::Draw() {
	if (!option.isVisible) return;
	ComPtr<ID3D12DescriptorHeap> ppHeaps[] = { m_basicDescHeap };
	m_dxManager.GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps->GetAddressOf());
	auto handle = m_basicDescHeap->GetGPUDescriptorHandleForHeapStart();
	m_dxManager.GetCommandList()->SetGraphicsRootDescriptorTable(0, handle);
	handle.ptr += m_dxManager.GetDevice()->GetDescriptorHandleIncrementSize(
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	m_dxManager.GetCommandList()->SetGraphicsRootDescriptorTable(1, handle);
	m_dxManager.GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void SpriteData::CreateConstantBuffer() {
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

	auto texture = m_graphicsManager.GetTexture(m_textureName);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = texture->GetBuffer()->GetDesc().Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	m_dxManager.GetDevice()->CreateShaderResourceView(texture->GetBuffer(), &srvDesc, basicDescHandle);
}

void SpriteData::UpdateConstantBuffer(GameObject* gameObject) {
	auto obj = gameObject;

	auto world =
		Matrix4::Translate(Vector3(0.5f - option.pivot.x, option.pivot.y - 0.5f, 0)) *
		Matrix4::Scale(Vector3(option.flipX ? -obj->GetScale().x : obj->GetScale().x, option.flipY ? obj->GetScale().y : -obj->GetScale().y, 0)) *
		Matrix4::RotateZ(-obj->GetRotation().z) *
		Matrix4::Translate(Vector3(obj->GetPosition().x, obj->GetPosition().y, obj->GetPosition().z));

	m_constMap->color = option.color;
	m_constMap->mat = world * Camera::Instance().GetOrthoMatrix();
}

Sprite::Sprite(const std::string& name) :
	m_spriteRenderer(SpriteRenderer::Instance()) {
	m_spriteData = std::make_shared<SpriteData>(name);
}

Sprite::~Sprite() {
}

void Sprite::Update() {
	m_spriteData->UpdateConstantBuffer(gameObject.lock().get());
	m_spriteRenderer.Add(m_spriteData);
}

void Sprite::SetColor(const Color4& color) {
	m_spriteData->option.color = color;
}

Color4 Sprite::GetColor() {
	return m_spriteData->option.color;
}

void Sprite::SetPivot(const Vector2& pivot) {
	m_spriteData->option.pivot = pivot;
}

void Sprite::SetVisible(bool isVisible) {
	m_spriteData->option.isVisible = isVisible;
}

void Sprite::SetFlipX(bool isFlip) {
	m_spriteData->option.flipX = isFlip;
}

void Sprite::SetFlipY(bool isFlip) {
	m_spriteData->option.flipY = isFlip;
}

void Sprite::SetTexture(const std::string& name) {
}