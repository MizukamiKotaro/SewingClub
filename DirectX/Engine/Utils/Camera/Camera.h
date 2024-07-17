#pragma once
#include "Utils/Math/Vector3.h"
#include "Utils/Math/Matrix4x4.h"
#include "Utils/Transform/Transform.h"
#include <wrl.h>
#include <d3d12.h>

class Camera {

public:

	struct CameraForGPU
	{
		Vector3 worldPosition;
	};

	Camera();
	~Camera();

	void Initialize();

	void Update();

	const Matrix4x4& GetViewProjection() const { return viewProjectionMatrix_; }
	const Matrix4x4& GetProjection() const { return projectionMatrix_; }
	const Matrix4x4& GetViewMatrix() const { return viewMatrix_; }

	static const Matrix4x4 GetOrthographicMat() { return orthographicMat_; }
	static const Matrix4x4 GetViewportMat() { return viewportMat_; }

	const D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() const { return cameraForGPUResource_->GetGPUVirtualAddress(); }

	const bool InScreenCheck2D(const Vector3& position, const float& radius) const;

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> cameraForGPUResource_;
	CameraForGPU* cameraForGPUData_;

private:

	void CreateResource();

public:

	Transform transform_;

private:

	Matrix4x4 projectionMatrix_;
	Matrix4x4 viewProjectionMatrix_;
	Matrix4x4 viewMatrix_;

	static Matrix4x4 orthographicMat_;
	static Matrix4x4 viewportMat_;

};