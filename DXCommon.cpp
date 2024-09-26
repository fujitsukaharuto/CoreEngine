#include "DXCommon.h"

#include <Logger.h>
#include <WinApp.h>
#include <cassert>


#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")



DXCommon* DXCommon::GetInstance() {
	static DXCommon instance;
	return &instance;
}


void DXCommon::Initialize() {

	DeviceInitialze();

	command_.reset(new DXCommand());
	command_->Initialize(device_.Get());

	descriptor_.reset(new DXDescriptor());
	descriptor_->Initialize(device_.Get());

	SwapChainInitialze();


}


void DXCommon::Finalize() {

	SwapChainFinalize();
	descriptor_.reset();
	command_.reset();
	DeviceFinalize();

}


void DXCommon::PreDraw() {

	//CreateBarrier(D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	ClearBackBuffer();

}


void DXCommon::PostDraw() {

	//CreateBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	command_->Close();
	command_->Execution();
	Present();
	command_->Reset();

}


void DXCommon::CommandExecution() {

	command_->Close();
	command_->Execution();
	command_->Reset();

}


void DXCommon::DeviceInitialze() {

	HRESULT hr;

	/// dxgiFactory-------------------------------
	hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
	assert(SUCCEEDED(hr));


	/// useAdapter--------------------------------
	for (UINT i = 0; dxgiFactory_->EnumAdapterByGpuPreference(i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
		IID_PPV_ARGS(&useAdapter_)) != DXGI_ERROR_NOT_FOUND; ++i) {

		DXGI_ADAPTER_DESC3 adapterDesc{};
		hr = useAdapter_->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr));

		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			Logger::Log(std::format(L"Use Adapter:{}", adapterDesc.Description));
			break;
		}

		useAdapter_ = nullptr;//ソフトウェアタブの場合見つからなかったことに
	}
	assert(useAdapter_ != nullptr);


	/// Device------------------------------------
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0
	};
	const char* featureLevelStrings[] = { "12.2","12.1","12.0" };

	//高い順に生成できるか試していく
	for (size_t i = 0; i < _countof(featureLevels); ++i) {
		hr = D3D12CreateDevice(useAdapter_.Get(), featureLevels[i], IID_PPV_ARGS(&device_));
		if (SUCCEEDED(hr)) {
			Logger::Log(std::format("FeatureLevel : {}", featureLevelStrings[i]));
			break;
		}
	}
	assert(device_ != nullptr);
	Logger::Log("Complete creat D3D12Device!!");


}


void DXCommon::DeviceFinalize() {

	device_.Reset();
	useAdapter_.Reset();
	dxgiFactory_.Reset();

}


void DXCommon::SwapChainInitialze() {

	HRESULT hr;

	/// SwapChain---------------------------------

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = WinApp::kWindowSizeX;
	swapChainDesc.Height = WinApp::kWindowSizeY;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	hr = dxgiFactory_->CreateSwapChainForHwnd(
		command_->GetQueue(), WinApp::GetInstance()->GetHWND(),
		&swapChainDesc, nullptr, nullptr,
		reinterpret_cast<IDXGISwapChain1**>(swapChain_.GetAddressOf()));
	assert(SUCCEEDED(hr));


	/// SwapChainResource-------------------------

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	swapChainResources_.resize(2);
	rtvHandles_.resize(2);

	for (uint32_t i = 0; i < 2; i++) {

		hr = swapChain_->GetBuffer(i, IID_PPV_ARGS(&swapChainResources_[i]));
		assert(SUCCEEDED(hr));

		rtvHandles_[i] = descriptor_->GetRTVCPUHandle();
		descriptor_->AddRTVCount();
		device_->CreateRenderTargetView(
			swapChainResources_[i].Get(), &rtvDesc, rtvHandles_[i]);

	}


}


void DXCommon::SwapChainFinalize() {

	swapChain_.Reset();

}


void DXCommon::ClearBackBuffer() {

	UINT index = swapChain_->GetCurrentBackBufferIndex();

	command_->GetList()->OMSetRenderTargets(1, &rtvHandles_[index], false, nullptr);
	float clearColor[] = { 0.1f, 0.25f, 0.5f, 1.0f };
	command_->GetList()->ClearRenderTargetView(rtvHandles_[index], clearColor, 0, nullptr);

}


void DXCommon::CreateBarrier(D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after) {

	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = swapChainResources_[swapChain_->GetCurrentBackBufferIndex()].Get();
	barrier.Transition.StateBefore = before;
	barrier.Transition.StateAfter = after;
	command_->GetList()->ResourceBarrier(1, &barrier);

}


void DXCommon::Present() {

	HRESULT hr;

	hr = swapChain_->Present(1, 0);
	assert(SUCCEEDED(hr));

}
