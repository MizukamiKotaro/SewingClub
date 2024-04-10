#include "Camera.h"
#include "Engine/Base/WindowsInfo/WindowsInfo.h"
#include "DirectXBase/DirectXBase.h"

Matrix4x4 Camera::orthographicMat_ = Matrix4x4::MakeOrthographicMatrix(0.0f, 0.0f, 1280.0f, 720.0f, 0.0f, 1.0f);

Camera::Camera()
{
	transform_.scale_ = { 1.0f,1.0f,1.0f };
	transform_.rotate_ = { 0.0f,0.0f,0.0f };
	transform_.translate_ = { 0.0f,0.0f,-15.0f };
	transform_.UpdateMatrix();

	CreateResource();
	
	Matrix4x4 viewMatrix = Matrix4x4::Inverse(transform_.worldMat_);

	Vector2 windowSize = WindowsInfo::GetInstance()->GetWindowSize();
	projectionMatrix_ = Matrix4x4::MakePerspectiveFovMatrix(0.45f, windowSize.x / windowSize.y, 0.1f, 1050.0f);
	viewProjectionMatrix_ = viewMatrix * projectionMatrix_;

	orthographicMat_ = Matrix4x4::MakeOrthographicMatrix(0.0f, 0.0f, windowSize.x, windowSize.y, 0.0f, 1.0f);
}

Camera::~Camera()
{
	cameraForGPUResource_->Release();
}



void Camera::Initialize()
{
	transform_.scale_ = { 1.0f,1.0f,1.0f };
	transform_.rotate_ = { 0.0f,0.0f,0.0f };
	transform_.translate_ = { 0.0f,1.0f,-25.0f };
	transform_.UpdateMatrix();

	Matrix4x4 viewMatrix = Matrix4x4::Inverse(transform_.worldMat_);
	Vector2 windowSize = WindowsInfo::GetInstance()->GetWindowSize();
	projectionMatrix_ = Matrix4x4::MakePerspectiveFovMatrix(0.45f, windowSize.x / windowSize.y, 0.1f, 1050.0f);
	viewProjectionMatrix_ = viewMatrix * projectionMatrix_;
}

void Camera::Update()
{
	transform_.UpdateMatrix();
	Matrix4x4 viewMatrix = Matrix4x4::Inverse(transform_.worldMat_);
	viewProjectionMatrix_ = viewMatrix * projectionMatrix_;

	cameraForGPUData_->worldPosition = transform_.GetWorldPosition();
}

const bool Camera::InScreenCheck2D(const Vector3& position, const float& radius) const 
{
	Vector2 win = WindowsInfo::GetInstance()->GetWindowSize();
	float ratio = win.y / (std::tanf(0.225f) * (position.z - transform_.translate_.z) * 2);

	Vector2 pos{};
	pos.x = position.x * ratio - transform_.translate_.x * ratio + transform_.translate_.x;
	pos.y = position.y * ratio - transform_.translate_.y * ratio + transform_.translate_.y;

	float size = radius * ratio;

	if ((std::abs(pos.x) - size > win.x * 0.7f) || (std::abs(pos.y) - size > win.y * 0.7f)) {
		return false;
	}
	return true;
}

void Camera::CreateResource()
{
	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	cameraForGPUResource_ = DirectXBase::CreateBufferResource(sizeof(CameraForGPU));
	cameraForGPUData_ = nullptr;
	cameraForGPUResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraForGPUData_));
	cameraForGPUData_->worldPosition = transform_.GetWorldPosition();
}
