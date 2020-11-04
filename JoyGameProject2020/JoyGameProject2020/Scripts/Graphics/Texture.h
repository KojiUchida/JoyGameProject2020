#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <DirectXTex.h>
#include <string>

#pragma comment(lib, "DirectXTex.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

class DirectXManager;
class Texture {

public:
	Texture();
	Texture(const std::string& filePath);
	~Texture();

	/* バッファの取得 */
	ID3D12Resource* GetBuffer();

private:
	void CreateWhiteTexture();
	void LoadTextureFromFile(const std::string& filePath);

private:
	DirectXManager& m_dxManager;
	ComPtr<ID3D12Resource> m_textureBuffer;
};

