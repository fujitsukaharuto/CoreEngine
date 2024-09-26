#pragma once
#include <d3d12.h>
#include <wrl/client.h>
#include <cmath>

using namespace Microsoft::WRL;



class DXDescriptor {
public:
	DXDescriptor() = default;
	~DXDescriptor();

public:

	void Initialize(ID3D12Device* device);


	/// ==========================================
	/// Getter
	/// ==========================================

	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUHandle();

	void AddRTVCount();


private:

	ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

	//D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(ID3D12DescriptorHeap* heap, uint32_t size, uint32_t index);

private:

	ComPtr<ID3D12DescriptorHeap> rtvHeap_ = nullptr;
	uint32_t rtvHeapSize_ = 0;
	uint32_t rtvCount_ = 0;



};
