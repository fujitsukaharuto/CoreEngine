#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <memory>
#include <vector>
#include <wrl/client.h>

#include "DXCommand.h"
#include "DXDescriptor.h"

using namespace Microsoft::WRL;



class DXCommon {
public:
	DXCommon() = default;
	~DXCommon() = default;

public:

	static DXCommon* GetInstance();

	void Initialize();

	void Finalize();

	void PreDraw();

	void PostDraw();

	void CommandExecution();


	/// ==========================================
	/// Getter
	/// ==========================================



private:

	/// Device------------------------------------

	void DeviceInitialze();

	void DeviceFinalize();


	/// SwapChain---------------------------------

	void SwapChainInitialze();

	void SwapChainFinalize();

	void ClearBackBuffer();

	void CreateBarrier(D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);

	void Present();


private:

	/// Device------------------------------------

	ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;
	ComPtr<IDXGIAdapter4> useAdapter_ = nullptr;
	ComPtr<ID3D12Device> device_ = nullptr;


	/// Command-----------------------------------

	std::unique_ptr<DXCommand> command_ = nullptr;


	/// Descriptor--------------------------------

	std::unique_ptr<DXDescriptor> descriptor_ = nullptr;


	/// SwapChain---------------------------------

	ComPtr<IDXGISwapChain4> swapChain_ = nullptr;
	std::vector<ComPtr<ID3D12Resource>> swapChainResources_{};
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> rtvHandles_;



};



