#include "DXCommand.h"

#include <cassert>



DXCommand::~DXCommand() {

	list_.Reset();
	allocator_.Reset();
	queue_.Reset();

}


void DXCommand::Initialize(ID3D12Device* device) {

	HRESULT hr;

	/// ------------------------------------------
	/// queue-------------------------------------
	D3D12_COMMAND_QUEUE_DESC queueDesc{};
	hr = device->CreateCommandQueue(
		&queueDesc, IID_PPV_ARGS(&queue_));
	assert(SUCCEEDED(hr));


	/// allocator---------------------------------
	hr = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allocator_));
	assert(SUCCEEDED(hr));


	/// list--------------------------------------
	hr = device->CreateCommandList(
		0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		allocator_.Get(), nullptr, IID_PPV_ARGS(&list_));
	assert(SUCCEEDED(hr));

}


void DXCommand::Close() {

	HRESULT hr;

	hr = list_->Close();
	assert(SUCCEEDED(hr));

}


void DXCommand::Execution() {

	ComPtr<ID3D12CommandList> commandLists[] = { list_.Get() };
	queue_->ExecuteCommandLists(1, commandLists->GetAddressOf());



}


void DXCommand::Reset() {

	HRESULT hr;

	hr = allocator_->Reset();
	assert(SUCCEEDED(hr));
	hr = list_->Reset(allocator_.Get(), nullptr);
	assert(SUCCEEDED(hr));

}
