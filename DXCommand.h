#pragma once
#include <d3d12.h>
#include <wrl/client.h>

using namespace Microsoft::WRL;



class DXCommand {
public:
	DXCommand() = default;
	~DXCommand();

public:

	void Initialize(ID3D12Device* device);

	void Close();

	void Execution();

	void Reset();


	/// ==========================================
	/// Getter
	/// ==========================================

	ID3D12CommandQueue* GetQueue() const { return queue_.Get(); }

	ID3D12GraphicsCommandList* GetList() const { return list_.Get(); }


private:



private:

	ComPtr<ID3D12CommandQueue> queue_ = nullptr;
	ComPtr<ID3D12CommandAllocator> allocator_ = nullptr;
	ComPtr<ID3D12GraphicsCommandList> list_ = nullptr;



};
