#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <vector>
#include "Utility/Color4.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

using namespace Microsoft::WRL;

class DirectXManager {

private:
	DirectXManager();
	~DirectXManager();
	DirectXManager(const DirectXManager&) = delete;
	void operator=(const DirectXManager&) = delete;

public:
	static DirectXManager& Instance();

	/* 初期化 */
	void Init(const HWND hwnd);
	/* シーン開始 */
	void BeginScene(const Color4& color = Color4(0, 0, 0, 1));
	/* シーン終了 */
	void EndScene();
	/* コマンドキューの処理が終わるまで待機 */
	void WaitForCommandQueue();

	/* デバイスの取得 */
	ComPtr<ID3D12Device> GetDevice();
	/* コマンドリストの取得 */
	ComPtr<ID3D12GraphicsCommandList> GetCommandList();
	/* コマンドキューの取得 */
	ComPtr<ID3D12CommandQueue> GetCommandQueue();
	/* ビューポートの取得 */
	D3D12_VIEWPORT GetViewPort();
	/* ImGui用ディスクリプタヒープの取得 */
	ComPtr<ID3D12DescriptorHeap> GetDescHeapForImGui();

private:
	/* デバッグレイヤーを有効にする */
	void EnableDebugLayer();
	/* DXGI周りの初期化 */
	void InitDXGI();
	/* コマンド周りの初期化 */
	void InitCommand();
	/* スワップチェイン周りの初期化 */
	void InitSwapChain(HWND hwnd);
	/* レンダーターゲット周りの初期化 */
	void InitRenderTarget();
	/* 深度バッファ周りの初期化 */
	void InitDepthBuffer();
	/* ImGui用にディスクリプタヒープの作成 */
	void CreateDescHeapForImGui();

private:
	ComPtr<ID3D12Device> m_device;
	ComPtr<IDXGIFactory6> m_factory;
	ComPtr<IDXGISwapChain4> m_swapchain;
	ComPtr<ID3D12CommandAllocator> m_cmdAllocator;
	ComPtr<ID3D12GraphicsCommandList> m_cmdList;
	ComPtr<ID3D12CommandQueue> m_cmdQueue;
	ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
	ComPtr<ID3D12Fence> m_fence;
	ComPtr<ID3D12Resource> m_depthBuffer;
	ComPtr<ID3D12DescriptorHeap> m_dsvHeap;
	UINT m_fenceVal;
	std::vector<ComPtr<ID3D12Resource>> m_backbuffers;
	D3D12_VIEWPORT m_viewport;

	ComPtr<ID3D12DescriptorHeap> m_imGuiDescHeap;
};

