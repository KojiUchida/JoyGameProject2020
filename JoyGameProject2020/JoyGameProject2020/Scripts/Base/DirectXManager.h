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

	/* ������ */
	void Init(const HWND hwnd);
	/* �V�[���J�n */
	void BeginScene(const Color4& color = Color4(0, 0, 0, 1));
	/* �V�[���I�� */
	void EndScene();
	/* �R�}���h�L���[�̏������I���܂őҋ@ */
	void WaitForCommandQueue();

	/* �f�o�C�X�̎擾 */
	ComPtr<ID3D12Device> GetDevice();
	/* �R�}���h���X�g�̎擾 */
	ComPtr<ID3D12GraphicsCommandList> GetCommandList();
	/* �R�}���h�L���[�̎擾 */
	ComPtr<ID3D12CommandQueue> GetCommandQueue();
	/* �r���[�|�[�g�̎擾 */
	D3D12_VIEWPORT GetViewPort();
	/* ImGui�p�f�B�X�N���v�^�q�[�v�̎擾 */
	ComPtr<ID3D12DescriptorHeap> GetDescHeapForImGui();

private:
	/* �f�o�b�O���C���[��L���ɂ��� */
	void EnableDebugLayer();
	/* DXGI����̏����� */
	void InitDXGI();
	/* �R�}���h����̏����� */
	void InitCommand();
	/* �X���b�v�`�F�C������̏����� */
	void InitSwapChain(HWND hwnd);
	/* �����_�[�^�[�Q�b�g����̏����� */
	void InitRenderTarget();
	/* �[�x�o�b�t�@����̏����� */
	void InitDepthBuffer();
	/* ImGui�p�Ƀf�B�X�N���v�^�q�[�v�̍쐬 */
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

