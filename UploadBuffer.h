#pragma once
#include "d3dUtil.h"

template<typename T>
class UploadBuffer {
public:
	UploadBuffer(ID3D12Device* device, UINT elementCount, bool isConstantBuffer) {
		this->isConstantBuffer = isConstantBuffer;
		elementByteSize = sizeof(T);

		if (isConstantBuffer) elementByteSize = CalcUpper(elementByteSize);

		device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(elementByteSize * elementCount),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&uploadBuffer));
		uploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mappedData));
	}
	~UploadBuffer() {
		if (uploadBuffer != nullptr)
			uploadBuffer->Unmap(0, nullptr);
		mappedData = nullptr;
	}
	void CopyData(int elementIndex, const T& data) {
		memcpy(&mappedData[elementIndex * elementByteSize], &data, sizeof(T));
	}

	ID3D12Resource* uploadBuffer;

private:
	BYTE* mappedData = nullptr;
	UINT elementByteSize = 0;
	bool isConstantBuffer = false;
};