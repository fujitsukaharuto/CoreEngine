#include "DXDescriptor.h"

#include <cassert>



DXDescriptor::~DXDescriptor() {

	rtvHeap_.Reset();

}


void DXDescriptor::Initialize(ID3D12Device* device) {

	rtvHeap_ = CreateDescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 8, false);
	rtvHeapSize_ = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);


}


D3D12_CPU_DESCRIPTOR_HANDLE DXDescriptor::GetRTVCPUHandle() {

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
	cpuHandle.ptr += (rtvHeapSize_ * rtvCount_);

	return cpuHandle;

}


void DXDescriptor::AddRTVCount() {
	rtvCount_++;
}


ComPtr<ID3D12DescriptorHeap> DXDescriptor::CreateDescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, [[maybe_unused]] bool shaderVisible) {

	ComPtr<ID3D12DescriptorHeap> descriptorHeap;
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = heapType;
	heapDesc.NumDescriptors = numDescriptors;
	//heapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	HRESULT hr = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&descriptorHeap));
	assert(SUCCEEDED(hr));

	return descriptorHeap;
}


//D3D12_CPU_DESCRIPTOR_HANDLE DXDescriptor::GetCpuHandle(ID3D12DescriptorHeap* heap, uint32_t size, uint32_t index) {
//	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = heap->GetCPUDescriptorHandleForHeapStart();
//	cpuHandle.ptr += (size * index);
//	return cpuHandle;
//}
