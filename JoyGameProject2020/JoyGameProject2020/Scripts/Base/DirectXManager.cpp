#include "DirectXManager.h"
#include <string>
#include "Def/Screen.h"

DirectXManager::DirectXManager() :
	m_fenceVal(0),
	m_viewport() {
}

DirectXManager::~DirectXManager() {
}

DirectXManager& DirectXManager::Instance() {
	static DirectXManager instance;
	return instance;
}

void DirectXManager::Init(HWND hwnd) {
	EnableDebugLayer();
	InitDXGI();
	InitCommand();
	InitSwapChain(hwnd);
	InitDepthBuffer();
	InitRenderTarget();
	CreateDescHeapForImGui();
}

void DirectXManager::EnableDebugLayer() {
	ComPtr<ID3D12Debug> debugLayer = nullptr;
	auto result = D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer));
	_ASSERT_EXPR(SUCCEEDED(result), L"デバッグレイヤーの取得に失敗しました");
	debugLayer->EnableDebugLayer();
}

void DirectXManager::InitDXGI() {
	HRESULT result;

	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	for (int i = 0; i < _countof(levels); ++i) {
		result = D3D12CreateDevice(nullptr, levels[i],
			IID_PPV_ARGS(m_device.ReleaseAndGetAddressOf()));
		if (SUCCEEDED(result)) {
			break;
		}
	}

	result = CreateDXGIFactory1(IID_PPV_ARGS(m_factory.ReleaseAndGetAddressOf()));
	_ASSERT_EXPR(SUCCEEDED(result), L"DXGIFactory1の生成に失敗しました");

	std::vector<ComPtr<IDXGIAdapter>> adapters;
	ComPtr<IDXGIAdapter> tmpAdapters = nullptr;
	for (int i = 0; m_factory->EnumAdapters(i,
		tmpAdapters.ReleaseAndGetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++i) {
		adapters.push_back(tmpAdapters);
	}

	for (int i = 0; i < adapters.size(); ++i) {
		DXGI_ADAPTER_DESC adesc{};
		adapters[i]->GetDesc(&adesc);
		std::wstring strDesc = adesc.Description;
		if (strDesc.find(L"Microsoft") == std::wstring::npos &&
			strDesc.find(L"Intel") == std::wstring::npos) {
			tmpAdapters = adapters[i];
			break;
		}
	}
}

void DirectXManager::InitCommand() {
	auto result = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(m_cmdAllocator.ReleaseAndGetAddressOf()));
	_ASSERT_EXPR(SUCCEEDED(result), L"コマンドアロケータの生成に失敗しました");

	result = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_cmdAllocator.Get(), nullptr, IID_PPV_ARGS(m_cmdList.ReleaseAndGetAddressOf()));
	_ASSERT_EXPR(SUCCEEDED(result), L"コマンドリスト生成に失敗しました");

	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	result = m_device->CreateCommandQueue(&cmdQueueDesc,
		IID_PPV_ARGS(m_cmdQueue.ReleaseAndGetAddressOf()));
	_ASSERT_EXPR(SUCCEEDED(result), L"コマンドキューの生成に失敗しました");
}

void DirectXManager::InitSwapChain(const HWND hwnd) {
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = Screen::WIDTH;
	swapchainDesc.Height = Screen::HEIGHT;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapchainDesc.BufferCount = 2;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	auto result = m_factory->CreateSwapChainForHwnd(
		m_cmdQueue.Get(),
		hwnd,
		&swapchainDesc,
		nullptr,
		nullptr,
		(IDXGISwapChain1**)m_swapchain.ReleaseAndGetAddressOf()
	);
	_ASSERT_EXPR(SUCCEEDED(result), L"スワップチェインの生成に失敗しました");
}

void DirectXManager::InitRenderTarget() {
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	descHeapDesc.NumDescriptors = 2;
	auto result = m_device->CreateDescriptorHeap(&descHeapDesc,
		IID_PPV_ARGS(m_rtvHeap.ReleaseAndGetAddressOf()));
	_ASSERT_EXPR(SUCCEEDED(result), L"ディスクリプタヒープの生成に失敗しました");

	m_backbuffers.resize(2);
	for (auto i = 0; i < m_backbuffers.size(); ++i) {
		result = m_swapchain->GetBuffer(i, IID_PPV_ARGS(m_backbuffers[i].ReleaseAndGetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(result), L"スワップチェインの取得に失敗しました");
		D3D12_CPU_DESCRIPTOR_HANDLE handle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
		handle.ptr += i * m_device->GetDescriptorHandleIncrementSize(descHeapDesc.Type);
		m_device->CreateRenderTargetView(m_backbuffers[i].Get(), nullptr, handle);
	}

	result = m_device->CreateFence(m_fenceVal, D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(m_fence.ReleaseAndGetAddressOf()));
	_ASSERT_EXPR(SUCCEEDED(result), L"フェンスの生成に失敗しました");

}

void DirectXManager::InitDepthBuffer() {
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		Screen::WIDTH, Screen::HEIGHT,
		1, 0, 1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	auto heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	auto clearvalue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, NULL);
	auto result = GetDevice()->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearvalue,
		IID_PPV_ARGS(m_depthBuffer.ReleaseAndGetAddressOf()));
	_ASSERT_EXPR(SUCCEEDED(result), L"深度バッファの生成に失敗しました");


	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

	result = GetDevice()->CreateDescriptorHeap(&dsvHeapDesc,
		IID_PPV_ARGS(m_dsvHeap.ReleaseAndGetAddressOf()));
	_ASSERT_EXPR(SUCCEEDED(result), L"ディスクリプタヒープの生成に失敗しました");

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	GetDevice()->CreateDepthStencilView(m_depthBuffer.Get(),
		&dsvDesc, m_dsvHeap->GetCPUDescriptorHandleForHeapStart());
}

void DirectXManager::CreateDescHeapForImGui() {
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask = 0;
	desc.NumDescriptors = 1;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(m_imGuiDescHeap.ReleaseAndGetAddressOf()));
}

void DirectXManager::BeginScene(const Color4& color) {
	UINT bbIndex = m_swapchain->GetCurrentBackBufferIndex();
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		m_backbuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_cmdList->ResourceBarrier(1, &barrier);

	auto rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), bbIndex,
		m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
	m_cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	float clearColor[] = { color.r ,color.g ,color.b ,color.a };
	m_cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	m_cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	m_viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, Screen::WIDTH, Screen::HEIGHT);
	m_cmdList->RSSetViewports(1, &m_viewport);
	auto rect = CD3DX12_RECT(0, 0, Screen::WIDTH, Screen::HEIGHT);
	m_cmdList->RSSetScissorRects(1, &rect);
}

void DirectXManager::EndScene() {
	UINT bbIndex = m_swapchain->GetCurrentBackBufferIndex();
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		m_backbuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	m_cmdList->ResourceBarrier(1, &barrier);

	m_cmdList->Close();

	ComPtr<ID3D12CommandList> cmdLists[] = { m_cmdList.Get() };
	m_cmdQueue->ExecuteCommandLists(1, cmdLists->GetAddressOf());
	WaitForCommandQueue();
	m_cmdAllocator->Reset();
	m_cmdList->Reset(m_cmdAllocator.Get(), nullptr);
	m_swapchain->Present(0, 0);
}

void DirectXManager::WaitForCommandQueue() {
	m_cmdQueue->Signal(m_fence.Get(), ++m_fenceVal);
	if (m_fence->GetCompletedValue() != m_fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		m_fence->SetEventOnCompletion(m_fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
}

ComPtr<ID3D12Device> DirectXManager::GetDevice() {
	return m_device;
}

ComPtr<ID3D12GraphicsCommandList> DirectXManager::GetCommandList() {
	return m_cmdList;
}

ComPtr<ID3D12CommandQueue> DirectXManager::GetCommandQueue() {
	return m_cmdQueue;
}

D3D12_VIEWPORT DirectXManager::GetViewPort() {
	return m_viewport;
}

ComPtr<ID3D12DescriptorHeap> DirectXManager::GetDescHeapForImGui() {
	return m_imGuiDescHeap;
}

