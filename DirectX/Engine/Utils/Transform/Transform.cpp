#include "Transform.h"
#include "Utils/Math/Vector2.h"

Transform::Transform()
{

	scale_ = { 1.0f,1.0f,1.0f };
	rotate_ = { 0.0f,0.0f,0.0f };
	translate_ = { 0.0f,0.0f,0.0f };

	worldMat_ = Matrix4x4::MakeAffinMatrix(*this);

	worldPos_.x = worldMat_.m[3][0];
	worldPos_.y = worldMat_.m[3][1];
	worldPos_.z = worldMat_.m[3][2];

	isQua_ = false;
	rot_ = Quaternion::Identity();
}

void Transform::Initialize()
{
	
}

void Transform::UpdateMatrix()
{
	if (isQua_) {
		worldMat_ = Matrix4x4::MakeScaleMatrix(scale_) * Matrix4x4::MakeRotateMatrix(rot_) * Matrix4x4::MakeTranslateMatrix(translate_);
	}
	else {
		worldMat_ = Matrix4x4::MakeAffinMatrix(*this);
	}

	if (parent_) {
		worldMat_ = Matrix4x4::Multiply(worldMat_, parent_->worldMat_);
	}
	if (worldTranslateParent_) {
		worldMat_.m[3][0] += worldTranslateParent_->worldMat_.m[3][0];
		worldMat_.m[3][1] += worldTranslateParent_->worldMat_.m[3][1];
		worldMat_.m[3][2] += worldTranslateParent_->worldMat_.m[3][2];
	}

	worldPos_.x = worldMat_.m[3][0];
	worldPos_.y = worldMat_.m[3][1];
	worldPos_.z = worldMat_.m[3][2];
}

const Vector3& Transform::GetWorldPosition() const
{
	return worldPos_;
}

bool Transform::IsCollisionXZ(const Transform& transform)
{

	Vector2 min0 = { worldPos_.x - scale_.x,worldPos_.z - scale_.z };
	Vector2 max0 = { worldPos_.x + scale_.x,worldPos_.z + scale_.z };

	Vector2 min1 = { transform.worldPos_.x - transform.scale_.x,transform.worldPos_.z - transform.scale_.z };
	Vector2 max1 = { transform.worldPos_.x + transform.scale_.x,transform.worldPos_.z + transform.scale_.z };

	if (min0.x <= max1.x && max0.x >= min1.x &&
		min0.y <= max1.y && max0.y >= min1.y) {
		return true;
	}

	return false;
}

void Transform::SetWorldTranslateParent(const Transform* transform)
{
	if (!worldTranslateParent_) {
		worldTranslateParent_ = transform;
		translate_ -= transform->worldPos_;
	}
}

void Transform::ClearWorldTranslateParent()
{
	if (worldTranslateParent_) {
		translate_ += worldTranslateParent_->worldPos_;
		worldTranslateParent_ = nullptr;
	}
}

void Transform::SetQuaRot(const Quaternion& qua)
{
	isQua_ = true;
	rot_ = qua;
}
