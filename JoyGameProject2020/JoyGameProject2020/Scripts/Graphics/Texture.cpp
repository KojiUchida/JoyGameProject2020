#include "Texture.h"
#include "Utility/FileUtility.h"
#include "Base/DirectXManager.h"

Texture::Texture() :
	m_dxManager(DirectXManager::Instance()) {
	CreateWhiteTexture();
}

Texture::Texture(const std::string& filePath) :
	m_dxManager(DirectXManager::Instance()) {
	LoadTextureFromFile(filePath);
}

Texture::~Texture() {
}

ID3D12Resource* Texture::GetBuffer() {
	return m_textureBuffer.Get();
}

void Texture::CreateWhiteTexture() {
	auto resDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, 4, 4);
	auto texHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
	auto result = m_dxManager.GetDevice()->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(m_textureBuffer.ReleaseAndGetAddressOf())
	);
	_ASSERT_EXPR(SUCCEEDED(result), L"テクスチャバッファの生成に失敗しました");

	std::vector<unsigned char> data(4 * 4 * 4);
	std::fill(data.begin(), data.end(), 0xff);

	result = m_textureBuffer->WriteToSubresource(0, nullptr, data.data(), 4 * 4, (UINT)data.size());
	_ASSERT_EXPR(SUCCEEDED(result), L"テクスチャバッファへの書き込みに失敗しました");
}

void Texture::LoadTextureFromFile(const std::string& filePath) {
	TexMetadata metadata{};
	ScratchImage scratchImage{};

	std::wstring path = FileUtility::GetWideStringFromString(filePath);

	auto result = LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &metadata, scratchImage);
	_ASSERT_EXPR(SUCCEEDED(result), L"テクスチャファイルの読み込みに失敗しました。");
	auto image = scratchImage.GetImage(0, 0, 0);

	D3D12_RESOURCE_DESC texResDesc{};
	texResDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
	texResDesc.Format = metadata.format;
	texResDesc.Width = (UINT16)metadata.width;
	texResDesc.Height = (UINT16)metadata.height;
	texResDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	texResDesc.MipLevels = (UINT16)metadata.mipLevels;
	texResDesc.SampleDesc.Count = 1;

	result = m_dxManager.GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_textureBuffer.ReleaseAndGetAddressOf())
	);
	_ASSERT_EXPR(SUCCEEDED(result), L"テクスチャバッファの生成に失敗しました");

	result = m_textureBuffer->WriteToSubresource(
		0,
		nullptr,
		image->pixels,
		(UINT16)image->rowPitch,
		(UINT16)image->slicePitch
	);
	_ASSERT_EXPR(SUCCEEDED(result), L"テクスチャバッファへの書き込みに失敗しました");
}

