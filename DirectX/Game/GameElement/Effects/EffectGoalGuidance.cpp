#include "EffectGoalGuidance.h"
#include"WindowsInfo/WindowsInfo.h"
#include"Math/calc.h"
#include<numbers>

EffectGoalGuidance::EffectGoalGuidance()
{
	model_ = std::make_unique<Sprite>("pause_arrow.png");
}

EffectGoalGuidance::~EffectGoalGuidance()
{
}

void EffectGoalGuidance::Initialize()
{
	model_->Initialize();
}

//ビューポート変換
Matrix4x4 MakeViewPortMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	return {
		width / 2.0f,0,0,0,
		0,-(height / 2.0f),0,0,
		0,0,maxDepth - minDepth,0,
		left + (width / 2.0f),top + (height / 2.0f),minDepth,1
	};
}

//座標変換
Vector3 TransformPosition(const Vector3& v, const Matrix4x4& m) {
	Vector3 result = {
		.x{v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + 1.0f * m.m[3][0]},
		.y{v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + 1.0f * m.m[3][1]},
		.z{v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + 1.0f * m.m[3][2] },
	};
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + 1.0f * m.m[3][3];

	if (w == 0) {
		return Vector3();
	}

	return {
		result.x / w,
		result.y / w,
		result.z / w,
	};

}

float GetYRotate(const Vector2& v) {
	Vector2 offset = { 0,1 };


	float dot = Calc::Dot(offset, v);

	float leng = offset.Length() * Calc::MakeLength(v);

	float angle = std::acos(dot / leng);

	if (v.x < 0) {
		angle *= -1;
	}



	return angle;

}

void EffectGoalGuidance::Update(const Vector3& playerPos, const Vector3& goalPos, const float& goalSize, const Camera& camera)
{
	//向きベクトル
	Vector3 direction = goalPos-playerPos;

	//スプライト配置領域
	Vector3 spritePos = goalPos-(direction.Normalize() * goalSize);
	

	Vector2 windowSize = WindowsInfo::GetInstance()->GetWindowSize();
	Matrix4x4 cameraVPV = camera.GetViewProjection() * MakeViewPortMatrix(0, 0, windowSize.x, windowSize.y, 0, 1);
	
	spritePos = TransformPosition(spritePos, cameraVPV);


	//画面外なら寄せる
	if (areaType_ == AreaType::Squea) {
		if (spritePos.x + scale_.x > area_.x) {
			spritePos.x = area_.x - scale_.x;
		}
		else if (spritePos.x - scale_.x < 0) {
			spritePos.x = scale_.x;
		}

		if (spritePos.y + scale_.y > area_.y) {
			spritePos.y =- scale_.y + area_.y;
		}
		else if (spritePos.y - scale_.y < 0) {
			spritePos.y = scale_.y;
		}
	}
	
	model_->rotate_ = GetYRotate(Vector2{ direction.x,direction.y })-((1.0f/2.0f)*(float)std::numbers::pi);

	model_->pos_ = { spritePos.x,spritePos.y };

	model_->Update();
}

void EffectGoalGuidance::Draw(const Camera* camera)
{

	if (isDraw_) {
		model_->Draw(*camera);
	}

}
